/*
The MIT License (MIT)
Copyright (c) 2016 Vasil Kalchev
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/**
@file
Include file for ExponentMap class.

@author Vasil Kalchev
@date 2017
@version 1.0.0
@copyright The MIT License
*/

#pragma once

#include <inttypes.h>
#include <math.h>

const float optimalExponentDivider = 1.5;

// If "EXPONENTMAP_DEBUG" is already defined - don't overwrite it.
#ifndef EXPONENTMAP_DEBUG
  /// Turns the debugging messages on or off.
  #define EXPONENTMAP_DEBUG false ///< @note Default: false
#endif

#if EXPONENTMAP_DEBUG
  #ifdef ARDUINO_ARCH_AVR
    #include <Arduino.h>
    #define DEBUG(x) Serial.print(x);
    #define DEBUGLN(x) Serial.println(x);
  #else
    #define DEBUG(x) std::cout << x;
    #define DEBUGLN(x) std::cout << x << std::endl;
  #endif
#else
  #define DEBUG(x)
  #define DEBUGLN(x)
#endif

#ifdef ARDUINO_ARCH_AVR
  #define PRINT(x) Serial.print(x);
  #define PRINTLN(x) Serial.println(x);
#else
  #define PRINT(x) std::cout << x;
  #define PRINTLN(x) std::cout << x << std::endl;
#endif


/**
This class calculates an exponential function (from 0 to
a specified value) based on the required steps. The
function is stored in an array with idexes equal to the
steps. The template specifies the integer size to be used
for the array (the default is 32 bits unsigned integer).
*/
template <class T = uint32_t> class ExponentMap {
public:
  /// @name Constructors
  ///@{

  /**
  This constructor takes only the maximum value and calculates
  the optimal number of steps. The optimal number of steps is
  the highest number of steps that doesn't have repeating values
  in the beginning. Then it calculates the exponential function
  for each step.
  @param max_value - the largest value
  */
  ExponentMap(T max_value) noexcept {
    _steps = round( 1 / (log10(2)/(log10(max_value)*optimalExponentDivider)) );
    calculateMap(max_value, optimalExponentDivider);
  }

  /**
  This constructor takes the number of steps and the maximum
  value and calculates the exponential function for each step.
  @param steps - the number of steps
  @param max_value - the largest value
  */
  ExponentMap(T steps, T max_value) noexcept : _steps(steps) {
    double stepsResolutionConstant = (_steps * log10(2)) / (log10(max_value));
    calculateMap(max_value, stepsResolutionConstant);
  }
  ///@}

  /// @name Public methods
  ///@{

  /// Copy constructor
  ExponentMap(const ExponentMap& other)
  : _map(new T[other._steps + 1]) {
    DEBUGLN(F("Copy constructor"));
    _steps = other._steps;
    memcpy(_map, other._map, sizeof(T) * (_steps + 1));
  }

  /// Move contructor
  ExponentMap(ExponentMap&& other) {
    DEBUGLN(F("Move constructor"));
    _steps = other._steps;
    other._steps = 0;
    _map = other._map;
    other._map = nullptr;
  }

  /// Copy assignment operator
  ExponentMap& operator=(const ExponentMap& other) {
    if (&other != this) {
      DEBUGLN(F("Copy assignment operator"));
      _steps = other._steps;
      delete [] _map;
      _map = nullptr;
      _map = new T[other._steps + 1];
      memcpy(_map, other._map, sizeof(T) * (_steps + 1));
    }
    return *this;
  }

  /// Move assignment operator
  ExponentMap operator=(ExponentMap&& other) {
    if (&other != this) {
      DEBUGLN(F("Move assignment operator"));
      _steps = other._steps;
      other._steps = 0;
      delete [] _map;
      _map = other._map;
      other._map = nullptr;
    }
    return *this;
  }

  /**
  The destructor deletes the allocated memory for the array.
  */
  ~ExponentMap() noexcept {
    delete [] _map;
    DEBUGLN(F("Deleted the map array"));
  }

  /**
  This is the main function. It takes a step and returns
  the corresponding value for that step.
  @param step - the step for which a value is requested
  @returns the corresponding value
  */
  T step_to_value(T step) const noexcept {
    if (step <= _steps + 1 && step >= 0) {
      return _map[step];
    } else {
      return 0;
    }
  }

  /**
  @see step_to_value();
  @param step - the step for which a value is requested
  @returns the corresponding value
  */
  T operator()(T step) const noexcept {
    return step_to_value(step);
  }

  /**
  This function returns the number of steps. It is useful
  when the number if steps is not specified in the constructor
  and is automatically calculated.
  @see ExponentMap(T max_value);
  @returns the number of steps
  */
  T stepsCount() const noexcept {
    return _steps + 1;
  }

  /**
  Checks if the chosen combination of steps - maximum value
  generated a table with repeating values.
  @returns true if there are repeating values
  */
  constexpr bool stepsRepeat() const noexcept {
    for (T i = 0; i < _steps; i++) {
      if (_map[i] == _map[i + 1]) {
        return true;
      }
      if (_map[i] + 2 > _map[i + 1]) {
        return false;
      }
    }
    return false;
  }

  /**
  Prints the generated table. First column - "Step", second
  column - "Value".
  */
  constexpr void printTable() const noexcept {
    PRINT(F("Exponent map with ")); PRINT(_steps); PRINT(F(" steps and "));
    PRINT(_map[_steps + 1]); PRINTLN(F(" max value:"));

    PRINTLN(F("Step:\tValue:"));
    for (T step = 0; step <= _steps; step++) {
      PRINT(step); PRINT(F("\t")); PRINTLN(_map[step]);
    }
  }

  /**
  Prints the code for creating the array directly. This is
  useful if you want to get rid of the class.
  */
  constexpr void printCode() const noexcept {
    char dataType[9] = {0};
    if (_map[_steps+1] > 4294967295ull) {
      strcpy(dataType, "uint64_t");
    } else if (_map[_steps+1] > 65535ul) {
      strcpy(dataType, "uint32_t");
    } else if (_map[_steps+1] > 255u) {
      strcpy(dataType, "uint16_t");
    } else {
      strcpy(dataType, "uint8_t");
    }

    PRINT(F("const ")); PRINT(dataType);
    #ifdef ARDUINO_ARCH_AVR
      PRINT(F("[] PROGMEM = {"));
    #else
      PRINT(F("[] = {"));
    #endif
      for (T i = 0; i < _steps; i++) {
        PRINT(_map[i]); PRINT(F(", "));
      }
      PRINT(_map[_steps]); PRINTLN(F("};"));
  }
  ///@}

private:
  /**
  Calculates the step-value array. Called from the constructors.
  @param max_value - the larges value
  @param stepsResolutionConstant - the exponent divider
  */
  void calculateMap(T max_value, double stepsResolutionConstant) {
    DEBUG(F("Exponent map with ")); DEBUG(_steps);
    DEBUG(F(" steps and ")); DEBUG(max_value); DEBUGLN(F(" max value"));
    DEBUG(F("Step-resolution constant: ")); DEBUGLN(stepsResolutionConstant);

    _map = new T[_steps + 1];
    DEBUG(F("Created the map array with length of ")); DEBUG(_steps + 1);
    DEBUG(F(" and address at ")); DEBUGLN((int)&_map);

    DEBUGLN(F("Step:\tValue:"));
    for (T step = 0; step <= _steps; step++) {
      _map[step] = round(pow(2, (step / stepsResolutionConstant)) - 1);
      if (step == _steps) {
        _map[step] = max_value;
      }
      DEBUG(step); DEBUG(F("\t")); DEBUGLN(_map[step]);
    }
    DEBUGLN();
  }
  T _steps; ///< The number of steps
  T* _map; /// Pointer to the map array
};

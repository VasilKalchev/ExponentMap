/*
The MIT License (MIT)
Copyright (c) 2017 Vasil Kalchev
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

#include <stdint.h>
#include <math.h>

// If "EXPONENTMAP_DEBUG" is already defined - don't overwrite it.
#ifndef EXPONENTMAP_DEBUG
/// Turns the debugging messages on or off.
#define EXPONENTMAP_DEBUG false ///< @note Default: false
#endif

#if EXPONENTMAP_DEBUG
#warning "ExponentMap: Debugging messages are enabled."
#ifdef ARDUINO_ARCH_AVR
#include <Arduino.h>
#define EM_DEBUG(x) Serial.print(x);
#define EM_DEBUGLN(x) Serial.println(x);
#else
#define EM_DEBUG(x) std::cout << x;
#define EM_DEBUGLN(x) std::cout << x << std::endl;
#endif
#else
/// Debug print
#define EM_DEBUG(x)
/// Debug print with newline
#define EM_DEBUGLN(x)
#endif

#ifdef ARDUINO_ARCH_AVR
/// Normal print
#define EM_PRINT(x) Serial.print(x);
/// Normal print with newline
#define EM_PRINTLN(x) Serial.println(x);
#else
#define EM_PRINT(x) std::cout << x;
#define EM_PRINTLN(x) std::cout << x << std::endl;
#endif

constexpr char EXPONENTMAP_VERSION[] = "1.0"; ///< The version of the library.

/// This is constant used to calculate the optimal steps for a given maximum value.
constexpr float optimalExponentDivider = 1.5;

/**
This class calculates an exponential function (from 0 to
a specified value) based on the required steps. The
function is stored in an array with idexes being the
steps. The template specifies the integer size to be used
for the array (the default is 32 bits unsigned integer).

@note: The equation is taken from here: https://diarmuid.ie/blog/pwm-exponential-led-fading-on-arduino-or-other-platforms/
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
    _steps = round( 1 / (log10(2) / (log10(max_value) * optimalExponentDivider)) );
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
    EM_DEBUGLN(F("Copy constructor called."));
    _steps = other._steps;
    memcpy(_map, other._map, sizeof(T) * (_steps + 1));
  }

  /// Move contructor
  ExponentMap(ExponentMap&& other) {
    EM_DEBUGLN(F("Move constructor called."));
    _steps = other._steps;
    other._steps = 0;
    _map = other._map;
    other._map = nullptr;
  }

  /// Copy assignment operator
  ExponentMap& operator=(const ExponentMap& other) {
    if (&other != this) {
      EM_DEBUGLN(F("Copy assignment operator called."));
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
      EM_DEBUGLN(F("Move assignment operator called."));
      _steps = other._steps;
      other._steps = 0;
      delete [] _map;
      _map = other._map;
      other._map = nullptr;
    }
    return *this;
  }


  /// The destructor deletes the allocated memory for the array.
  ~ExponentMap() noexcept {
    delete [] _map;
    EM_DEBUGLN(F("Deleted the map array."));
  }

  /**
  This is the main function. It takes a step and returns
  the corresponding value for that step.
  @param step - the step for which a value is requested
  @returns the corresponding value
  */
  T stepToValue(T step) const noexcept {
    if (step <= _steps && step >= 0) {
      return _map[step];
    } else {
      return 0;
    }
  }

  /**
  @see stepToValue();
  @param step - the step for which a value is requested
  @returns the corresponding value
  */
  T operator()(T step) const noexcept {
    return stepToValue(step);
  }

  /**
  This function is the reverse of stepToValue(). It
  takes a value and returns the closest corresponding step.
  @param value - the value for which a step is requested
  @returns the corresponding step
  */
  T valueToStep(T value) const noexcept {
    T s = 0;
    for (s = 0; s <= _steps; ++s) {
      T values[3] = { 0 };
      (s > 0) ? values[0] = stepToValue(s - 1) : values[0] = 0;
      values[1] = stepToValue(s);
      (s < _steps) ? values[2] = stepToValue(s + 1) : values[2] = 0;

      for (uint8_t i = 0; i < 3; ++i) {
        values[i] = abs(values[i] - value);
      }

      if (values[1] <= values[0] && values[1] <= values[2]) {
        break;
      }
    }
    return s;
  }

  /**
  This function returns the number of steps. It is useful
  when the number of steps is not specified in the constructor
  and is automatically calculated.
  @see ExponentMap(T max_value);
  @returns the number of steps
  */
  T stepsCount() const noexcept {
    return _steps;
  }

  /**
  Checks if the chosen combination of steps and maximum value
  generated a table with repeating values.
  @returns true if there are repeating values
  */
  bool stepsRepeat() const noexcept {
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
  void printTable() const noexcept {
    EM_PRINT(F("Exponent map with ")); EM_PRINT(_steps); EM_PRINT(F(" steps and "));
    EM_PRINT(_map[_steps]); EM_PRINTLN(F(" max value:"));

    EM_PRINTLN(F("Step:\tValue:"));
    for (T step = 0; step <= _steps; step++) {
      EM_PRINT(step); EM_PRINT(F("\t")); EM_PRINTLN(_map[step]);
    }
  }

  /**
  Prints the code for creating the array directly. This is
  useful if you want to get rid of the class.
  */
  void printCode() const noexcept {
    char dataType[9] = {0};
    if (_map[_steps] > 4294967295ull) {
      strcpy(dataType, "uint64_t");
    } else if (_map[_steps] > 65535ul) {
      strcpy(dataType, "uint32_t");
    } else if (_map[_steps] > 255u) {
      strcpy(dataType, "uint16_t");
    } else {
      strcpy(dataType, "uint8_t");
    }

    EM_PRINT(F("const ")); EM_PRINT(dataType);
#ifdef ARDUINO_ARCH_AVR
    EM_PRINT(F("[] PROGMEM = {"));
#else
    EM_PRINT(F("[] = {"));
#endif
    for (T i = 0; i < _steps; i++) {
      EM_PRINT(_map[i]); EM_PRINT(F(", "));
    }
    EM_PRINT(_map[_steps]); EM_PRINTLN(F("};"));
  }
  ///@}

private:
  /**
  Calculates the step-value array.
  @param max_value - the larges value
  @param stepsResolutionConstant - the exponent divider
  */
  void calculateMap(T max_value, double stepsResolutionConstant) {
    EM_DEBUG(F("Exponent map with ")); EM_DEBUG(_steps);
    EM_DEBUG(F(" steps and ")); EM_DEBUG(max_value); EM_DEBUGLN(F(" max value"));
    EM_DEBUG(F("Step-resolution constant: ")); EM_DEBUGLN(stepsResolutionConstant);

    _map = new T[_steps + 1];
    EM_DEBUG(F("Created the map array with length of ")); EM_DEBUG(_steps);
    EM_DEBUG(F(" and address at ")); EM_DEBUGLN((int)&_map);

    EM_DEBUGLN(F("Step:\tValue:"));
    for (T step = 0; step <= _steps; step++) {
      _map[step] = round(pow(2, (step / stepsResolutionConstant)) - 1);
      if (step == _steps) {
        _map[step] = max_value;
      }
      EM_DEBUG(step); EM_DEBUG(F("\t")); EM_DEBUGLN(_map[step]);
    }
    EM_DEBUGLN();
  }
  T _steps; ///< The number of steps
  T* _map; ///< Pointer to the map array
};

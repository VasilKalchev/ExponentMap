/*
 * ExponentMap library - linear_output.ino
 *
 * This example demonstrates how to actually use the object
 * to get values for fading an LED.
 *
 * In this example an LED is faded in-out first using linearly
 * growing values for the PWM, then using the exponentially
 * growing values from the generated array. To get the values
 * from the array use the function operator on the object or
 * the method "stepToValue(step)".
 *
 * The circuit:
 * An LED with the appropriate resistor to Arduino pin 3.
 *
 * Created February 25, 2017
 * by Vasil Kalchev
 *
 * https://github.com/VasilKalchev/ExponentMap
 *
 */

#include <ExponentMap.h>

const byte led_pin = 3;

// This is the maximum value of the PWM output. An 8 bits PWM
// has 2^8 = 256 states, a 12 bits one has 2^12 = 4096 states.
const byte pwm_max_value = 255;

// Since we won't provide the number of steps, this variable
// will be defined after the object calculates them.
byte steps_count;

ExponentMap<byte> e(pwm_max_value);

void setup() {
  Serial.begin(250000);
  Serial.print("ExponentMap v."); Serial.print(EXPONENTMAP_VERSION);
  Serial.println(" linear_output example.");

  steps_count = e.stepsCount();
}

void loop() {
  // The first two loops fade in-out the LED assigning
  // linearly growing values (1, 2, 3, 4, 5...) to the PWM.
  for (int i = 0; i <= pwm_max_value; i++) {
    analogWrite(led_pin, i);
    delay(5);
  }
  for (int i = pwm_max_value; i >= 0; i--) {
    analogWrite(led_pin, i);
    delay(5);
  }

  // The second two loops fade in-out the LED assigning
  // exponentially growing values to the PWM.
  for (int i = 0; i <= steps_count; i++) {
    // Here we use the function operator on the object to get
    // the step's corresponding value. Using e.stepToValue(i)
    // is equivalent.
    analogWrite(led_pin, e(i));
    // Different delays are used because the number of steps is lower.
    delay(75);
  }
  for (int i = steps_count; i >= 0; i--) {
    analogWrite(led_pin, e(i));
    delay(75);
  }
}


ExponentMap
===========

*Generates an exponentially growing array based on provided number of steps and maximum value.*

[![Build Status](https://travis-ci.org/VasilKalchev/ExponentMap.svg?branch=master)](https://travis-ci.org/VasilKalchev/ExponentMap)
[![release](https://img.shields.io/badge/release-1.0.0-yellow.svg)](https://github.com/VasilKalchev/ExponentMap/releases)
[![documentation](https://img.shields.io/badge/docs-doxygen-green.svg)](https://vasilkalchev.github.io/ExponentMap/doc/Doxygen/html/index.html)
[![license](https://img.shields.io/github/license/mashape/apistatus.svg?maxAge=2592000)](https://opensource.org/licenses/mit-license.php)

**The generated array can be used to control a PWM output perceived by humans (brigthness, sound...). Changing the power of an output device exponentially counteracts the logarithmic nature of the human perception. *The equation for generating the array is taken from [Diarmuid][diarmuid]***


Resources
---------
 - [Examples][examples]
 - [API reference][doxygen classes]
 - [Latest release][latest release]

Features
--------
 - Configurable number of steps and max value.
 - Automatic calculation of optimum number of steps.

How it works
------------
The first two plots demonstrate that a linearly changing output in time is perceived by a person logarithmically. And the second two plots demonstrate that an exponentially chaning output in time is perceived by a person linearly.
![plots](https://raw.githubusercontent.com/VasilKalchev/ExponentMap/master/doc/Images/plots.png)

Example "exponent map":

| Step   | Value  |
| ------ |--------|
| 0      | 0      |
| 1      | 1      |
| 2      | 2      |
| 3      | 3      |
| 4      | 5      |
| 5      | 9      |
| 6      | 15     |
| 7      | 25     |
| 8      | 39     |
| 9      | 63     |
| 10     | 101    |
| 11     | 160    |
| 12     | 255    |


### API
The library consists of a single class **ExponentMap**, which generates the array based on the parameters it was constructed with. The first parameter is the desired number of steps and the second is the maximum value. If the number of steps is not specified, the optimal number of steps will be calculated automatically.
```c++
ExponentMap<byte> exp1(12, 255);
ExponentMap<byte> exp2(255);
```


To get the value for a given step use the function operator. Alternatively the method `stepToValue(step)` can be used.
```c++
analogWrite(led, exp1(4));
analogWrite(led, exp1.stepToValue(4));
```


### Basic example
```c++
#include "ExponentMap.h"

...

const byte led_pin = 3;
const byte pwm_max_value = 255;
byte steps_count = 12
byte current_step = 0;

ExponentMap<byte> e(steps_count, pwm_max_value);

...

void loop() {
  ...
  if (up_button == true) {
    analogWrite(led_pin, e(++current_step));
  }
  if (down_button == true) {
    analogWrite(led_pin, e(--current_step));
  }
  ...
}
```


License
-------
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

[doxygen]: https://vasilkalchev.github.io/ExponentMap/doc/Doxygen/html/index.html
[examples]: https://github.com/VasilKalchev/ExponentMap/tree/master/examples
[doxygen classes]: https://vasilkalchev.github.io/ExponentMap/doc/Doxygen/html/annotated.html
[latest release]: https://github.com/VasilKalchev/ExponentMap/releases/latest
[diarmuid]: https://diarmuid.ie/blog/pwm-exponential-led-fading-on-arduino-or-other-platforms/
[wiki]: 404

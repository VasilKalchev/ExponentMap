/*
 * ExponentMap library - object_construction.ino
 *
 * This example demonstrates how to construct an object
 * and print the generated array.
 *
 * Created February 25, 2017
 * by Vasil Kalchev
 *
 * https://github.com/VasilKalchev/ExponentMap
 *
 */

#include <ExponentMap.h>

/*
 * The class's constructor takes the number of steps required
 * and the maximum value. If the number of steps is not
 * specified, the optimal number of steps is calculated
 * automatically. The template parameter specifies the data
 * type for the array holding the function. If it is not
 * specified, it will default to 32 bits unsigned integer.
 */
// This object will have 20 steps with a maximum value of 255.
// The array will be of data type "byte".
ExponentMap<byte> exp1(20, 255);

// This object will calculate the optimum number of steps
// automatically. In case of 255 maximum value, it will be 12.
ExponentMap<byte> exp2(255);

// This object will have bigger number of optimal steps.
ExponentMap<> exp3(4096);

void setup() {
  Serial.begin(250000);
  Serial.print("ExponentMap v."); Serial.print(EXPONENTMAP_VERSION);
  Serial.println(" object_construction example.");

  Serial.println("\nObject 'exp1':");
  /*
   * The method "stepsCount()" returns the number of steps. It is useful
   * when the number of steps is not specified in the constructor.
   */
  Serial.print("Specified number of steps: "); Serial.println(exp1.stepsCount());
  /*
   * The method "stepsRepeat()" returns "true" if the specified number of steps
   * produces a function with repeating values. To avoid that, decrease the number
   * of steps or increase the maximum value.
   */
  exp1.stepsRepeat() ? Serial.println("There are repeating values. Decrease the number of steps.") : 1;
  /*
   * The method "printTable()" prints a table with all the steps and their
   * corresponding value.
   */
  exp1.printTable();
  /*
   * The method "printCode()" prints the code for creating the generated array
   * directly. This is useful if you want to get rid of the class.
   */
  exp1.printCode();

  Serial.println("\nObject 'exp2':");
  Serial.print("Calculated number of steps: "); Serial.println(exp2.stepsCount());
  exp2.stepsRepeat() ? Serial.println("There are repeating values. Decrease the number of steps.") : 1;
  exp2.printTable();
  exp2.printCode();

  Serial.println("\nObject 'exp3':");
  Serial.print("Calculated number of steps: "); Serial.println(exp3.stepsCount());
  exp3.stepsRepeat() ? Serial.println("There are repeating values. Decrease the number of steps.") : 1;
  exp3.printTable();
  exp3.printCode();
}

void loop() {}


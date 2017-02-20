#define EXPONENTMAP_DEBUGGING true
#include "ExponentMap.h"

void setup() {
  Serial.begin(9600);
  ExponentMap<uint8_t> e2(12, 255);
  ExponentMap<uint16_t> e3(12, 255);
  ExponentMap<uint32_t> e4(40, 432423);
  ExponentMap<uint32_t> e5(70, 3000);
  //  for (auto i = 0; i <= 20; i++) {
  //        Serial.print("Step "); Serial.print(i); Serial.print(": "); Serial.println(eMap(i));
  //  }
  int a = 0;
#ifdef ARDUINO_ARCH_AVR
  Serial.println((int)&a, OCT);
#endif
  e2.printTable();
  e2.printCode();
  Serial.println(e2.stepsCount());
  if (e2.stepsRepeat()) {Serial.println("e2 steps repeat");}
  e3.printTable();
  e3.printCode();
  if (e3.stepsRepeat()) {Serial.println("e3 steps repeat");}
  e4.printTable();
  e4.printCode();
  if (e4.stepsRepeat()) {Serial.println("e4 steps repeat");}
  e5.printTable();
  e5.printCode();
  if (e5.stepsRepeat()) {Serial.println("e5 steps repeat");}
}

void loop() {
}


byte led = 3;

const uint16_t testsCount = 50;
uint8_t tests[testsCount] = {0};
uint8_t userInput[testsCount] = {0};

void setup() {
  Serial.begin(250000);
  pinMode(led, OUTPUT);
  randomSeed(analogRead(A3));
  Serial.print("Human perception of birghtness test ("); Serial.print(testsCount); Serial.println(" iterations).\n");
  Serial.println("Type the bightness you think the LED is set to (from 0 to 10).");

  for (uint16_t i = 0; i < testsCount; i++) {
    Serial.print("Iteration "); Serial.print(i); Serial.print(": input brightness...");
    tests[i] = random(256);
    analogWrite(led, tests[i]);
    while (userInput[i] == 0) {
      while (Serial.available() > 0) {
        String userValue_str = Serial.readStringUntil('\0');
        userInput[i] = userValue_str.toInt();
      }
    }
    Serial.println(" - done");
    for (uint8_t b = 0; b < 2; b++ ) {
      analogWrite(led, 0);
      delay(75);
      analogWrite(led, 255);
      delay(75);
    }
  }
  analogWrite(led, 0);

  for (uint16_t i = 0; i < testsCount; i++) {
    Serial.print(tests[i]); Serial.print("\t"); Serial.println(userInput[i]);
  }
}

void loop() {}

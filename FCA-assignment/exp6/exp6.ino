
#include <Arduino.h>
#include <stdio.h>

volatile int integerNumber = 12345;
volatile float floatNumber = 123.456f;

char integerBuffer[32];
char floatBuffer[32];

const int TEST_COUNT = 1000;
const int SERIAL_COUNT = 100;
const float CPU_FREQ_MHZ = 240.0;

// Replace 1.0 with the average addition cost measured in Experiment 1.
const float ADD_CYCLES_PER_OPERATION = 1.0;

void setup() {
  Serial.begin(115200);
  delay(1500);

  Serial.println("EXPERIMENT 6: THE HIDDEN COST OF PRINTING");
  Serial.println("----------------------------------------");

  // PART 1: Measure number-to-text conversion only
  uint32_t startInt = ESP.getCycleCount();

  for (int i = 0; i < TEST_COUNT; i++) {
    snprintf(integerBuffer, sizeof(integerBuffer),
             "%d", (int)integerNumber);
  }

  uint32_t integerCycles = ESP.getCycleCount() - startInt;

  uint32_t startFloat = ESP.getCycleCount();

  for (int i = 0; i < TEST_COUNT; i++) {
    snprintf(floatBuffer, sizeof(floatBuffer),
             "%.3f", (double)floatNumber);
  }

  uint32_t floatCycles = ESP.getCycleCount() - startFloat;

  Serial.println();
  Serial.println("1. CONVERSION RESULTS");
  Serial.print("Integer text: ");
  Serial.println(integerBuffer);
  Serial.print("Float text: ");
  Serial.println(floatBuffer);

  Serial.print("Integer total cycles: ");
  Serial.println(integerCycles);
  Serial.print("Integer average cycles: ");
  Serial.println((float)integerCycles / TEST_COUNT, 2);

  Serial.print("Float total cycles: ");
  Serial.println(floatCycles);
  Serial.print("Float average cycles: ");
  Serial.println((float)floatCycles / TEST_COUNT, 2);

  // PART 2: Calculate and measure one character's transmission time
  // 8 data bits + 1 start bit + 1 stop bit = 10 bits per character
  const float THEORETICAL_CHAR_US = (10.0 / 115200.0) * 1000000.0;

  Serial.println();
  Serial.println("2. SERIAL TRANSMISSION");
  Serial.print("Calculated time per character (us): ");
  Serial.println(THEORETICAL_CHAR_US, 2);

  Serial.flush();
  uint32_t startSend = micros();

  for (int i = 0; i < SERIAL_COUNT; i++) {
    Serial.write('A');
  }

  Serial.flush();
  uint32_t totalSendTime = micros() - startSend;

  float measuredCharUs = (float)totalSendTime / SERIAL_COUNT;

  Serial.println();
  Serial.print("Characters sent: ");
  Serial.println(SERIAL_COUNT);
  Serial.print("Total transmission time (us): ");
  Serial.println(totalSendTime);
  Serial.print("Measured average per character (us): ");
  Serial.println(measuredCharUs, 2);

  // PART 3: Compare decimal, hexadecimal and binary printing
  Serial.println();
  Serial.println("3. NUMBER REPRESENTATION PRINTING");
  Serial.println("Each format is printed 100 times; transmission is included.");

  Serial.flush();
  uint32_t startDecimal = micros();

  for (int i = 0; i < SERIAL_COUNT; i++) {
    Serial.print(255);
  }

  Serial.flush();
  uint32_t decimalTime = micros() - startDecimal;

  Serial.println();

  Serial.flush();
  uint32_t startHex = micros();

  for (int i = 0; i < SERIAL_COUNT; i++) {
    Serial.print(255, HEX);
  }

  Serial.flush();
  uint32_t hexTime = micros() - startHex;

  Serial.println();

  Serial.flush();
  uint32_t startBinary = micros();

  for (int i = 0; i < SERIAL_COUNT; i++) {
    Serial.print(255, BIN);
  }

  Serial.flush();
  uint32_t binaryTime = micros() - startBinary;

  Serial.println();

  Serial.print("Decimal output (255), total us: ");
  Serial.println(decimalTime);
  Serial.print("Hex output (FF), total us: ");
  Serial.println(hexTime);
  Serial.print("Binary output (11111111), total us: ");
  Serial.println(binaryTime);

  // PART 4: CPU cycles available during one character's transmission
  float cyclesDuringCharacter = CPU_FREQ_MHZ * measuredCharUs;

  Serial.println();
  Serial.println("4. CPU ADDITION COMPARISON");
  Serial.print("CPU cycles during one character: ");
  Serial.println(cyclesDuringCharacter, 2);

  Serial.print("Assumed cycles per addition: ");
  Serial.println(ADD_CYCLES_PER_OPERATION, 2);

  if (ADD_CYCLES_PER_OPERATION > 0) {
    Serial.print("Estimated additions possible: ");
    Serial.println(cyclesDuringCharacter / ADD_CYCLES_PER_OPERATION, 2);
  }

  Serial.println();
  Serial.println("Experiment completed.");
}

void loop() {
}
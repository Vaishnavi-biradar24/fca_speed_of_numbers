
#include <Arduino.h>
#include <stdint.h>
#include <math.h>

#define SCALE 65536LL
#define N 10000

volatile int32_t fixedA = 98304;    // 1.5
volatile int32_t fixedB = 131072;   // 2.0
volatile float floatA = 1.5f;
volatile float floatB = 2.0f;

volatile int32_t fixedResult;
volatile float floatResult;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("EXPERIMENT 5: FIXED-POINT VS FLOAT");
  Serial.println("--------------------------------");

  Serial.println("Q16.16 REPRESENTATION");
  Serial.print("3.75 encoded = ");
  Serial.println((int32_t)(3.75 * SCALE));
  Serial.print("-1.25 encoded = ");
  Serial.println((int32_t)(-1.25 * SCALE));

  Serial.print("Minimum Q16.16 = ");
  Serial.println((double)INT32_MIN / SCALE, 5);
  Serial.print("Maximum Q16.16 = ");
  Serial.println((double)INT32_MAX / SCALE, 5);

  Serial.println("\nADDITION TEST");

  uint32_t start = ESP.getCycleCount();
  for (int i = 0; i < N; i++) {
    fixedResult = (int32_t)((int64_t)fixedA + fixedB);
  }
  uint32_t fixedAddCycles = ESP.getCycleCount() - start;

  start = ESP.getCycleCount();
  for (int i = 0; i < N; i++) {
    floatResult = floatA + floatB;
  }
  uint32_t floatAddCycles = ESP.getCycleCount() - start;

  Serial.print("Fixed addition = ");
  Serial.println((float)fixedResult / SCALE, 4);
  Serial.print("Float addition = ");
  Serial.println(floatResult, 4);
  Serial.print("Fixed addition total cycles = ");
  Serial.println(fixedAddCycles);
  Serial.print("Float addition total cycles = ");
  Serial.println(floatAddCycles);

  Serial.println("\nMULTIPLICATION TEST");

  start = ESP.getCycleCount();
  for (int i = 0; i < N; i++) {
    int64_t product = (int64_t)fixedA * fixedB;
    fixedResult = (int32_t)(product / SCALE);
  }
  uint32_t fixedMulCycles = ESP.getCycleCount() - start;

  start = ESP.getCycleCount();
  for (int i = 0; i < N; i++) {
    floatResult = floatA * floatB;
  }
  uint32_t floatMulCycles = ESP.getCycleCount() - start;

  Serial.print("Fixed multiplication = ");
  Serial.println((float)fixedResult / SCALE, 4);
  Serial.print("Float multiplication = ");
  Serial.println(floatResult, 4);
  Serial.print("Fixed multiplication total cycles = ");
  Serial.println(fixedMulCycles);
  Serial.print("Float multiplication total cycles = ");
  Serial.println(floatMulCycles);

  Serial.println("\nDIVISION TEST");

  start = ESP.getCycleCount();
  for (int i = 0; i < N; i++) {
    fixedResult = (int32_t)(((int64_t)fixedA * SCALE) / fixedB);
  }
  uint32_t fixedDivCycles = ESP.getCycleCount() - start;

  start = ESP.getCycleCount();
  for (int i = 0; i < N; i++) {
    floatResult = floatA / floatB;
  }
  uint32_t floatDivCycles = ESP.getCycleCount() - start;

  Serial.print("Fixed division = ");
  Serial.println((float)fixedResult / SCALE, 4);
  Serial.print("Float division = ");
  Serial.println(floatResult, 4);
  Serial.print("Fixed division total cycles = ");
  Serial.println(fixedDivCycles);
  Serial.print("Float division total cycles = ");
  Serial.println(floatDivCycles);

  Serial.println("\nACCURACY TEST");
  double reference = 1.5 / 2.0;
  double fixedValue = (double)fixedResult / SCALE;
  double floatValue = floatResult;

  Serial.print("Double reference = ");
  Serial.println(reference, 8);
  Serial.print("Fixed absolute error = ");
  Serial.println(fabs(reference - fixedValue), 8);
  Serial.print("Float absolute error = ");
  Serial.println(fabs(reference - floatValue), 8);

  Serial.println("\nExperiment completed.");
}

void loop() {
}
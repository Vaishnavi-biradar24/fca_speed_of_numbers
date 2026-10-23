
/*
  Experiment 4: Are Bit Tricks Really Faster?

  Tests:
  1. x / 8 versus x >> 3
  2. x % 16 versus x & 15
  3. Constant divisor versus volatile divisor
  4. -9 / 8 versus -9 >> 3

*/

#include <Arduino.h>

const uint32_t N = 1000;

volatile int32_t x = 123456;
volatile int32_t result = 0;
volatile int32_t divisor = 8;

// Baseline for a particular test.
// Uses the same loop structure and volatile result store.
uint32_t measureBaseline() {
  uint32_t start = ESP.getCycleCount();

  for (uint32_t i = 0; i < N; i++) {
    int32_t value = x;
    result = value;
  }

  uint32_t end = ESP.getCycleCount();
  return (uint32_t)(end - start);
}

// Run one test and report raw and baseline-adjusted timing.
template <typename Operation>
void runTest(const char *name, Operation operation) {
  uint32_t baseline = measureBaseline();

  // Warm up before timing.
  for (uint32_t i = 0; i < 10; i++) {
    result = operation();
  }

  uint32_t start = ESP.getCycleCount();

  for (uint32_t i = 0; i < N; i++) {
    result = operation();
  }

  uint32_t end = ESP.getCycleCount();
  uint32_t rawCycles = (uint32_t)(end - start);
  int32_t finalResult = result;

  float adjustedCycles =
      ((float)rawCycles - (float)baseline) / N;

  Serial.print(name);
  Serial.print(" | Raw cycles: ");
  Serial.print(rawCycles);
  Serial.print(" | Baseline: ");
  Serial.print(baseline);
  Serial.print(" | Estimated cycles/op: ");
  Serial.print(adjustedCycles, 3);
  Serial.print(" | Result: ");
  Serial.println(finalResult);
}

void setup() {
  Serial.begin(115200);
  delay(1500);

  Serial.println();
  Serial.println("FCA Experiment 4: Bit Tricks");
  Serial.print("CPU frequency: ");
  Serial.print(getCpuFrequencyMhz());
  Serial.println(" MHz");

  Serial.print("Repetitions: ");
  Serial.println(N);

  Serial.println();
  Serial.println("--- Test A: Division versus Right Shift ---");

  runTest("x / 8", []() -> int32_t {
    return x / 8;
  });

  runTest("x >> 3", []() -> int32_t {
    return x >> 3;
  });

  Serial.println();
  Serial.println("--- Test B: Remainder versus Bit Mask ---");

  runTest("x % 16", []() -> int32_t {
    return x % 16;
  });

  runTest("x & 15", []() -> int32_t {
    return x & 15;
  });

  Serial.println();
  Serial.println("--- Test C: Constant versus Volatile Divisor ---");

  runTest("x / 8 (constant)", []() -> int32_t {
    return x / 8;
  });

  runTest("x / divisor (volatile)", []() -> int32_t {
    return x / divisor;
  });

  Serial.println();
  Serial.println("--- Test D: Negative Integer Behaviour ---");

  int32_t negativeDivision = -9 / 8;
  int32_t negativeShift = -9 >> 3;

  Serial.print("-9 / 8 = ");
  Serial.println(negativeDivision);

  Serial.print("-9 >> 3 = ");
  Serial.println(negativeShift);

  Serial.println(
    "Negative signed right-shift behaviour is implementation-defined."
  );

  Serial.println();

}

void loop() {
  // Experiment runs once in setup().
}
/*
  FCA Assignment: The Speed of Numbers
  Experiment 3: Float versus Double

  Tests:
  1. Float and double addition
  2. Float and double multiplication
  3. Float and double division
  4. Float and double square root
  5. x * 0.5 versus x * 0.5f
  6. Fastest float operation versus int32 addition

  Set CPU frequency to 240 MHz if supported.
  Serial Monitor: 115200 baud.
  Record predictions before running the tests.
*/

#include <Arduino.h>
#include <math.h>

const uint32_t N = 1000;

// Volatile helps prevent some compiler optimizations.
volatile float fa = 12.25f, fb = 3.5f, fr = 0;
volatile double da = 12.25, db = 3.5, dr = 0;
volatile int32_t ia = 12345, ib = 678, ir = 0;

// Print the timing result after subtracting the baseline.
void report(const char *name, uint32_t raw,
            uint32_t baseline) {
  float adjusted = ((float)raw - baseline) / N;

  Serial.print(name);
  Serial.print(" | Raw cycles: ");
  Serial.print(raw);
  Serial.print(" | Baseline: ");
  Serial.print(baseline);
  Serial.print(" | Estimated cycles/op: ");
  Serial.println(adjusted, 3);
}

// Baseline for floating-point tests.
// This measures loop and variable-access overhead approximately.
uint32_t floatBaseline() {
  uint32_t t0 = ESP.getCycleCount();

  for (uint32_t i = 0; i < N; i++) {
    float x = fa;
    float y = fb;
    fr = x;
    (void)y;
  }

  return ESP.getCycleCount() - t0;
}

// Baseline for double-precision tests.
uint32_t doubleBaseline() {
  uint32_t t0 = ESP.getCycleCount();

  for (uint32_t i = 0; i < N; i++) {
    double x = da;
    double y = db;
    dr = x;
    (void)y;
  }

  return ESP.getCycleCount() - t0;
}

// EXPERIMENT 3A: Float addition
void testFloatAdd() {
  uint32_t base = floatBaseline();
  uint32_t t0 = ESP.getCycleCount();

  for (uint32_t i = 0; i < N; i++)
    fr = (float)fa + (float)fb;

  uint32_t raw = ESP.getCycleCount() - t0;
  report("Float addition", raw, base);
  Serial.println((float)fr, 6);
}

// EXPERIMENT 3B: Double addition
void testDoubleAdd() {
  uint32_t base = doubleBaseline();
  uint32_t t0 = ESP.getCycleCount();

  for (uint32_t i = 0; i < N; i++)
    dr = (double)da + (double)db;

  uint32_t raw = ESP.getCycleCount() - t0;
  report("Double addition", raw, base);
  Serial.println((double)dr, 8);
}

// EXPERIMENT 3C: Float multiplication
void testFloatMultiply() {
  uint32_t base = floatBaseline();
  uint32_t t0 = ESP.getCycleCount();

  for (uint32_t i = 0; i < N; i++)
    fr = (float)fa * (float)fb;

  uint32_t raw = ESP.getCycleCount() - t0;
  report("Float multiplication", raw, base);
  Serial.println((float)fr, 6);
}

// EXPERIMENT 3D: Double multiplication
void testDoubleMultiply() {
  uint32_t base = doubleBaseline();
  uint32_t t0 = ESP.getCycleCount();

  for (uint32_t i = 0; i < N; i++)
    dr = (double)da * (double)db;

  uint32_t raw = ESP.getCycleCount() - t0;
  report("Double multiplication", raw, base);
  Serial.println((double)dr, 8);
}

// EXPERIMENT 3E: Float division
void testFloatDivide() {
  uint32_t base = floatBaseline();
  uint32_t t0 = ESP.getCycleCount();

  for (uint32_t i = 0; i < N; i++)
    fr = (float)fa / (float)fb;

  uint32_t raw = ESP.getCycleCount() - t0;
  report("Float division", raw, base);
  Serial.println((float)fr, 6);
}

// EXPERIMENT 3F: Double division
void testDoubleDivide() {
  uint32_t base = doubleBaseline();
  uint32_t t0 = ESP.getCycleCount();

  for (uint32_t i = 0; i < N; i++)
    dr = (double)da / (double)db;

  uint32_t raw = ESP.getCycleCount() - t0;
  report("Double division", raw, base);
  Serial.println((double)dr, 8);
}

// EXPERIMENT 3G: Float square root; sqrtf returns float.
void testFloatSqrt() {
  uint32_t base = floatBaseline();
  uint32_t t0 = ESP.getCycleCount();

  for (uint32_t i = 0; i < N; i++)
    fr = sqrtf((float)fa);

  uint32_t raw = ESP.getCycleCount() - t0;
  report("Float sqrtf", raw, base);
  Serial.println((float)fr, 6);
}

// EXPERIMENT 3H: Double square root; sqrt returns double.
void testDoubleSqrt() {
  uint32_t base = doubleBaseline();
  uint32_t t0 = ESP.getCycleCount();

  for (uint32_t i = 0; i < N; i++)
    dr = sqrt((double)da);

  uint32_t raw = ESP.getCycleCount() - t0;
  report("Double sqrt", raw, base);
  Serial.println((double)dr, 8);
}

// EXPERIMENT 3I: Compare 0.5f with 0.5.
// 0.5f is a float literal; 0.5 is a double literal.
void testLiteralSuffix() {
  uint32_t t0 = ESP.getCycleCount();

  for (uint32_t i = 0; i < N; i++)
    fr = (float)fa * 0.5f;

  uint32_t floatLiteralCycles = ESP.getCycleCount() - t0;

  t0 = ESP.getCycleCount();

  for (uint32_t i = 0; i < N; i++)
    fr = (float)fa * 0.5;

  uint32_t doubleLiteralCycles = ESP.getCycleCount() - t0;

  Serial.print("x * 0.5f raw cycles: ");
  Serial.println(floatLiteralCycles);

  Serial.print("x * 0.5 raw cycles: ");
  Serial.println(doubleLiteralCycles);

  Serial.println("These raw timings include loop overhead.");
}

// EXPERIMENT 3J: Compare with 32-bit integer addition.
void testInt32Add() {
  uint32_t t0 = ESP.getCycleCount();

  for (uint32_t i = 0; i < N; i++)
    ir = (int32_t)ia + (int32_t)ib;

  uint32_t raw = ESP.getCycleCount() - t0;

  Serial.print("Int32 addition raw cycles: ");
  Serial.println(raw);
  Serial.print("Result: ");
  Serial.println((long)ir);
}

void setup() {
  Serial.begin(115200);
  delay(1500);

  Serial.println("FCA Experiment 3: Float vs Double");
  Serial.print("CPU frequency: ");
  Serial.println(getCpuFrequencyMhz());
  Serial.print("Repetitions: ");
  Serial.println(N);

  testFloatAdd();
  testDoubleAdd();

  testFloatMultiply();
  testDoubleMultiply();

  testFloatDivide();
  testDoubleDivide();

  testFloatSqrt();
  testDoubleSqrt();

  testLiteralSuffix();
  testInt32Add();


}

void loop() {
  // Run the experiment only once after startup.
}
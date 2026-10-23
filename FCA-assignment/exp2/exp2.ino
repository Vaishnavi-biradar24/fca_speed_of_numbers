
#include <Arduino.h>
#include <stdint.h>

#define ITERATIONS 10000
#define RUNS 5

volatile int8_t a8 = 10, b8 = 2;
volatile int16_t a16 = 1000, b16 = 5;
volatile int32_t a32 = 100000, b32 = 10;
volatile int64_t a64 = 1000000000LL, b64 = 10;
volatile uint32_t au32 = 100000, bu32 = 10;

volatile int64_t result = 0;

typedef int64_t (*Operation)();

uint32_t measureBaseline() {
  uint32_t start = ESP.getCycleCount();

  for (int i = 0; i < ITERATIONS; i++) {
    int64_t left = a32;
    int64_t right = b32;
    (void)right;
    result = left;
  }

  return ESP.getCycleCount() - start;
}

uint32_t measureOperation(Operation operation) {
  // Warm-up before measuring
  for (int i = 0; i < 100; i++) {
    result = operation();
  }

  uint32_t start = ESP.getCycleCount();

  for (int i = 0; i < ITERATIONS; i++) {
    result = operation();
  }

  return ESP.getCycleCount() - start;
}

void sortValues(float values[]) {
  for (int i = 0; i < RUNS - 1; i++) {
    for (int j = i + 1; j < RUNS; j++) {
      if (values[j] < values[i]) {
        float temp = values[i];
        values[i] = values[j];
        values[j] = temp;
      }
    }
  }
}

void runTest(const char *name, Operation operation) {
  float adjusted[RUNS];
  uint32_t raw[RUNS];
  uint32_t base[RUNS];

  for (int r = 0; r < RUNS; r++) {
    base[r] = measureBaseline();
    raw[r] = measureOperation(operation);

    adjusted[r] =
        ((float)raw[r] - (float)base[r]) / ITERATIONS;
  }

  sortValues(adjusted);

  Serial.print(name);
  Serial.print(" | Median estimated cycles/op: ");
  Serial.print(adjusted[RUNS / 2], 3);

  Serial.print(" | Last raw cycles: ");
  Serial.print(raw[RUNS - 1]);

  Serial.print(" | Last baseline: ");
  Serial.println(base[RUNS - 1]);

  Serial.print("  Result: ");
  Serial.println((long long)result);
}

int64_t add8()  { return (int8_t)(a8 + b8); }
int64_t add16() { return (int16_t)(a16 + b16); }
int64_t add32() { return a32 + b32; }
int64_t add64() { return a64 + b64; }

int64_t mul8()  { return (int8_t)(a8 * b8); }
int64_t mul16() { return (int16_t)(a16 * b16); }
int64_t mul32() { return a32 * b32; }
int64_t mul64() { return a64 * b64; }

int64_t div8()  { return (int8_t)(a8 / b8); }
int64_t div16() { return (int16_t)(a16 / b16); }
int64_t div32() { return a32 / b32; }
int64_t div64() { return a64 / b64; }
int64_t divU32() { return au32 / bu32; }

void setup() {
  Serial.begin(115200);
  delay(1500);

  Serial.println("FCA EXPERIMENT 2");
  Serial.println("IS SMALLER FASTER?");
  Serial.println("==============================");

  Serial.print("CPU Frequency: ");
  Serial.print(getCpuFrequencyMhz());
  Serial.println(" MHz");

  Serial.print("Iterations per run: ");
  Serial.println(ITERATIONS);
  Serial.print("Runs per test: ");
  Serial.println(RUNS);
  Serial.println("Reported value: median estimated cycles/op");
  Serial.println("Baseline subtraction is approximate.");

  Serial.println();
  Serial.println("ADDITION TEST");
  runTest("8-bit addition", add8);
  runTest("16-bit addition", add16);
  runTest("32-bit addition", add32);
  runTest("64-bit addition", add64);

  Serial.println();
  Serial.println("MULTIPLICATION TEST");
  runTest("8-bit multiplication", mul8);
  runTest("16-bit multiplication", mul16);
  runTest("32-bit multiplication", mul32);
  runTest("64-bit multiplication", mul64);

  Serial.println();
  Serial.println("DIVISION TEST");
  runTest("8-bit signed division", div8);
  runTest("16-bit signed division", div16);
  runTest("32-bit signed division", div32);
  runTest("64-bit signed division", div64);
  runTest("32-bit unsigned division", divU32);

  Serial.println();
  Serial.println("Experiment completed.");

}

void loop() {
}
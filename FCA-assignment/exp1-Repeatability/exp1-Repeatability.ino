
#include <stdint.h>

const int N = 1000;

volatile uint32_t a = 10;
volatile uint32_t b = 20;
volatile uint32_t result = 0;

// Measure baseline loop
uint32_t measureBaseline(int n) {
  uint32_t start = ESP.getCycleCount();

  for (int i = 0; i < n; i++) {
    asm volatile("" ::: "memory");
  }

  uint32_t end = ESP.getCycleCount();
  return end - start;
}

// Measure addition loop
uint32_t measureAddition(int n) {
  uint32_t start = ESP.getCycleCount();

  for (int i = 0; i < n; i++) {
    result = a + b;
  }

  uint32_t end = ESP.getCycleCount();
  return end - start;
}

// Run five trials
void runTest(int n) {
  Serial.println();
  Serial.print("Operations: ");
  Serial.println(n);

  for (int trial = 1; trial <= 5; trial++) {
    uint32_t baseline = measureBaseline(n);
    uint32_t total = measureAddition(n);

    Serial.print("Trial ");
    Serial.print(trial);

    Serial.print(" | Baseline: ");
    Serial.print(baseline);

    Serial.print(" | Total: ");
    Serial.print(total);

    Serial.print(" | Estimated cycles/op: ");
    Serial.println((float)(total - baseline) / n, 3);
  }

  Serial.print("Final result: ");
  Serial.println(result);
}

void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.println("FCA Experiment 1 - Repeatability");
  Serial.print("CPU Frequency: ");
  Serial.print(ESP.getCpuFreqMHz());
  Serial.println(" MHz");

  runTest(10);
  runTest(1000);

  Serial.println();
  Serial.println("Test complete. Reset board to repeat.");
}

void loop() {
}
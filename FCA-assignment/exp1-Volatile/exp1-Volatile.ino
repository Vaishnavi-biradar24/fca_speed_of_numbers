
#include <stdint.h>

const int N = 1000;

// Test A: volatile variables
volatile uint32_t va = 10;
volatile uint32_t vb = 20;
volatile uint32_t vr = 0;

// Test B: ordinary variables
uint32_t a = 10;
uint32_t b = 20;
uint32_t r = 0;

void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.println("FCA Experiment 1 - Volatile Test");
  Serial.println("CPU Frequency:");
  Serial.println(ESP.getCpuFreqMHz());

  uint32_t start, end;

  // Test A: volatile addition
  start = ESP.getCycleCount();
  for (int i = 0; i < N; i++) {
    vr = va + vb;
  }
  end = ESP.getCycleCount();

  Serial.print("Volatile cycles: ");
  Serial.println(end - start);
  Serial.print("Volatile result: ");
  Serial.println(vr);

  // Test B: ordinary addition
  start = ESP.getCycleCount();
  for (int i = 0; i < N; i++) {
    r = a + b;
  }
  end = ESP.getCycleCount();

  Serial.print("Non-volatile cycles: ");
  Serial.println(end - start);
  Serial.print("Non-volatile result: ");
  Serial.println(r);
}

void loop() {
}
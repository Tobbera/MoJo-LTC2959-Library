#include <Wire.h>
#include "LTC2959.h"

// Create an instance of the LTC2959 class
LTC2959 batteryMonitor;
// if your rsene is not 250mohms, ucomment he line below and  input your rsense value
// a wrong rsense will affect current values
//LTC2959 batteryMonitor(xxx);

void setup() {
  Serial.begin(115200);
  delay(100);  // Wait for Serial

  Serial.println("Initializing LTC2959...");
  Wire.begin(16, 17);
  // Wire.setSDA(PB9);
  // Wire.setSCL(PB8);
  if (!batteryMonitor.begin()) {
    Serial.println("LTC2959 not found. Check wiring and I2C address.");
    while (1)
      ;  // Halt
  }

  Serial.println("LTC2959 initialized successfully!");

  // 🟢 Enable Coulomb Counter with LOW deadband (20uV)
  bool counterEnabled = batteryMonitor.enableCounter(LTC2959::LOW_DEADBAND,false);
  if (counterEnabled) {
    Serial.println("Coulomb counter enabled successfully.");
  } else {
    Serial.println("Failed to enable Coulomb counter.");
  }
}

void loop() {
  float voltage = batteryMonitor.readVoltage();
  float temperature = batteryMonitor.readTemperature();
  float current = batteryMonitor.readCurrent();
  float mAh = batteryMonitor.readCharge_mAh();
  char buffer[120];

  snprintf(buffer, sizeof(buffer),
           "Voltage: %7.3f V | Temp: %6.2f °C | Current: %7.3f A | mAh: %13.6f mAh",
           voltage, temperature, current, mAh);

  Serial.println(buffer);

  delay(1000);
}

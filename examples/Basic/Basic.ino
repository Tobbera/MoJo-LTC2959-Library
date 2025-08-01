#include <Wire.h>
#include <LTC2959.h>

// Create an instance of the LTC2959 class
// You can pass a different Rsense value if not using 0.250Ω
LTC2959 batteryMonitor;

void setup() {
  Serial.begin(115200);
  delay(100);  // Give time for Serial to initialize

  Wire.begin();  // Set up I2C using default SDA/SCL

  Serial.println("Initializing LTC2959...");
  if (!batteryMonitor.begin()) {
    Serial.println("LTC2959 not found. Check wiring!");
    while (true); // Halt here if chip is not found
  }

  Serial.println("LTC2959 initialized!");

  // Enable the Coulomb counter with LOW deadband (20 µV)
  if (batteryMonitor.enableCounter(LTC2959::LOW_DEADBAND)) {
    Serial.println("Coulomb counter enabled.");
  } else {
    Serial.println("Failed to enable Coulomb counter.");
  }

  // Optional: Reset accumulated charge on startup
  batteryMonitor.resetChargeCounter();
}

void loop() {
  float voltage     = batteryMonitor.readVoltage();      // in Volts
  float current     = batteryMonitor.readCurrent();      // in Amps
  float temperature = batteryMonitor.readTemperature();  // in °C
  float charge      = batteryMonitor.readCharge_mAh();   // in mAh

  // Display results
  Serial.print("Voltage: ");
  Serial.print(voltage, 3);
  Serial.print(" V | ");

  Serial.print("Current: ");
  Serial.print(current, 3);
  Serial.print(" A | ");

  Serial.print("Temp: ");
  Serial.print(temperature, 2);
  Serial.print(" °C | ");

  Serial.print("Charge: ");
  Serial.print(charge, 3);
  Serial.println(" mAh");

  delay(1000);
}

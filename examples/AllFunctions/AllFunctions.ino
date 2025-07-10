#include <Wire.h>
#include <LTC2959.h>

// Create an instance of the LTC2959 class
// The default Rsense is 0.250 ohms. You can pass a different value if needed
LTC2959 batteryMonitor;

void setup() {
  Serial.begin(115200);
  delay(100); // Give Serial time to initialize

  // Initialize I2C (SDA/SCL will use default pins unless manually set)
  //Wire.begin();

  Wire.begin(16,17); //for esp32. I usually use 16 as SDA or 17 as SCL
  
  // For STM32
    // Wire.setSDA(PB9);
  // Wire.setSCL(PB8);

  Serial.println("Initializing LTC2959...");

  // Check if the LTC2959 is responding on I2C
  if (!batteryMonitor.begin()) {
    Serial.println("LTC2959 not detected. Please check wiring and address.");
    while (true); // Halt execution
  }

  Serial.println("LTC2959 initialized successfully!");

  //  Enable the Coulomb counter with your preferred deadband setting
  // Available options: NO_DEADBAND, LOW_DEADBAND, MEDIUM_DEADBAND, HIGH_DEADBAND
  if (batteryMonitor.enableCounter(LTC2959::LOW_DEADBAND)) {
    Serial.println("Coulomb counter enabled.");
  } else {
    Serial.println("Failed to enable Coulomb counter.");
  }

  //  Reset the charge accumulator (ACR) to zero
  batteryMonitor.resetChargeCounter();
  Serial.println("Charge counter reset.");
}

void loop() {
  //  Read supply/battery voltage (depends on ADC config set internally)
  float voltage = batteryMonitor.readVoltage(); // Volts

  //  Read internal temperature of the LTC2959
  float temperature = batteryMonitor.readTemperature(); // Celsius

  //  Read instantaneous current using RSENSE
  float current = batteryMonitor.readCurrent(); // Amps

  //  Read total accumulated charge in mAh (since last reset)
  float charge = batteryMonitor.readCharge_mAh(); // mAh

  //  Read the maximum recorded voltage since power-up
  float maxVoltage = batteryMonitor.readMaxVoltage(); // Volts

  //  Read the minimum recorded voltage since power-up
  float minVoltage = batteryMonitor.readMinVoltage(); // Volts

  //  Read the maximum recorded current since power-up
  float maxCurrent = batteryMonitor.readMaxCurrent(); // Amps

  // Read the minimum recorded current since power-up
  float minCurrent = batteryMonitor.readMinCurrent(); // Amps



  // Print all values in a clean, readable format
  Serial.println(F("-----------------------------------------------------"));
  Serial.print(F("Voltage        : ")); Serial.print(voltage, 3); Serial.println(F(" V"));
  Serial.print(F("Temperature    : ")); Serial.print(temperature, 2); Serial.println(F(" °C"));
  Serial.print(F("Current        : ")); Serial.print(current, 3); Serial.println(F(" A"));
  Serial.print(F("Charge (mAh)   : ")); Serial.print(charge, 3); Serial.println(F(" mAh"));
  Serial.print(F("Max Voltage    : ")); Serial.print(maxVoltage, 3); Serial.println(F(" V"));
  Serial.print(F("Min Voltage    : ")); Serial.print(minVoltage, 3); Serial.println(F(" V"));
  Serial.print(F("Max Current    : ")); Serial.print(maxCurrent, 3); Serial.println(F(" A"));
  Serial.print(F("Min Current    : ")); Serial.print(minCurrent, 3); Serial.println(F(" A"));
  Serial.println(F("-----------------------------------------------------"));

  delay(2000); // Wait 2 seconds before next update
}

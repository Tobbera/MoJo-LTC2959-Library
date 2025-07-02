#include "LTC2959.h"

LTC2959::LTC2959(float rSenseOhms) {
  _address = 0x63;
  _adcAddr = 0x01;
  _adcConfig = 0xA4;
  _rSense = rSenseOhms;
  // 0XA4  = 10100100. The third bit from behind control which voltage is read
  // if it 0 as in 10100000 = 0xA0 , it will read the supply voltage
  // if it 1 as in 10100100 = 0xA4 , it will read the battery voltage [ which is voltage at SENSEN pin]
  // refer to datasheet page 12 for more information
}

bool LTC2959::begin() {
  Wire.begin();  // Start I2C if not already running

  // Simple check: try to read a known register (e.g., voltage MSB at 0x0F)
  Wire.beginTransmission(_address);
  Wire.write(0x0F);  // Voltage MSB . Just using it to check if the device will respond
  if (Wire.endTransmission(false) != 0) {
    // Ends the write phase, but sets false to send a repeated start, not a STOP.

    // This is important when you want to follow the write with a read (which we’ll do later).

    // The return value tells us if the transmission succeeded.

    return false;  // Device not responding
  }

  Wire.requestFrom(_address, (uint8_t)1);
  return Wire.available() == 1;  // check if it responded with data
}

float LTC2959::readVoltage() {
  writeRegister(_adcAddr, _adcConfig);
  uint16_t raw = readRegister16(0x0F);  // 0x0F = Voltage MSB
  return 62.6 * raw / 65536.0;
}

float LTC2959::readTemperature() {
  writeRegister(_adcAddr, _adcConfig);
  uint16_t raw = readRegister16(0x23);  // 0x23 = Temperature MSB
  return 825.0 * raw / 65536.0 - 273.15;
}

float LTC2959::readCurrent(){
  writeRegister(_adcAddr, _adcConfig); // tell ADC to convert voltage, current and temp readings
  int16_t raw = readRegister16(0x19);  // 0x19 = Current MSB
  return (97.5e-3 / _rSense) * (raw / 32768.0);
}



int16_t LTC2959::readRegister16(uint8_t reg) {
  Wire.beginTransmission(_address);
  Wire.write(reg);
  Wire.endTransmission(false);

  Wire.requestFrom(_address, (uint8_t)2);
  if (Wire.available() < 2) return 0;

  uint8_t msb = Wire.read();
  uint8_t lsb = Wire.read();

  return (int16_t)(msb << 8 | lsb);
}


void LTC2959::writeRegister(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(_address);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

#ifndef LTC2959_H
#define LTC2959_H

#include <Arduino.h>
#include <Wire.h>

class LTC2959 {
public:
    LTC2959(float rSenseOhms = 0.250); // Constructor

    bool begin(); // Initializes I2C and checks device
    float readVoltage();
    float readTemperature();
    float readCurrent();
    // Add more functions later...

private:
    uint8_t _address;
    uint8_t _adcAddr;
    uint8_t _adcConfig; // decides what data the ADC returns
    float _rSense;
    int16_t readRegister16(uint8_t reg);
    void writeRegister(uint8_t reg, uint8_t value);
};

#endif

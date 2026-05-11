// Name: Seth Motta and Daniel Appleford
// Purpose: Read Accelerometer, Temperature, and Gyroscope data from an MPU-6050
// Help From: https://cdn.sparkfun.com/datasheets/Sensors/Accelerometers/RM-MPU-6000A.pdf


#include <Arduino.h>
#include <Wire.h>

class MPU {
private:
    const int address = 0x68; // MPU-6050 I2C Address
public:
    MPU() = default;

    int16_t AcX, AcY, AcZ, Temp, GyX, GyY, GyZ; // Accelerometer, Temperature, and Gyroscope Data

    void initialize() {
        Wire.begin();
        write(0x6B, 0); // Wake up the MPU
        write(0x1B, 0x00); // Reset Gyroscope
        write(0x1C, 0x00); // Reset Accelerometer
    }

    // Write data to MPU reg at its I2C address
    void write(byte reg, byte data) {
        Wire.beginTransmission(address);
        Wire.write(reg);
        Wire.write(data);
        Wire.endTransmission(true);
    }

    // Read Acceleration, Temperature, and Gyroscope data
    void read() {
        Wire.beginTransmission(address);
        Wire.write(0x3B); // Begins with register 0x3B (ACCEL_XOUT_H)
        Wire.endTransmission(false);
        Wire.requestFrom(address, 14, true); // Request a total of 14 registers

        // Read data from the MPU
        AcX = (Wire.read() << 8) | Wire.read();
        AcY = (Wire.read() << 8) | Wire.read();
        AcZ = (Wire.read() << 8) | Wire.read();
        Temp = (Wire.read() << 8) | Wire.read();
        GyX = (Wire.read() << 8) | Wire.read();
        GyY = (Wire.read() << 8) | Wire.read();
        GyZ = (Wire.read() << 8) | Wire.read();
    }
};
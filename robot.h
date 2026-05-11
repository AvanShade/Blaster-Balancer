#include <Arduino.h>
#include <Wire.h>
#include "motor.h"
#include "mpu.h"
#include "controller.h"

class Robot {
private:
  // Note: If motor runs backward, flip in1 and in2
  Motor leftMotor{ 5, 4, 3 };
  Motor rightMotor{ 6, 7, 8 };
  Controller pidController;
  MPU mpu;

  int16_t mpuData[6];
  float accelerationAngle[2];
  float gyroscopeAngle[2];
  float totalAngle[2];

  // Controller Gains
  float Kp = 5;
  float Ki = 0;
  float Kd = 0;

  int minSpeed = 70;
public:
  Robot() = default;

  void initialize() {
    leftMotor.initializePins();
    rightMotor.initializePins();
    mpu.initialize();
    pidController.setGains(Kp, Ki, Kd);
  }

  void runMotors(int speed) {
    leftMotor.run(speed);
    rightMotor.run(speed);
  }

  void stopMotors() {
    leftMotor.stop();
    rightMotor.stop();
  }

  void readMPU() {
    mpu.read();
    mpuData[0] = mpu.AcX;
    mpuData[1] = mpu.AcY;
    mpuData[2] = mpu.AcZ;
    mpuData[3] = mpu.GyX;
    mpuData[4] = mpu.GyY;
    mpuData[5] = mpu.GyZ;
  }

  // Print MPU data (Used for testing)
  void printMPU() {
    readMPU();
    Serial.print("AcX: ");
    Serial.println(mpuData[0]);
    Serial.print("AcY: ");
    Serial.println(mpuData[1]);
    Serial.print("AcZ: ");
    Serial.println(mpuData[2]);
    Serial.print("GyX: ");
    Serial.println(mpuData[3]);
    Serial.print("GyY: ");
    Serial.println(mpuData[4]);
    Serial.print("GyZ: ");
    Serial.println(mpuData[5]);
  }

  // Balancing Function:
  // Computes the angles derived from accelerometer and gyroscope data
  // Gets the total angle using a Complementary Filter
  // Error is based off of total Pitch (totalAngle[1]) and sent to the PID controller
  // PID value + minimum speed gets you the speed needed to balance the robot
  void balance() {
    float error = 0, previousError = 0;
    float time, previousTime, elapsedTime;
    float pid;
    float speed;
    time = millis();

    while (true) {
      previousTime = time;
      time = millis();
      elapsedTime = (time - previousTime) / 1000;

      readMPU();

      accelerationAngle[0] = atan((mpuData[1]/16384.0) / sqrt(pow((mpuData[0]/16384.0), 2) + pow((mpuData[2]/16384.0), 2))) * (180 / M_PI);
      accelerationAngle[1] = atan(-1 * (mpuData[0]/16384.0) / sqrt(pow((mpuData[1]/16384.0), 2) + pow((mpuData[2]/16384.0), 2))) * (180 / M_PI);
      
      gyroscopeAngle[0] = mpuData[3] / 131.0;
      gyroscopeAngle[1] = mpuData[4] / 131.0;

      totalAngle[0] = 0.96 * (totalAngle[0] + gyroscopeAngle[0] * elapsedTime) + 0.04 * accelerationAngle[0];
      totalAngle[1] = 0.96 * (totalAngle[1] + gyroscopeAngle[1] * elapsedTime) + 0.04 * accelerationAngle[1];

      error = totalAngle[1];

      pid = pidController.getCmd(error, previousError, elapsedTime);

      Serial.print("Error: ");
      Serial.println(error);
      Serial.print("PID: ");
      Serial.println(pid);

      if (pid > 0) speed = pid + minSpeed;
      else if (pid < 0) speed = pid - minSpeed;
      else speed = 0;

      previousError = error;

      runMotors(speed);

      if (abs(totalAngle[1]) > 45) {
        stopMotors();
      }
    }
  }

};
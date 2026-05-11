#include <Arduino.h>

class Motor {
private:
  int ena, in1, in2;
public:
  Motor(int enaPin, int in1Pin, int in2Pin)
    : ena(enaPin), in1(in1Pin), in2(in2Pin) {}

  void initializePins() {
    pinMode(ena, OUTPUT);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
  }

  // Motor driver functions
  void run(float speed) {
    int pwm, reverse;
    if (speed >= 0) {
      reverse = 0;
      pwm = int(speed);
    } else {
      reverse = 1;
      pwm = abs(int(speed));
    }
    
    if (pwm > 255) {
      pwm = 255;
    }

    digitalWrite(in1, !reverse);
    digitalWrite(in2, reverse);
    analogWrite(ena, pwm);
  }

  void stop() {
    digitalWrite(in1, 0);
    digitalWrite(in2, 0);
    analogWrite(ena, 0);
  }
};
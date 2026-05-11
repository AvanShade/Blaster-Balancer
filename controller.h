#include <Arduino.h>

class Controller {
    public:
        float Kp, Ki, Kd, pidP, pidI, pidD, PID;

        void setGains(float kp, float ki, float kd) {
            Kp = kp;
            Ki = ki;
            Kd = kd;
            pidP = 0;
            pidI = 0;
            pidD = 0;
        }

        float getCmd(float error, float previousError, float elapsedTime) {
            pidP = Kp * error;
            pidI = constrain(pidI + (Ki * error), -200, 200);
            pidD = Kd * ((error - previousError) / elapsedTime);
            PID = pidP + pidI + pidD;

            return PID;
        }
};
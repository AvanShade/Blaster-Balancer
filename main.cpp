#include <Arduino.h>
#include "robot.h"

Robot balancingRobot;

void setup(){
  balancingRobot.initialize();
  Serial.begin(9600);
}

void loop(){
  balancingRobot.balance();
}

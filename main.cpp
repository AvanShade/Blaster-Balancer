// Name: Seth Motta and Daniel Appleford
// Purpose: This project, the Blaster Balancer, is Seth and Daniel's final project for the MEGN441 - Introduction to Robotics course
//          taught at the Colorado School of Mines by Dr. Gary Nave during the Spring 2026 semester.

#include <Arduino.h>
#include "blasterBalancer.h"

BlasterBalancer balancingRobot;

void setup(){
  balancingRobot.initialize();
  Serial.begin(9600);
}

void loop(){
  balancingRobot.balance();
}

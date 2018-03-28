/*
 * Simple demo, should work with any driver board
 *
 * Connect STEP, DIR as indicated
 *
 * Copyright (C)2015-2017 Laurentiu Badea
 *
 * This file may be redistributed under the terms of the MIT license.
 * A copy of this license has been included with this distribution in the file LICENSE.
 */
#include <Arduino.h>
#include "BasicStepperDriver.h"

// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 200
#define RPM 120

// Since microstepping is set externally, make sure this matches the selected mode
// If it doesn't, the motor will move at a different RPM than chosen
// 1=full step, 2=half step etc.
#define MICROSTEPS 4

// All the wires needed for full functionality
#define DIR 8
#define STEP 9
#define DIR2 10
#define STEP2 11
//Uncomment line to use enable/disable functionality
//#define ENABLE 13

// 2-wire basic config, microstepping is hardwired on the driver
BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP);
BasicStepperDriver stepper2(MOTOR_STEPS, DIR2, STEP2);

//Uncomment line to use enable/disable functionality
//BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP, ENABLE);

void setup() {
    stepper.begin(RPM, MICROSTEPS);
    stepper2.begin(RPM, MICROSTEPS);
    pinMode(5, OUTPUT);
    pinMode(12, OUTPUT);
}

void loop() {
    digitalWrite(5, HIGH);
    digitalWrite(12, HIGH);
    // energize coils - the motor will hold position
    // stepper.enable();
  
    /*
     * Moving motor one full revolution using the degree notation
     */
     for (int i=0; i<3; i++){
    for (int x=0; x<3; x++){
      digitalWrite(5, HIGH);
      delay(1);
      stepper.rotate(-600);
      digitalWrite(5, LOW);
      delay(1000);
    }
      digitalWrite(12, HIGH);
      delay(1);
      stepper2.rotate(1800);
      digitalWrite(12, LOW);
      delay(1000);
    for (int x=0; x<3; x++){
      digitalWrite(5, HIGH);
      delay(1);
      stepper.rotate(600);
      digitalWrite(5, LOW);
      delay(1000);
    }
     }
     digitalWrite(12, HIGH);
      delay(1);
      stepper2.rotate(-5400);
      digitalWrite(12, LOW);
      delay(1000);
}

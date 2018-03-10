#include <Arduino.h>
#include "BasicStepperDriver.h"
#include <SoftwareSerial.h>

#define MOTOR_STEPS 200
#define RPM 120
#define MICROSTEPS 4

// All the wires needed for full functionality
#define DIR_UP 8
#define STEP_UP 9
#define SLEEP_UP 5
#define DIR_SIDE 10
#define STEP_SIDE 11
#define SLEEP_SIDE 12

/* Setting up Serial Object */
static char receivedData; // To hold received characters
const byte rxPin = 1;
const byte txPin = 0;
SoftwareSerial picConnection = SoftwareSerial(rxPin, txPin);

BasicStepperDriver verticalStepper(MOTOR_STEPS, DIR_UP, STEP_UP);
BasicStepperDriver horizontalStepper(MOTOR_STEPS, DIR_SIDE, STEP_SIDE);

/*PIC Instructions*/
const char rowUp = '1';
const char rowDown = '2';
const char columnLeft = '3';
const char columnRight = '4';

void setup() {
    /* Configure pin modes for tx and rx. */
    pinMode(rxPin, INPUT);
    pinMode(txPin, OUTPUT);
    picConnection.begin(9600);
  
    verticalStepper.begin(RPM, MICROSTEPS);
    pinMode(SLEEP_UP, OUTPUT);
    digitalWrite(SLEEP_UP, HIGH);

    horizontalStepper.begin(RPM, MICROSTEPS);
    pinMode(SLEEP_SIDE, OUTPUT);
    digitalWrite(SLEEP_SIDE, HIGH);
}

void loop() {
    /* Wait to receive the message from the main PIC. */
    while(picConnection.available() < 1){ continue; }

    receivedData = picConnection.read();

    switch(receivedData){
      case rowUp:
        digitalWrite(SLEEP_UP, HIGH);
        delay(1);
        verticalStepper.rotate(1800);
        digitalWrite(SLEEP_UP, LOW);
        delay(1);
        break;
      case rowDown:
        digitalWrite(SLEEP_UP, HIGH);
        delay(1);
        verticalStepper.rotate(-1800);
        digitalWrite(SLEEP_UP, LOW);
        delay(1);
        break;
      case columnLeft:
        digitalWrite(SLEEP_SIDE, HIGH);
        delay(1);
        horizontalStepper.rotate(600);
        digitalWrite(SLEEP_SIDE, LOW);
        delay(1);
        break;
      case columnRight:
        digitalWrite(SLEEP_SIDE, HIGH);
        delay(1);
        horizontalStepper.rotate(-600);
        digitalWrite(SLEEP_SIDE, LOW);
        delay(1);
        break;
      default:
        break;
    }
}

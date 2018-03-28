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
const byte rxPin = 1;
const byte txPin = 0;
static char arr[2]; // To hold received characters
SoftwareSerial mySerial = SoftwareSerial(rxPin, txPin);

BasicStepperDriver verticalStepper(MOTOR_STEPS, DIR_UP, STEP_UP);
BasicStepperDriver horizontalStepper(MOTOR_STEPS, DIR_SIDE, STEP_SIDE);

/*PIC Instructions*/
const char rowUp = '1';
const char rowDown = '2';
const char columnLeft = '3';
const char columnRight = '4';
const char drawerUp = '5';
const char drawerDown = '6';
const char drawerOpen = '7';
const char drawerClose = '8';
const char drawerUpSmall = '9';
const char drawerDownSmall = 'A';


void setup() {
    /* Configure pin modes for tx and rx. */
    pinMode(rxPin, INPUT);
    pinMode(txPin, OUTPUT);
    mySerial.begin(9600);
  
    verticalStepper.begin(RPM, MICROSTEPS);
    pinMode(SLEEP_UP, OUTPUT);
    digitalWrite(SLEEP_UP, LOW);

    horizontalStepper.begin(RPM, MICROSTEPS);
    pinMode(SLEEP_SIDE, OUTPUT);
    digitalWrite(SLEEP_SIDE, LOW);

    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(6, OUTPUT);
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(6, LOW);
}

void loop() {
    /* Wait to receive the message from the main PIC. */
    while(mySerial.available() < 1){ continue; }

    arr[0] = mySerial.read();

    switch(arr[0]){
      case rowUp:
        digitalWrite(SLEEP_UP, HIGH);
        delay(1);
        verticalStepper.rotate(-1700);
        digitalWrite(SLEEP_UP, LOW);
        delay(1);
        break;
      case rowDown:
        digitalWrite(SLEEP_UP, HIGH);
        delay(1);
        verticalStepper.rotate(1700);
        digitalWrite(SLEEP_UP, LOW);
        delay(1);
        break;
      case columnLeft:
        digitalWrite(SLEEP_SIDE, HIGH);
        delay(1);
        horizontalStepper.rotate(-600);
        digitalWrite(SLEEP_SIDE, LOW);
        delay(1);
        break;
      case columnRight:
        digitalWrite(SLEEP_SIDE, HIGH);
        delay(1);
        horizontalStepper.rotate(600);
        digitalWrite(SLEEP_SIDE, LOW);
        delay(1);
        break;
      case drawerUp:
        digitalWrite(SLEEP_UP, HIGH);
        delay(1);
        verticalStepper.rotate(-700);
        digitalWrite(SLEEP_UP, LOW);
        delay(1);
        break;
      case drawerDown:
        digitalWrite(SLEEP_UP, HIGH);
        delay(1);
        verticalStepper.rotate(700);
        digitalWrite(SLEEP_UP, LOW);
        delay(1);
        break;
      case drawerOpen:
        for (int i=0; i<500; i++){
          digitalWrite(2, LOW);
          digitalWrite(3, LOW);
          digitalWrite(4, HIGH);
          digitalWrite(6, LOW);
          delay(2);
          digitalWrite(2, LOW);
          digitalWrite(3, HIGH);
          digitalWrite(4, LOW);
          digitalWrite(6, LOW);
          delay(2);
          digitalWrite(2, HIGH);
          digitalWrite(3, LOW);
          digitalWrite(4, LOW);
          digitalWrite(6, LOW);
          delay(2);
          digitalWrite(2, LOW);
          digitalWrite(3, LOW);
          digitalWrite(4, LOW);
          digitalWrite(6, HIGH);
          delay(2);
        }
        digitalWrite(2, LOW);
        digitalWrite(3, LOW);
        digitalWrite(4, LOW);
        digitalWrite(6, LOW);
        break;
      case drawerClose:
        for (int i=0; i<500; i++){
          digitalWrite(2, HIGH);
          digitalWrite(3, LOW);
          digitalWrite(4, LOW);
          digitalWrite(6, LOW);
          delay(2);
          digitalWrite(2, LOW);
          digitalWrite(3, HIGH);
          digitalWrite(4, LOW);
          digitalWrite(6, LOW);
          delay(2);
          digitalWrite(2, LOW);
          digitalWrite(3, LOW);
          digitalWrite(4, HIGH);
          digitalWrite(6, LOW);
          delay(2);
          digitalWrite(2, LOW);
          digitalWrite(3, LOW);
          digitalWrite(4, LOW);
          digitalWrite(6, HIGH);
          delay(2);
        }
        digitalWrite(2, LOW);
        digitalWrite(3, LOW);
        digitalWrite(4, LOW);
        digitalWrite(6, LOW);
        break;
      case drawerUpSmall:
        digitalWrite(SLEEP_UP, HIGH);
        delay(1);
        verticalStepper.rotate(-300);
        digitalWrite(SLEEP_UP, LOW);
        delay(1);
        break;
      case drawerDownSmall:
        digitalWrite(SLEEP_UP, HIGH);
        delay(1);
        verticalStepper.rotate(300);
        digitalWrite(SLEEP_UP, LOW);
        delay(1);
        break;
      default:
        break;
    }
}

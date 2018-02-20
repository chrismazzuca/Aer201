/*
 * File:   operationMode.c
 * Author: chrismazzuca
 *
 * Created on February 4, 2018, 4:48 PM
 */

#include "operationMode.h"
//#include "EEPROMstorage.h"

/*Function will start/stop the timer whenever required*/
void configTimer(unsigned int configure){
    /*Function will take in an integer "configure" which will determine if the timer is to be started or stopped*/
}

/*This function will control the arm vertical steppers (connected to the threaded rods)*/
void verticalStepper(unsigned int rotations, unsigned int direction){
    /*Function will control the steppers by the inputted "rotations" variable, and will determine its direction with the input "direction"*/
    /*Will return 1 when finished*/
}

/*This function will control the arm horizontal stepper (connected to the belt)*/
unsigned int horizontalStepper(unsigned int rotations, unsigned int direction){
    /*Function will control the steppers by the inputted "rotations" variable, and will determine its direction with the input "direction"*/
    /*Will return 1 when finished*/
    return 1;
}

/*This function will detect the tape at a given drawer, and return the results*/
unsigned int detectTape(void){
    /*Will return 1 if tape, 0 if no tape*/
    return 1;
}

/*This function will tell the stepper motor to open or close the drawer*/
unsigned int configDrawer(unsigned int direction){
    /*"Direction" specifies whether to open or close the drawer*/
    /*Function will return 1 when finished*/
    return 1;
}

/*This function distributes the specified number of each food type*/
unsigned int distributePieces(unsigned int count, unsigned int pieceType){
    /*"Count" specifies how many pieces, and will determine how long the motor will turn for*/
    /*"Type" specifies which food type, and specifies which DC motor needs to be turned on*/
    return count;
}

/*Counts the number of pieces distributed (through contact sensor), by food type*/
unsigned int checkCount(unsigned int pieceType){
    /*Function will take in food type, and count the number of pieces distributed*/
    return 1;
}

/*This function will release the pieces into the reservoirs*/
unsigned int trapDoor(unsigned int type, unsigned int direction){
    /*"type" specifies which food type is to be released*/
    /*"direction" specifies which direction the trapDoor needs to turn*/
    /*This function will return 1 when finished*/
    return 1;
}

/*This function will be called in main.c, and holds the code for the entire operation*/
void mainOperation(void){
    /* writeEEPROM(address, data);               Loop through and store data from the inputs, date and time
     * configTimer(1);                            Start the timer
     * 
     * 
     * First initialize arm to (0,0), or drawer 1:
     * verticalStepper(rotation, direction);      Move the arm to the appropriate height
     * horizontalStepper(rotation, direction);    Move the arm to the appropriate horizontal coordinate
     * 
     * 
     * Iterate through all drawers inputted by the user (in order of drawer number):
     * verticalStepper(rotation, direction);      Move the arm to the appropriate height
     * horizontalStepper(rotation, direction);    Move the arm to the appropriate horizontal coordinate
     * if (detectTape() == 1)                     Then proceed, otherwise move to the next drawer;
     * configDrawer(direction);                   Open the drawer
     * distributePieces(count, type);             Distribute pieces based on inputs
     * configDrawer(direction);                   Close the drawer
     * 
     * 
     * Continue this loop until the drawers are properly filled:
     * configTimer(0);                            Stop the timer
     * writeEEPROM(address, data);               Store duration and remaining pieces in EEPROM
     * trapDoor(type, direction);                 Release all pieces to the reservoirs
     */
}
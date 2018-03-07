/*
 * File:   operationMode.c
 * Author: chrismazzuca
 *
 * Created on February 4, 2018, 4:48 PM
 */

#include "operationMode.h"
#include "arduinoSetup.h"
#include "UART_PIC.h"
#include "EEPROMstorage.h"
#include "RTC.h"
#include "ADC.h"
#include "LCD.h"

unsigned char rowUp[1] = {'1'};
unsigned char rowDown[1] = {'2'};
unsigned char columnLeft[1] = {'3'};
unsigned char columnRight[1] = {'4'};

unsigned int drawerTable[8] = {0,0,0,0,0,0,0,0};
unsigned int sortedDrawerTable[8] = {0,0,0,0,0,0,0,0};
unsigned int dietTable[8] = {0,0,0,0,0,0,0,0};
unsigned int foodTable[8] = {0,0,0,0,0,0,0,0};


/*Gets the inputs (drawer, diet, food) from EEPROM and stores in a global list*/
void getUserInputs(void){
    unsigned int data = 0;
    unsigned int i = 0;
    unsigned int logNumber = 0;
    unsigned int n = 0;
    unsigned int count = 0;
    
    logNumber = readEEPROM(250);
    
    if (logNumber == 0 || logNumber == 4){
        n = 0;
    }
    else{
        n = (logNumber)*35;
    }
    
    for (i=11; i<35; i+=3){
        data = readEEPROM(i+n);
        drawerTable[count] = data;
        data = readEEPROM(i+1+n);
        dietTable[count] = data;
        data = readEEPROM(i+2+n);
        foodTable[count] = data;
        count += 1;
    }
}


/*Sorts the drawer list so that we can visit drawers in order (regardless of user input)*/
void sortDrawerList(void){
    unsigned int i = 0;
    unsigned int j = 0;
    unsigned int temp = 0;
    
    for (i=0; i<8; i++){
        sortedDrawerTable[i] = drawerTable[i];
    }
    
    for (i=0; i<8; i++){
		for (j=0; j<8; j++){
			if (sortedDrawerTable[j] > sortedDrawerTable[i]){
				temp = sortedDrawerTable[i];
				sortedDrawerTable[i] = sortedDrawerTable[j];
				sortedDrawerTable[j] = temp;   
			}  
		}
	}
}


/*This function will control the arm vertical steppers (connected to the threaded rods)*/
void verticalStepper(unsigned int rotations, unsigned int direction){
    /*Function will control the steppers by the inputted "rotations" variable, and will determine its direction with the input "direction"*/
    
    unsigned int i = 0;
    
    if (direction == 1){
        for (i=0; i<rotations; i++){
            uartTransmitBlocking(rowUp, 1);
        }
    }
    else if (direction == 0){
        for (i=0; i<rotations; i++){
            uartTransmitBlocking(rowDown, 1);
        }
    }
}

/*This function will control the arm horizontal stepper (connected to the belt)*/
void horizontalStepper(unsigned int rotations, unsigned int direction){
    /*Function will control the steppers by the inputted "rotations" variable, and will determine its direction with the input "direction"*/
    
    unsigned int i = 0;
    
    if (direction == 1){
        for (i=0; i<rotations; i++){
            uartTransmitBlocking(columnLeft, 1);
        }
    }
    else if (direction == 0){
        for (i=0; i<rotations; i++){
            uartTransmitBlocking(columnRight, 1);
        }
    }
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

/*This function distributes the specified number of round pieces*/
unsigned int distributeRound(unsigned int count){
    /*"Count" specifies how many pieces, and will determine how long the motor will turn for*/
    /*"Type" specifies which food type, and specifies which DC motor needs to be turned on*/
    
    unsigned int i = 0;
    
    TRISCbits.TRISC5 = 0;
    TRISCbits.TRISC6 = 0;
    TRISCbits.TRISC7 = 0;
    TRISCbits.TRISC0 = 0;
    LATCbits.LATC5 = 0;
    LATCbits.LATC6 = 0;
    LATCbits.LATC7 = 0;
    LATCbits.LATC0 = 0;
    
    for (i=0; i<count; i++){
        LATCbits.LATC5 = 1;
        LATCbits.LATC6 = 0;
        LATCbits.LATC7 = 0;
        LATCbits.LATC0 = 0;
        __delay_ms(2);
        LATCbits.LATC5 = 0;
        LATCbits.LATC6 = 1;
        LATCbits.LATC7 = 0;
        LATCbits.LATC0 = 0;
        __delay_ms(2);
        LATCbits.LATC5 = 0;
        LATCbits.LATC6 = 0;
        LATCbits.LATC7 = 1;
        LATCbits.LATC0 = 0;
        __delay_ms(2);
        LATCbits.LATC5 = 0;
        LATCbits.LATC6 = 0;
        LATCbits.LATC7 = 0;
        LATCbits.LATC0 = 1;
        __delay_ms(2);
    }
    
    return 1;
}

/*This function distributes the specified number of flat pieces*/
unsigned int distributeFlat(unsigned int count){
    /*"Count" specifies how many pieces, and will determine how long the motor will turn for*/
    /*"Type" specifies which food type, and specifies which DC motor needs to be turned on*/
    
    unsigned int i = 0;
    
    TRISCbits.TRISC5 = 0;
    TRISCbits.TRISC6 = 0;
    TRISCbits.TRISC7 = 0;
    TRISCbits.TRISC0 = 0;
    LATCbits.LATC5 = 0;
    LATCbits.LATC6 = 0;
    LATCbits.LATC7 = 0;
    LATCbits.LATC0 = 0;
    
    for (i=0; i<count; i++){
        LATCbits.LATC5 = 1;
        LATCbits.LATC6 = 0;
        LATCbits.LATC7 = 0;
        LATCbits.LATC0 = 0;
        __delay_ms(2);
        LATCbits.LATC5 = 0;
        LATCbits.LATC6 = 1;
        LATCbits.LATC7 = 0;
        LATCbits.LATC0 = 0;
        __delay_ms(2);
        LATCbits.LATC5 = 0;
        LATCbits.LATC6 = 0;
        LATCbits.LATC7 = 1;
        LATCbits.LATC0 = 0;
        __delay_ms(2);
        LATCbits.LATC5 = 0;
        LATCbits.LATC6 = 0;
        LATCbits.LATC7 = 0;
        LATCbits.LATC0 = 1;
        __delay_ms(2);
    }
    return 1;
}

/*This function distributes the specified number of long pieces*/
unsigned int distributeLong(unsigned int count){
    /*"Count" specifies how many pieces, and will determine how long the motor will turn for*/
    /*"Type" specifies which food type, and specifies which DC motor needs to be turned on*/
    
        unsigned int i = 0;
    
    TRISCbits.TRISC5 = 0;
    TRISCbits.TRISC6 = 0;
    TRISCbits.TRISC7 = 0;
    TRISCbits.TRISC0 = 0;
    LATCbits.LATC5 = 0;
    LATCbits.LATC6 = 0;
    LATCbits.LATC7 = 0;
    LATCbits.LATC0 = 0;
    
    for (i=0; i<count; i++){
        LATCbits.LATC5 = 1;
        LATCbits.LATC6 = 0;
        LATCbits.LATC7 = 0;
        LATCbits.LATC0 = 0;
        __delay_ms(2);
        LATCbits.LATC5 = 0;
        LATCbits.LATC6 = 1;
        LATCbits.LATC7 = 0;
        LATCbits.LATC0 = 0;
        __delay_ms(2);
        LATCbits.LATC5 = 0;
        LATCbits.LATC6 = 0;
        LATCbits.LATC7 = 1;
        LATCbits.LATC0 = 0;
        __delay_ms(2);
        LATCbits.LATC5 = 0;
        LATCbits.LATC6 = 0;
        LATCbits.LATC7 = 0;
        LATCbits.LATC0 = 1;
        __delay_ms(2);
    }
    return 1;
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
    unsigned int i = 0;
    unsigned int currentDrawer = 1;
    unsigned int nextDrawer = 0;
    
    /* Initialize UART. */
    UART_Init(9600);
    
    getUserInputs();
    sortDrawerList();
    
    for (i=0; i<7; i++){
        if (sortedDrawerTable[i] != 0){
            if (currentDrawer != sortedDrawerTable[i]){
                __lcd_clear();
                printf("Filling");
                __lcd_newline();
                printf("drawer %i", sortedDrawerTable[i]);
                __delay_ms(1000);
            }
            else{
                __lcd_clear();
                printf("Moving to");
                __lcd_newline();
                printf("drawer %i", sortedDrawerTable[i]);
                __delay_ms(1000);
            }
        }
    }
    
    /* writeEEPROM(address, data);                Loop through and store data from the inputs, date and time
     * tick();                                    Start the timer
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
     * tock()       ;                             Stop the timer
     * writeEEPROM(address, data);                Store duration and remaining pieces in EEPROM
     * trapDoor(type, direction);                 Release all pieces to the reservoirs
     */
}
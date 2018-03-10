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


/*Gets the inputs (drawer, diet, food) from EEPROM and stores in a global list*/
void getUserInputs(unsigned int drawerTable[8], unsigned int dietTable[8], unsigned int foodTable[8]){
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
void sortDrawerList(unsigned int drawerTable[8], unsigned int sortedDrawerTable[8]){
    unsigned int i = 0;
    unsigned int j = 0;
    unsigned int sublist1[] = {0,0,0,0};
    unsigned int count1 = 0;
    unsigned int sublist2[] = {0,0,0,0};
    unsigned int count2 = 0;
    unsigned int sublist3[] = {0,0,0,0};
    unsigned int count3 = 0;
    unsigned int sublist4[] = {0,0,0,0};
    unsigned int count4 = 0;
    unsigned int temp;
    
    for (i=0; i<8; i++){
        sortedDrawerTable[i] = drawerTable[i];
    }
    
    for(i=0; i<8; i++){
      if (sortedDrawerTable[i] == 1 || sortedDrawerTable[i] == 2 || sortedDrawerTable[i] == 3 || sortedDrawerTable[i] == 4){
        sublist1[count1]=sortedDrawerTable[i];
        count1 ++;
      }
      else if (sortedDrawerTable[i] == 5 || sortedDrawerTable[i] == 6 || sortedDrawerTable[i] == 7 || sortedDrawerTable[i] == 8){
        sublist2[count2]=sortedDrawerTable[i];
        count2+=1;
      }
      else if (sortedDrawerTable[i] == 9 || sortedDrawerTable[i] == 10 || sortedDrawerTable[i] == 11 || sortedDrawerTable[i] == 12){
        sublist3[count3]=sortedDrawerTable[i];
        count3+=1;
      }
      else if (sortedDrawerTable[i] == 13 || sortedDrawerTable[i] == 14 || sortedDrawerTable[i] == 15 || sortedDrawerTable[i] == 16){
        sublist4[count4]=sortedDrawerTable[i];
        count4+=1;
      }
    }
    
    for (i=0; i<count1; i++){
		for (j=0; j<count1; j++){
			if (sublist1[j] > sublist1[i]){
				temp = sublist1[i];
				sublist1[i] = sublist1[j];
				sublist1[j] = temp;   
			}  
		}
	}
	
	for (i=0; i<count3; i++){
		for (j=0; j<count3; j++){
			if (sublist3[j] > sublist3[i]){
				temp = sublist3[i];
				sublist3[i] = sublist3[j];
				sublist3[j] = temp;   
			}  
		}
	}
	
	for (i=0; i<count2; i++){
		for (j=0; j<count2; j++){
			if (sublist2[j] < sublist2[i]){
				temp = sublist2[i];
				sublist2[i] = sublist2[j];
				sublist2[j] = temp;   
			}  
		}
	}
	
	for (i=0; i<count4; i++){
		for (j=0; j<count4; j++){
			if (sublist4[j] < sublist4[i]){
				temp = sublist4[i];
				sublist4[i] = sublist4[j];
				sublist4[j] = temp;   
			}  
		}
	}
	
	for (i=0; i<count1; i++){
	  sortedDrawerTable[i] = sublist1[i];
	}
	for (i=0; i<count2; i++){
	  sortedDrawerTable[i+count1] = sublist2[i];
	}
	for (i=0; i<count3; i++){
	  sortedDrawerTable[i+count1+count2] = sublist3[i];
	}
	for (i=0; i<count4; i++){
	  sortedDrawerTable[i+count1+count2+count3] = sublist4[i];
	}
}


/*This function will control the arm vertical steppers (connected to the threaded rods)*/
void verticalStepper(unsigned int rotations, unsigned int direction){
    /*Function will control the steppers by the inputted "rotations" variable, and will determine its direction with the input "direction"*/
    
    unsigned int i = 0;
    
    /*Move up*/
    if (direction == 1){
        for (i=0; i<rotations; i++){
            uartTransmitBlocking(rowUp, 1);
        }
    }
    
    /*Move down*/
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
    
    /*Move left*/
    if (direction == 1){
        for (i=0; i<rotations; i++){
            uartTransmitBlocking(columnLeft, 1);
        }
    }
    /*Move right*/
    else if (direction == 0){
        for (i=0; i<rotations; i++){
            uartTransmitBlocking(columnRight, 1);
        }
    }
}


/*Control vertical motor logic*/
unsigned int moveVertically(unsigned int currentDrawer, unsigned int nextDrawer){
    
    if (currentDrawer != 0 && nextDrawer != 0 && nextDrawer > currentDrawer){
        if (currentDrawer == 1 || currentDrawer == 2 || currentDrawer == 3 || currentDrawer == 4){
            if (nextDrawer == 5 || nextDrawer == 6 || nextDrawer == 7 || nextDrawer == 8){
                verticalStepper(1, 0);
                return (currentDrawer + 4);
            }
            else if (nextDrawer == 9 || nextDrawer == 10 || nextDrawer == 11 || nextDrawer == 12){
                verticalStepper(2, 0);
                return (currentDrawer + 8);
            }
            else if (nextDrawer == 13 || nextDrawer == 14 || nextDrawer == 15 || nextDrawer == 16){
                verticalStepper(3, 0);
                return (currentDrawer + 12);
            }
        }
        else if (currentDrawer == 5 || currentDrawer == 6 || currentDrawer == 7 || currentDrawer == 8){
            if (nextDrawer == 9 || nextDrawer == 10 || nextDrawer == 11 || nextDrawer == 12){
                verticalStepper(1, 0);
                return (currentDrawer + 4);
            }
            else if (nextDrawer == 13 || nextDrawer == 14 || nextDrawer == 15 || nextDrawer == 16){
                verticalStepper(2, 0);
                return (currentDrawer + 8);
            }
        }
        else if (currentDrawer == 9 || currentDrawer == 10 || currentDrawer == 11 || currentDrawer == 12){
            if (nextDrawer == 13 || nextDrawer == 14 || nextDrawer == 15 || nextDrawer == 16){
                verticalStepper(1, 0);
                return (currentDrawer + 4);
            }
        }
    }
    else{
        return currentDrawer;
    }
}


/*Control vertical motor logic*/
void moveHorizontally(unsigned int currentDrawer, unsigned int nextDrawer){
    if (currentDrawer != 0 && nextDrawer != 0){
        /*Move arm right*/
        if (currentDrawer < nextDrawer){
            if ((nextDrawer - currentDrawer) == 1){
                horizontalStepper(1, 0);
            }
            else if ((nextDrawer - currentDrawer) == 2){
                horizontalStepper(2, 0);
            }
            else if ((nextDrawer - currentDrawer) == 3){
                horizontalStepper(3, 0);
            }
        }
        /*Move arm left*/
        else if (currentDrawer > nextDrawer){
            if ((currentDrawer - nextDrawer) == 1){
                horizontalStepper(1, 1);
            }
            else if ((currentDrawer - nextDrawer) == 2){
                horizontalStepper(2, 1);
            }
            else if ((currentDrawer - nextDrawer) == 3){
                horizontalStepper(3, 1);
            }
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
unsigned int trapDoor(unsigned int direction){
    /*"type" specifies which food type is to be released*/
    /*"direction" specifies which direction the trapDoor needs to turn*/
    /*This function will return 1 when finished*/
    
    TRISCbits.TRISC5 = 0;
    TRISCbits.TRISC6 = 0;
    TRISCbits.TRISC7 = 0;
    
    LATCbits.LATC5 = 1;
    LATCbits.LATC6 = 1;
    LATCbits.LATC7 = 1;
    
    if (direction == 1){
        LATCbits.LATC5 = 1;
        LATCbits.LATC6 = 0;
        __delay_ms(1000);
        LATCbits.LATC5 = 1;
        LATCbits.LATC6 = 1;
        __delay_ms(1);
        LATCbits.LATC5 = 0;
        LATCbits.LATC6 = 0;
    }
    else if (direction == 0){
        LATCbits.LATC5 = 0;
        LATCbits.LATC6 = 1;
        __delay_ms(1000);
        LATCbits.LATC5 = 1;
        LATCbits.LATC6 = 1;
        __delay_ms(1);
        LATCbits.LATC5 = 0;
        LATCbits.LATC6 = 0;
    }
    
    return 1;
}

/*This function will be called in main.c, and holds the code for the entire operation*/
void mainOperation(void){
    unsigned int i = 0;
    unsigned int currentDrawer = 0;
    unsigned int temp = 0;
    unsigned int nextDrawer = 0;
    
    unsigned int drawerTable[8] = {0,0,0,0,0,0,0,0};
    unsigned int sortedDrawerTable[8] = {0,0,0,0,0,0,0,0};
    unsigned int dietTable[8] = {0,0,0,0,0,0,0,0};
    unsigned int foodTable[8] = {0,0,0,0,0,0,0,0};
    
    /*Call function to initialize arm*/
    currentDrawer = 1;
    
    getUserInputs(drawerTable, dietTable, foodTable);
    sortDrawerList(drawerTable, sortedDrawerTable);
    
    for (i=0; i<7; i++){
        if (sortedDrawerTable[i] != 0){
            currentDrawer = sortedDrawerTable[i];
            nextDrawer = sortedDrawerTable[i+1];
            __lcd_clear();
            printf("Moving to");
            __lcd_newline();
            printf("drawer %i", sortedDrawerTable[i]);
            __delay_ms(1000);
            
            temp = moveVertically(currentDrawer, nextDrawer);
            currentDrawer = temp;
            moveHorizontally(currentDrawer, nextDrawer);
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
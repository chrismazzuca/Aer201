/*
 * File:   operationMode.c
 * Author: chrismazzuca
 *
 * Created on February 4, 2018, 4:48 PM
 */

#include "operationMode.h"
//#include "arduinoSetup.h"
#include "UART_PIC.h"
#include "EEPROMstorage.h"
#include "RTC.h"
#include "ADC.h"
#include "LCD.h"

unsigned char rowUp[1] = {'1'};
unsigned char rowDown[1] = {'2'};
unsigned char columnLeft[1] = {'3'};
unsigned char columnRight[1] = {'4'};
unsigned char drawerUp[1] = {'5'};
unsigned char drawerDown[1] = {'6'};
unsigned char drawerOpen[1] = {'7'};
unsigned char drawerClose[1] = {'8'};

const char* inputs[] = {"R", "F", "L", "RF", "RL", "FL", "RRF", "RRL", "RFF", "RLL", "RFL", "FFL", "FLL", "RRFL", "RFFL", "RFLL", "RLLL", "FLLL"};
const char* foodInputs[] = {"1", "2", "3", "11", "12", "13", "21", "22", "111", "112", "121", "211", "1111"};


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
        n = (logNumber-1)*35;
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
        count1 += 1;
      }
      else if (sortedDrawerTable[i] == 5 || sortedDrawerTable[i] == 6 || sortedDrawerTable[i] == 7 || sortedDrawerTable[i] == 8){
        sublist2[count2]=sortedDrawerTable[i];
        count2 += 1;
      }
      else if (sortedDrawerTable[i] == 9 || sortedDrawerTable[i] == 10 || sortedDrawerTable[i] == 11 || sortedDrawerTable[i] == 12){
        sublist3[count3]=sortedDrawerTable[i];
        count3 += 1;
      }
      else if (sortedDrawerTable[i] == 13 || sortedDrawerTable[i] == 14 || sortedDrawerTable[i] == 15 || sortedDrawerTable[i] == 16){
        sublist4[count4]=sortedDrawerTable[i];
        count4 += 1;
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
    
    if (sublist3[count3 - 1] == 10 || sublist3[count3 - 1] == 9){
        for (i=0; i<count4; i++){
            for (j=0; j<count4; j++){
                if (sublist4[j] > sublist4[i]){
                    temp = sublist4[i];
                    sublist4[i] = sublist4[j];
                    sublist4[j] = temp;   
                }  
            }
        }
    }
    
    if (sublist2[0] == 7 || sublist2[0] == 8){
        for (i=0; i<count3; i++){
            for (j=0; j<count3; j++){
                if (sublist3[j] < sublist3[i]){
                    temp = sublist3[i];
                    sublist3[i] = sublist3[j];
                    sublist3[j] = temp;   
                }  
            }
        }
    }

    if (sublist1[count1 - 1] == 1 || sublist1[count1 - 1] == 2){
        for (i=0; i<count2; i++){
            for (j=0; j<count2; j++){
                if (sublist2[j] > sublist2[i]){
                    temp = sublist2[i];
                    sublist2[i] = sublist2[j];
                    sublist2[j] = temp;   
                }  
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


/*Get the index of the sorted list in the drawer list*/
unsigned int findDrawerIndex(unsigned int drawerTable[8], unsigned int sortedDrawerValue){
    unsigned int returnValue = 0;
    unsigned int i = 0;
    
    for (i=0; i<8; i++){
        if(drawerTable[i] == sortedDrawerValue){
            returnValue = i;
        }
    }
    return returnValue;
}

/*Function returns the number of round pieces required in the drawer*/
unsigned int getRoundPieces(unsigned int dietType, unsigned int foodType){
    unsigned int returnValue = 0;
    
    switch(dietType){
        /*R*/
        case 0:
            if (foodType == 0){
                returnValue = 1;
            }
            else if (foodType == 1){
                returnValue = 2;
            }
            break;
        /*RF*/
        case 3:
            if (foodType == 3){
                returnValue = 1;
            }
            else if (foodType == 4){
                returnValue = 1;
            }
            else if (foodType == 6){
                returnValue = 2;
            }
            else if (foodType == 7){
                returnValue = 2;
            }
            break;
        /*RL*/
        case 4:
            if (foodType == 3){
                returnValue = 1;
            }
            else if (foodType == 4){
                returnValue = 1;
            }
            else if (foodType == 5){
                returnValue = 1;
            }
            else if (foodType == 6){
                returnValue = 2;
            }
            else if (foodType == 7){
                returnValue = 2;
            }
            break;
        /*RRF*/
        case 6:
            if (foodType == 8){
                returnValue = 2;
            }
            else if (foodType == 9){
                returnValue = 2;
            }
            break;
        /*RRL*/
        case 7:
            if (foodType == 8){
                returnValue = 2;
            }
            else if (foodType == 9){
                returnValue = 2;
            }
            break;
        /*RFF*/
        case 8:
            if (foodType == 8){
                returnValue = 1;
            }
            else if (foodType == 11){
                returnValue = 2;
            }
            break;
        /*RLL*/
        case 9:
            if (foodType == 8){
                returnValue = 1;
            }
            else if (foodType == 9){
                returnValue = 1;
            }
            else if (foodType == 10){
                returnValue = 1;
            }
            else if (foodType == 11){
                returnValue = 2;
            }
            break;
        /*RFL*/
        case 10:
            if (foodType == 8){
                returnValue = 1;
            }
            else if (foodType == 9){
                returnValue = 1;
            }
            else if (foodType == 10){
                returnValue = 1;
            }
            else if (foodType == 11){
                returnValue = 2;
            }
            break;
        /*RRFL*/
        case 13:
            if (foodType == 12){
                returnValue = 2;
            }
            break;
        /*RFFL*/
        case 14:
            if (foodType == 12){
                returnValue = 1;
            }
            break;
        /*RFLL*/
        case 15:
            if (foodType == 12){
                returnValue = 1;
            }
            break;
        /*RLLL*/
        case 16:
            if (foodType == 12){
                returnValue = 1;
            }
            break;
        default:
            returnValue = 0;
            break;
    }
    return returnValue;
}


/*Function returns the number of round pieces required in the drawer*/
unsigned int getFlatPieces(unsigned int dietType, unsigned int foodType){
    unsigned int returnValue = 0;
    
    switch(dietType){
        /*F*/
        case 1:
            if (foodType == 0){
                returnValue = 1;
            }
            else if (foodType == 1){
                returnValue = 2;
            }
            break;
        /*RF*/
        case 3:
            if (foodType == 3){
                returnValue = 1;
            }
            else if (foodType == 4){
                returnValue = 2;
            }
            else if (foodType == 6){
                returnValue = 1;
            }
            else if (foodType == 7){
                returnValue = 2;
            }
            break;
        /*FL*/
        case 5:
            if (foodType == 3){
                returnValue = 1;
            }
            else if (foodType == 4){
                returnValue = 1;
            }
            else if (foodType == 5){
                returnValue = 1;
            }
            else if (foodType == 6){
                returnValue = 2;
            }
            else if (foodType == 7){
                returnValue = 2;
            }
            break;
        /*RRF*/
        case 6:
            if (foodType == 8){
                returnValue = 1;
            }
            else if (foodType == 9){
                returnValue = 2;
            }
            break;
        /*RFF*/
        case 8:
            if (foodType == 8){
                returnValue = 2;
            }
            else if (foodType == 11){
                returnValue = 2;
            }
            break;
        /*RFL*/
        case 10:
            if (foodType == 8){
                returnValue = 1;
            }
            else if (foodType == 9){
                returnValue = 1;
            }
            else if (foodType == 10){
                returnValue = 2;
            }
            else if (foodType == 11){
                returnValue = 1;
            }
            break;
        /*FFL*/
        case 11:
            if (foodType == 8){
                returnValue = 2;
            }
            else if (foodType == 9){
                returnValue = 2;
            }
            break;
        /*FLL*/
        case 12:
            if (foodType == 8){
                returnValue = 1;
            }
            else if (foodType == 9){
                returnValue = 1;
            }
            else if (foodType == 10){
                returnValue = 1;
            }
            else if (foodType == 11){
                returnValue = 2;
            }
            break;
        /*RRFL*/
        case 13:
            if (foodType == 12){
                returnValue = 1;
            }
            break;
        /*RFFL*/
        case 14:
            if (foodType == 12){
                returnValue = 2;
            }
            break;
        /*RFLL*/
        case 15:
            if (foodType == 12){
                returnValue = 1;
            }
            break;
        /*FLLL*/
        case 17:
            if (foodType == 12){
                returnValue = 1;
            }
            break;
        default:
            returnValue = 0;
            break;
    }
    return returnValue;
}


/*Function returns the number of round pieces required in the drawer*/
unsigned int getLongPieces(unsigned int dietType, unsigned int foodType){
    unsigned int returnValue = 0;
    switch(dietType){
        /*L*/
        case 2:
            if (foodType == 0){
                returnValue = 1;
            }
            else if (foodType == 1){
                returnValue = 2;
            }
            else if (foodType == 2){
                returnValue = 3;
            }
            break;
        /*RL*/
        case 4:
            if (foodType == 3){
                returnValue = 1;
            }
            else if (foodType == 4){
                returnValue = 2;
            }
            else if (foodType == 5){
                returnValue = 3;
            }
            else if (foodType == 6){
                returnValue = 1;
            }
            else if (foodType == 7){
                returnValue = 2;
            }
            break;
        /*FL*/
        case 5:
            if (foodType == 3){
                returnValue = 1;
            }
            else if (foodType == 4){
                returnValue = 2;
            }
            else if (foodType == 5){
                returnValue = 3;
            }
            else if (foodType == 6){
                returnValue = 1;
            }
            else if (foodType == 7){
                returnValue = 2;
            }
            break;
        /*RRL*/
        case 7:
            if (foodType == 8){
                returnValue = 1;
            }
            else if (foodType == 9){
                returnValue = 2;
            }
            break;
        /*RLL*/
        case 9:
            if (foodType == 8){
                returnValue = 2;
            }
            else if (foodType == 9){
                returnValue = 3;
            }
            else if (foodType == 10){
                returnValue = 3;
            }
            else if (foodType == 11){
                returnValue = 2;
            }
            break;
        /*RFL*/
        case 10:
            if (foodType == 8){
                returnValue = 1;
            }
            else if (foodType == 9){
                returnValue = 2;
            }
            else if (foodType == 10){
                returnValue = 1;
            }
            else if (foodType == 11){
                returnValue = 1;
            }
            break;
        /*FFL*/
        case 11:
            if (foodType == 8){
                returnValue = 1;
            }
            else if (foodType == 9){
                returnValue = 2;
            }
            break;
        /*FLL*/
        case 12:
            if (foodType == 8){
                returnValue = 2;
            }
            else if (foodType == 9){
                returnValue = 3;
            }
            else if (foodType == 10){
                returnValue = 3;
            }
            else if (foodType == 11){
                returnValue = 2;
            }
            break;
        /*RRFL*/
        case 13:
            if (foodType == 12){
                returnValue = 1;
            }
            break;
        /*RFFL*/
        case 14:
            if (foodType == 12){
                returnValue = 1;
            }
            break;
        /*RFLL*/
        case 15:
            if (foodType == 12){
                returnValue = 2;
            }
            break;
        /*RLLL*/
        case 16:
            if (foodType == 12){
                returnValue = 3;
            }
            break;
        /*FLLL*/
        case 17:
            if (foodType == 12){
                returnValue = 3;
            }
            break;
        default:
            returnValue = 0;
            break;
    }
    return returnValue;
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

/*This function will control the arm vertical stepper to hook onto the drawer*/
void drawerStepper(unsigned int direction){
    
    /*Move up*/
    if (direction == 1){
        uartTransmitBlocking(drawerUp, 1);
    }
    /*Move down*/
    else if (direction == 0){
        uartTransmitBlocking(drawerDown, 1);
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
            else if (nextDrawer == 1 || nextDrawer == 2 || nextDrawer == 3 || nextDrawer == 4){
                return currentDrawer;
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

/*Initialize the arm back to drawer 1*/
void initArm(unsigned int currentDrawer){
    if (currentDrawer != 0){
        if (currentDrawer == 1 || currentDrawer == 2 || currentDrawer == 3 || currentDrawer == 4){
            if (currentDrawer == 4){
                horizontalStepper(3, 1);
            }
            else if (currentDrawer == 3){
                horizontalStepper(2, 1);
            }
            else if (currentDrawer == 2){
                horizontalStepper(1, 1);
            }
        }
        else if (currentDrawer == 5 || currentDrawer == 6 || currentDrawer == 7 || currentDrawer == 8){
            verticalStepper(1, 1);
            if (currentDrawer == 8){
                horizontalStepper(3, 1);
            }
            else if (currentDrawer == 7){
                horizontalStepper(2, 1);
            }
            else if (currentDrawer == 6){
                horizontalStepper(1, 1);
            }
        }
        else if (currentDrawer == 9 || currentDrawer == 10 || currentDrawer == 11 || currentDrawer == 12){
            verticalStepper(2, 1);
            if (currentDrawer == 12){
                horizontalStepper(3, 1);
            }
            else if (currentDrawer == 11){
                horizontalStepper(2, 1);
            }
            else if (currentDrawer == 10){
                horizontalStepper(1, 1);
            }
        }
        else if (currentDrawer == 13 || currentDrawer == 14 || currentDrawer == 15 || currentDrawer == 16){
            verticalStepper(3, 1);
            if (currentDrawer == 16){
                horizontalStepper(3, 1);
            }
            else if (currentDrawer == 15){
                horizontalStepper(2, 1);
            }
            else if (currentDrawer == 14){
                horizontalStepper(1, 1);
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
void configDrawer(unsigned int direction){
    /*"Direction" specifies whether to open or close the drawer*/
    
    /*Open drawer*/
    if (direction == 1){
        uartTransmitBlocking(drawerOpen, 1);
    }
    /*Close drawer*/
    else if (direction == 0){
        uartTransmitBlocking(drawerClose, 1);
    }
}

/*This function distributes the specified number of round pieces*/
void distributeRound(unsigned int count){
    /*"Count" specifies how many pieces, and will determine how long the motor will turn for*/
    /*"Type" specifies which food type, and specifies which DC motor needs to be turned on*/
    
    unsigned int i = 0;
    
    for (i=0; i<count*523; i++){
        LATDbits.LATD4 = 0;
        LATDbits.LATD5 = 0;
        LATDbits.LATD6 = 1;
        LATDbits.LATD7 = 0;
        __delay_ms(2);
        LATDbits.LATD4 = 0;
        LATDbits.LATD5 = 1;
        LATDbits.LATD6 = 0;
        LATDbits.LATD7 = 0;
        __delay_ms(2);
        LATDbits.LATD4 = 1;
        LATDbits.LATD5 = 0;
        LATDbits.LATD6 = 0;
        LATDbits.LATD7 = 0;
        __delay_ms(2);
        LATDbits.LATD4 = 0;
        LATDbits.LATD5 = 0;
        LATDbits.LATD6 = 0;
        LATDbits.LATD7 = 1;
        __delay_ms(2);
    }
    
    LATDbits.LATD4 = 0;
    LATDbits.LATD5 = 0;
    LATDbits.LATD6 = 0;
    LATDbits.LATD7 = 0;
}

/*This function distributes the specified number of flat pieces*/
void distributeFlat(unsigned int count){
    /*"Count" specifies how many pieces, and will determine how long the motor will turn for*/
    /*"Type" specifies which food type, and specifies which DC motor needs to be turned on*/
    
    unsigned int i = 0;
    
    for (i=0; i<count*523; i++){
        LATDbits.LATD0 = 1;
        LATDbits.LATD1 = 0;
        LATDbits.LATD2 = 0;
        LATDbits.LATD3 = 0;
        __delay_ms(2);
        LATDbits.LATD0 = 0;
        LATDbits.LATD1 = 1;
        LATDbits.LATD2 = 0;
        LATDbits.LATD3 = 0;
        __delay_ms(2);
        LATDbits.LATD0 = 0;
        LATDbits.LATD1 = 0;
        LATDbits.LATD2 = 1;
        LATDbits.LATD3 = 0;
        __delay_ms(2);
        LATDbits.LATD0 = 0;
        LATDbits.LATD1 = 0;
        LATDbits.LATD2 = 0;
        LATDbits.LATD3 = 1;
        __delay_ms(2);
    }
    
    LATDbits.LATD0 = 0;
    LATDbits.LATD1 = 0;
    LATDbits.LATD2 = 0;
    LATDbits.LATD3 = 0;
}

/*This function distributes the specified number of long pieces*/
void distributeLong(unsigned int count){
    /*"Count" specifies how many pieces, and will determine how long the motor will turn for*/
    /*"Type" specifies which food type, and specifies which DC motor needs to be turned on*/
    
    unsigned int i = 0;
    
    for (i=0; i<count*523; i++){
        LATCbits.LATC0 = 0;
        LATCbits.LATC1 = 0;
        LATCbits.LATC2 = 1;
        LATEbits.LATE2 = 0;
        __delay_ms(2);
        LATCbits.LATC0 = 0;
        LATCbits.LATC1 = 1;
        LATCbits.LATC2 = 0;
        LATEbits.LATE2 = 0;
        __delay_ms(2);
        LATCbits.LATC0 = 1;
        LATCbits.LATC1 = 0;
        LATCbits.LATC2 = 0;
        LATEbits.LATE2 = 0;
        __delay_ms(2);
        LATCbits.LATC0 = 0;
        LATCbits.LATC1 = 0;
        LATCbits.LATC2 = 0;
        LATEbits.LATE2 = 1;
        __delay_ms(2);
    }
    
    LATCbits.LATC0 = 0;
    LATCbits.LATC1 = 0;
    LATCbits.LATC2 = 0;
    LATEbits.LATE2 = 0;
}


/*Controls the main piece distribution*/
void distributePieces(unsigned int roundPieces, unsigned int flatPieces, unsigned int longPieces){
    switch(roundPieces){
        case 1:
            distributeRound(1);
            __delay_ms(2000);
            break;
        case 2:
            distributeRound(2);
            __delay_ms(2000);
            break;
        default:
            break;
    }
    switch(flatPieces){
        case 1:
            distributeFlat(1);
            __delay_ms(2000);
            break;
        case 2:
            distributeFlat(2);
            __delay_ms(2000);
            break;
        default:
            break;
    }
    switch(longPieces){
        case 1:
            distributeLong(1);
            __delay_ms(2000);
            break;
        case 2:
            distributeLong(2);
            __delay_ms(2000);
            break;
        case 3:
            distributeLong(3);
            __delay_ms(2000);
            break;
        default:
            break;
    }
}


/*Counts the number of pieces distributed (through contact sensor), by food type*/
unsigned int checkCount(unsigned int pieceType){
    /*Function will take in food type, and count the number of pieces distributed*/
    return 1;
}


/*This function will release the pieces into the reservoirs*/
void trapDoor(unsigned int direction){
    /*"type" specifies which food type is to be released*/
    /*"direction" specifies which direction the trapDoor needs to turn*/
    /*This function will return 1 when finished*/
    
    unsigned int i = 0;
    
    /*Open trap door*/
    if (direction == 1){
        LATAbits.LATA4 = 1;
        LATAbits.LATA5 = 0;
        LATEbits.LATE1 = 1;
        __delay_ms(500);
    }
    
    /*Close trap door*/
    else if (direction == 1){
        LATAbits.LATA4 = 0;
        LATAbits.LATA5 = 1;
        LATEbits.LATE1 = 1;
        __delay_ms(500);
    }
    
    LATAbits.LATA4 = 0;
    LATAbits.LATA5 = 0;
    LATEbits.LATE1 = 0;
}

/*This function will be called in main.c, and holds the code for the entire operation*/
void mainOperation(void){
    unsigned int i = 0;
    unsigned int currentDrawer = 0;
    unsigned int temp = 0;
    unsigned int nextDrawer = 0;
    unsigned int trueValue = 0;
    unsigned int roundPieces = 0;
    unsigned int flatPieces = 0;
    unsigned int longPieces = 0;
    
    unsigned int drawerTable[8] = {0,0,0,0,0,0,0,0};
    unsigned int sortedDrawerTable[8] = {0,0,0,0,0,0,0,0};
    unsigned int dietTable[8] = {0,0,0,0,0,0,0,0};
    unsigned int foodTable[8] = {0,0,0,0,0,0,0,0};
    
    /*Call function to initialize arm*/
    currentDrawer = 1;
    
    getUserInputs(drawerTable, dietTable, foodTable);
    sortDrawerList(drawerTable, sortedDrawerTable);
    
    for (i=0; i<8; i++){
        if (sortedDrawerTable[i] != 0){
            currentDrawer = sortedDrawerTable[i];
            nextDrawer = sortedDrawerTable[i+1];
            
            if (currentDrawer != 1 && i == 0){
                __lcd_clear();
                printf("Moving to");
                __lcd_newline();
                printf("drawer %i", sortedDrawerTable[i]);
                __delay_ms(2000);
                temp = moveVertically(1, sortedDrawerTable[i]);
                moveHorizontally(1, sortedDrawerTable[i]);
                currentDrawer = sortedDrawerTable[i];
            }
            
            uartTransmitBlocking(drawerDown, 1);
            __delay_ms(1000);
            configDrawer(0);
            __delay_ms(1000);
            uartTransmitBlocking(drawerUp, 1);
            __delay_ms(1000);
            configDrawer(1);
            __delay_ms(1000);
            
            trueValue = findDrawerIndex(drawerTable, sortedDrawerTable[i]);
            __lcd_clear();
            printf("Diet: %s", inputs[dietTable[trueValue]]);
            __lcd_newline();
            printf("Food: %s", foodInputs[foodTable[trueValue]]);
            __delay_ms(2000);
            roundPieces = getRoundPieces(dietTable[trueValue], foodTable[trueValue]);
            flatPieces = getFlatPieces(dietTable[trueValue], foodTable[trueValue]);
            longPieces = getLongPieces(dietTable[trueValue], foodTable[trueValue]);
            __lcd_clear();
            printf("R:%i F:%i L:%i", roundPieces, flatPieces, longPieces);
            __delay_ms(2000);
            distributePieces(roundPieces, flatPieces, longPieces);
            __delay_ms(5000);
            
            configDrawer(0);
            __delay_ms(1000);
            uartTransmitBlocking(drawerDown, 1);
            __delay_ms(1000);
            configDrawer(1);
            __delay_ms(1000);
            uartTransmitBlocking(drawerUp, 1);
            __delay_ms(1000);
            
            __lcd_clear();
            printf("Moving to");
            __lcd_newline();
            printf("drawer %i", sortedDrawerTable[i+1]);
            temp = moveVertically(currentDrawer, nextDrawer);
            currentDrawer = temp;
            moveHorizontally(currentDrawer, nextDrawer);
            __delay_ms(2000);
        }
    }
    initArm(currentDrawer);
    __lcd_clear();
    printf("Initializing");
    __lcd_newline();
    printf("Arm");
    __delay_ms(2000);
    
    
    trapDoor(1);
    __lcd_clear();
    printf("Distributing to");
    __lcd_newline();
    printf("reservoir.");
    __delay_ms(2000);
    trapDoor(0);
    
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
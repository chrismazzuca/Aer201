/*
 * File:   standbyMode.c
 * Author: chrismazzuca
 *
 * Created on February 14, 2018, 4:07 PM
 */

#include "standbyMode.h"
#include "EEPROMstorage.h"
#include "I2C.h"
#include "RTC.h"
#include "arduinoSetup.h"

const char input[] = "123R456F789L*0#D";
const char keys[] = "123A456B789C*0#D";

void lcdInst(char data){
    /* Sends a command to a display control register.
     * 
     * Arguments: data, the command byte for the Hitachi controller
     * 
     * Returns: none
     */
    
    RS = 0;
    lcdNibble(data);
    __delay_us(100);
}


void initLCD(void){
    /* Initializes the character LCD.
     *
     * Arguments: none
     *
     * Returns: none
     */
    
    __delay_ms(15);
    lcdInst(0b00110011);
    lcdInst(0b00110010);
    lcdInst(0b00101000);
    lcdInst(0b00001111);
    lcdInst(0b00000110);
    __lcd_clear();
    
    /* Enforce on: display, cursor, and cursor blinking. */
    __lcd_display_control(1, 1, 1);
}


void lcdNibble(char data){
    /* Low-level byte-sending implementation.
     * 
     * Arguments: data, the byte to be sent
     * 
     * Returns: none
     */
    
    char temp = (unsigned char) (data & 0xF0);
    LATD = (unsigned char) (LATD & 0x0F);
    LATD = (unsigned char) (temp | LATD);

    __PULSE_E();
    
    /* Send the 4 most significant bits (MSb). */
    data = (unsigned char) (data << 4);
    temp = (unsigned char) (data & 0xF0);
    LATD = (unsigned char) (LATD & 0x0F);
    LATD = (unsigned char) (temp | LATD);

    __PULSE_E();
}


void putch(char data){
    /* Sends a character to the display RAM for printing.
     * 
     * Arguments data, the character (byte) to be sent
     * 
     * Returns: none
     */
    
    RS = 1;
    lcdNibble(data);
    __delay_us(100);
}


unsigned int check_food(unsigned int sum, unsigned int foodInput[4]){
    unsigned int returnValue = 0; /*Used to return if input is valid or not*/
    
    switch(sum){
        /*Diet R*/
        case 3:
            if (foodInput[0] == 1){
                returnValue = 1;
            }
            else if (foodInput[0] == 2){
                returnValue = 2;
            }
            break;
            
        /*Diet F*/
        case 7:
            if (foodInput[0] == 1){
                returnValue = 3;
            }
            else if (foodInput[0] == 2){
                returnValue = 4;
            }
            break;
            
        /*Diet L*/
        case 12:
            if (foodInput[0] == 1){
                returnValue = 5;
            }
            else if (foodInput[0] == 2){
                returnValue = 6;
            }
            else if (foodInput[0] == 3){
                returnValue = 7;
            }
            break;
            
        /*Diet RF*/
        case 10:
            if (foodInput[0] == 1 && foodInput[1] == 1){
                returnValue = 8;
            }
            else if (foodInput[0] == 1 && foodInput[1] == 2){
                returnValue = 9;
            }
            else if (foodInput[0] == 2 && foodInput[1] == 1){
                returnValue = 10;
            }
            else if (foodInput[0] == 2 && foodInput[1] == 2){
                returnValue = 11;
            }
            break;
            
        /*Diet RL*/
        case 15:
            if (foodInput[0] == 1 && foodInput[1] == 1){
                returnValue = 12;
            }
            else if (foodInput[0] == 1 && foodInput[1] == 2){
                returnValue = 13;
            }
            else if (foodInput[0] == 1 && foodInput[1] == 3){
                returnValue = 14;
            }
            else if (foodInput[0] == 2 && foodInput[1] == 1){
                returnValue = 15;
            }
            else if (foodInput[0] == 2 && foodInput[1] == 2){
                returnValue = 16;
            }
            break;
            
        /*Diet FL*/
        case 19:
            if (foodInput[0] == 1 && foodInput[1] == 1){
                returnValue = 17;
            }
            else if (foodInput[0] == 1 && foodInput[1] == 2){
                returnValue = 18;
            }
            else if (foodInput[0] == 1 && foodInput[1] == 3){
                returnValue = 19;
            }
            else if (foodInput[0] == 2 && foodInput[1] == 1){
                returnValue = 20;
            }
            else if (foodInput[0] == 2 && foodInput[1] == 2){
                returnValue = 21;
            }
            break;
        
        /*Diet RRF*/
        case 13:
            if (foodInput[0] == 1 && foodInput[1] == 1 && foodInput[2] == 1){
                returnValue = 22;
            }
            else if (foodInput[0] == 1 && foodInput[1] == 1 && foodInput[2] == 2){
                returnValue = 23;
            }
            break;
            
        /*Diet RRL*/
        case 18:
            if (foodInput[0] == 1 && foodInput[1] == 1 && foodInput[2] == 1){
                returnValue = 24;
            }
            else if (foodInput[0] == 1 && foodInput[1] == 1 && foodInput[2] == 2){
                returnValue = 25;
            }
            break;
            
        /*Diet RFF*/
        case 17:
            if (foodInput[0] == 1 && foodInput[1] == 1 && foodInput[2] == 1){
                returnValue = 26;
            }
            else if (foodInput[0] == 2 && foodInput[1] == 1 && foodInput[2] == 1){
                returnValue = 27;
            }
            break;
            
        /*Diet RLL*/
        case 27:
            if (foodInput[0] == 1 && foodInput[1] == 1 && foodInput[2] == 1){
                returnValue = 28;
            }
            else if (foodInput[0] == 1 && foodInput[1] == 1 && foodInput[2] == 2){
                returnValue = 29;
            }
            else if (foodInput[0] == 1 && foodInput[1] == 2 && foodInput[2] == 1){
                returnValue = 30;
            }
            else if (foodInput[0] == 2 && foodInput[1] == 1 && foodInput[2] == 1){
                returnValue = 31;
            }
            break;
            
        /*Diet RFL*/
        case 22:
            if (foodInput[0] == 1 && foodInput[1] == 1 && foodInput[2] == 1){
                returnValue = 32;
            }
            else if (foodInput[0] == 1 && foodInput[1] == 1 && foodInput[2] == 2){
                returnValue = 33;
            }
            else if (foodInput[0] == 1 && foodInput[1] == 2 && foodInput[2] == 1){
                returnValue = 34;
            }
            else if (foodInput[0] == 2 && foodInput[1] == 1 && foodInput[2] == 1){
                returnValue = 35;
            }
            break;
            
        /*Diet FFL*/
        case 26:
            if (foodInput[0] == 1 && foodInput[1] == 1 && foodInput[2] == 1){
                returnValue = 36;
            }
            else if (foodInput[0] == 1 && foodInput[1] == 1 && foodInput[2] == 2){
                returnValue = 37;
            }
            break;
            
        /*Diet FLL*/
        case 31:
            if (foodInput[0] == 1 && foodInput[1] == 1 && foodInput[2] == 1){
                returnValue = 38;
            }
            else if (foodInput[0] == 1 && foodInput[1] == 1 && foodInput[2] == 2){
                returnValue = 39;
            }
            else if (foodInput[0] == 1 && foodInput[1] == 2 && foodInput[2] == 1){
                returnValue = 40;
            }
            else if (foodInput[0] == 2 && foodInput[1] == 1 && foodInput[2] == 1){
                returnValue = 41;
            }
            break;
            
        /*Diet RRFL*/
        case 25:
            if (foodInput[0] == 1 && foodInput[1] == 1 && foodInput[2] == 1 && foodInput[3] == 1){
                returnValue = 42;
            }
            break;
            
        /*Diet RFFL*/
        case 29:
            if (foodInput[0] == 1 && foodInput[1] == 1 && foodInput[2] == 1 && foodInput[3] == 1){
                returnValue = 43;
            }
            break;
            
        /*Diet RFLL*/
        case 34:
            if (foodInput[0] == 1 && foodInput[1] == 1 && foodInput[2] == 1 && foodInput[3] == 1){
                returnValue = 44;
            }
            break;
            
        /*Diet RLLL*/
        case 39:
            if (foodInput[0] == 1 && foodInput[1] == 1 && foodInput[2] == 1 && foodInput[3] == 1){
                returnValue = 45;
            }
            break;
            
        /*Diet FLLL*/
        case 43:
            if (foodInput[0] == 1 && foodInput[1] == 1 && foodInput[2] == 1 && foodInput[3] == 1){
                returnValue = 46;
            }
            break;
            
        /* # Button pressed (no food for this drawer)*/
        default:
            break;
    }
    return returnValue;
}


unsigned int getRound(unsigned int code){
    unsigned int returnValue = 0;
    switch(code){
        /*Diet R1*/
        case 1:
            returnValue = 1;
            break;
            
        /*Diet R2*/
        case 2:
            returnValue = 2;
            break;
            
        /*Diet RF11*/
        case 8:
            returnValue = 1;
            break;
            
        /*Diet RF12*/
        case 9:
            returnValue = 1;
            break;
            
        /*Diet RF21*/
        case 10:
            returnValue = 2;
            break;
            
        /*Diet RF22*/
        case 11:
            returnValue = 2;
            break;
            
        /*Diet RL11*/
        case 12:
            returnValue = 1;
            break;
            
        /*Diet RL12*/
        case 13:
            returnValue = 1;
            break;
            
        /*Diet RL13*/
        case 14:
            returnValue = 1;
            break;
            
        /*Diet RL21*/
        case 15:
            returnValue = 2;
            break;
            
        /*Diet RL22*/
        case 16:
            returnValue = 2;
            break;
            
        /*Diet RRF111*/
        case 22:
            returnValue = 2;
            break;
            
        /*Diet RRF112*/
        case 23:
            returnValue = 2;
            break;
            
        /*Diet RRL111*/
        case 24:
            returnValue = 2;
            break;
            
        /*Diet RRL112*/
        case 25:
            returnValue = 2;
            break;
            
        /*Diet RFF111*/
        case 26:
            returnValue = 1;
            break;
            
        /*Diet RFF211*/
        case 27:
            returnValue = 2;
            break;
            
        /*Diet RLL111*/
        case 28:
            returnValue = 1;
            break;
            
        /*Diet RLL112*/
        case 29:
            returnValue = 1;
            break;
            
        /*Diet RLL121*/
        case 30:
            returnValue = 1;
            break;
            
        /*Diet RLL211*/
        case 31:
            returnValue = 2;
            break;
            
        /*Diet RFL111*/
        case 32:
            returnValue = 1;
            break;
            
        /*Diet RFL112*/
        case 33:
            returnValue = 1;
            break;
            
        /*Diet RFL121*/
        case 34:
            returnValue = 1;
            break;
            
        /*Diet RFL211*/
        case 35:
            returnValue = 2;
            break;
            
        /*Diet RRFL1111*/
        case 42:
            returnValue = 2;
            break;
        
        /*Diet RFFL1111*/
        case 43:
            returnValue = 1;
            break;
            
        /*Diet RFLL1111*/
        case 44:
            returnValue = 1;
            break;
            
        /*Diet RLLL1111*/
        case 45:
            returnValue = 1;
            break;
            
        default:
            returnValue = 0;
            break;
    }
    return returnValue;
}


unsigned int getFlat(unsigned int code){
    unsigned int returnValue = 0;
    
    switch(code){
        /*Diet F1*/
        case 3:
            returnValue = 1;
            break;
            
        /*Diet F2*/
        case 4:
            returnValue = 2;
            break;
            
        /*Diet RF11*/
        case 8:
            returnValue = 1;
            break;
            
        /*Diet RF12*/
        case 9:
            returnValue = 2;
            break;
            
        /*Diet RF21*/
        case 10:
            returnValue = 1;
            break;
            
        /*Diet RF22*/
        case 11:
            returnValue = 2;
            break;
            
        /*Diet FL11*/
        case 17:
            returnValue = 1;
            break;
            
        /*Diet FL12*/
        case 18:
            returnValue = 1;
            break;
            
        /*Diet FL13*/
        case 19:
            returnValue = 1;
            break;
            
        /*Diet FL21*/
        case 20:
            returnValue = 2;
            break;
            
        /*Diet FL22*/
        case 21:
            returnValue = 2;
            break;
            
        /*Diet RRF111*/
        case 22:
            returnValue = 1;
            break;
            
        /*Diet RRF112*/
        case 23:
            returnValue = 2;
            break;
            
        /*Diet RFF111*/
        case 26:
            returnValue = 2;
            break;
            
        /*Diet RFF211*/
        case 27:
            returnValue = 2;
            break;

        /*Diet RFL111*/
        case 32:
            returnValue = 1;
            break;
            
        /*Diet RFL112*/
        case 33:
            returnValue = 1;
            break;
            
        /*Diet RFL121*/
        case 34:
            returnValue = 2;
            break;
            
        /*Diet RFL211*/
        case 35:
            returnValue = 1;
            break;
            
        /*Diet FFL111*/
        case 36:
            returnValue = 2;
            break;
            
        /*Diet FFL112*/
        case 37:
            returnValue = 2;
            break;
            
        /*Diet FLL111*/
        case 38:
            returnValue = 1;
            break;
            
        /*Diet FLL112*/
        case 39:
            returnValue = 1;
            break;
            
        /*Diet FLL121*/
        case 40:
            returnValue = 1;
            break;
            
        /*Diet FLL211*/
        case 41:
            returnValue = 2;
            break;
            
        /*Diet RRFL1111*/
        case 42:
            returnValue = 1;
            break;
        
        /*Diet RFFL1111*/
        case 43:
            returnValue = 2;
            break;
            
        /*Diet RFLL1111*/
        case 44:
            returnValue = 1;
            break;

        /*Diet FLLL1111*/
        case 46:
            returnValue = 1;
            break;
            
        default:
            returnValue = 0;
            break;
    }
    return returnValue;
}


unsigned int getLong(unsigned int code){
    unsigned int returnValue = 0;
    
    switch(code){
        /*Diet L1*/
        case 5:
            returnValue = 1;
            break;
            
        /*Diet L2*/
        case 6:
            returnValue = 2;
            break;
            
        /*Diet L3*/
        case 7:
            returnValue = 3;
            break;
            
        /*Diet RL11*/
        case 12:
            returnValue = 1;
            break;
            
        /*Diet RL12*/
        case 13:
            returnValue = 2;
            break;
            
        /*Diet RL13*/
        case 14:
            returnValue = 3;
            break;
            
        /*Diet RL21*/
        case 15:
            returnValue = 1;
            break;
            
        /*Diet RL22*/
        case 16:
            returnValue = 2;
            break;
            
        /*Diet FL11*/
        case 17:
            returnValue = 1;
            break;
            
        /*Diet FL12*/
        case 18:
            returnValue = 2;
            break;
            
        /*Diet FL13*/
        case 19:
            returnValue = 3;
            break;
            
        /*Diet FL21*/
        case 20:
            returnValue = 1;
            break;
            
        /*Diet FL22*/
        case 21:
            returnValue = 2;
            break;
            
        /*Diet RRL111*/
        case 24:
            returnValue = 1;
            break;
            
        /*Diet RRL112*/
        case 25:
            returnValue = 2;
            break;
            
        /*Diet RLL111*/
        case 28:
            returnValue = 2;
            break;
            
        /*Diet RLL112*/
        case 29:
            returnValue = 3;
            break;
            
        /*Diet RLL121*/
        case 30:
            returnValue = 3;
            break;
            
        /*Diet RLL211*/
        case 31:
            returnValue = 2;
            break;
            
        /*Diet RFL111*/
        case 32:
            returnValue = 1;
            break;
            
        /*Diet RFL112*/
        case 33:
            returnValue = 2;
            break;
            
        /*Diet RFL121*/
        case 34:
            returnValue = 1;
            break;
            
        /*Diet RFL211*/
        case 35:
            returnValue = 1;
            break;
            
        /*Diet FFL111*/
        case 36:
            returnValue = 1;
            break;
            
        /*Diet FFL112*/
        case 37:
            returnValue = 2;
            break;
            
        /*Diet FLL111*/
        case 38:
            returnValue = 2;
            break;
            
        /*Diet FLL112*/
        case 39:
            returnValue = 3;
            break;
            
        /*Diet FLL121*/
        case 40:
            returnValue = 3;
            break;
            
        /*Diet FLL211*/
        case 41:
            returnValue = 2;
            break;
            
        /*Diet RRFL1111*/
        case 42:
            returnValue = 1;
            break;
        
        /*Diet RFFL1111*/
        case 43:
            returnValue = 1;
            break;
            
        /*Diet RFLL1111*/
        case 44:
            returnValue = 2;
            break;
            
        /*Diet RLLL1111*/
        case 45:
            returnValue = 3;
            break;
            
        /*Diet FLLL1111*/
        case 46:
            returnValue = 3;
            break;
            
        default:
            returnValue = 0;
            break;
    }
    return returnValue;
}


/*Sub function used to verify keypress in viewLogs()*/
unsigned int verifyKeypress(unsigned char logNumber, unsigned int keypress){
    unsigned int i = 0;
    unsigned int returnVal = 0;
    
    for (i=0; i<logNumber; i++){
        if (keypress == i){
            returnVal = 1;
        }
    }
    return returnVal;
}


/*Main function called to view logs*/
unsigned int viewLogs(void){
    
    unsigned char logNumber = readEEPROM(250);
    unsigned int logView = 0;
    unsigned int checkLog = 0;
    unsigned int returnVal = 0;
    
    if (logNumber == 0){
        __lcd_clear();
        printf("No logs");
        __lcd_newline();
        printf("available.");
        __delay_ms(2000);
        return 14;
    }
    else {
        if (logNumber == 1){
            logView = 1;
            
            __lcd_clear();
            printf("1 previous log");
            __lcd_newline();
            printf("available.");
            __delay_ms(2000);

            __lcd_clear();
            printf("Press any key to");
            __lcd_newline();
            printf("move forward.");
            __delay_ms(500);
            
            while(PORTBbits.RB1 == 0){  
                continue;   
            }
        }
        else {
            __lcd_clear();
            printf("%i previous logs", logNumber);
            __lcd_newline();
            printf("available.");
            __delay_ms(2000);
            __lcd_clear();
            printf("Please input log");
            __lcd_newline();
            printf("number: ");
        
            while (1){
                while(PORTBbits.RB1 == 0){  continue;   }
                unsigned char keypress2 = (PORTB & 0xF0) >> 4;
                while(PORTBbits.RB1 == 1){  continue;   }
            
                if (keypress2 == 0 || keypress2 == 1 || keypress2 == 2 || keypress2 == 4){
                    checkLog = verifyKeypress(logNumber, keypress2);
                    if (checkLog == 1){
                        putch(keys[keypress2]);
                        if (keypress2 == 4){
                            logView = keypress2;
                        }
                        else{
                            logView = keypress2 + 1;
                        }
                        __delay_ms(1000);
                        break;
                    }
                }
            }
        
            __lcd_clear();
            printf("Great!");
            __delay_ms(1000);
            __lcd_clear();
            printf("Press any key to");
            __lcd_newline();
            printf("move forward.");
            __delay_ms(500);
            
            while(PORTBbits.RB1 == 0){  
                continue;   
            }
        }
        returnVal = displayLogs(logView);
        return returnVal;
    }
}


/*Takes in the log number, and displays information gathered from EEPROM*/
unsigned int displayLogs(unsigned int logNumber){
    unsigned int n = (logNumber-1)*51;
    unsigned int year = readEEPROM(n);
    unsigned int month = readEEPROM(1+n);
    unsigned int day = readEEPROM(2+n);
    unsigned int operationTime = readEEPROM(3+n);
    unsigned int roundPieces = readEEPROM(4+n);
    unsigned int flatPieces = readEEPROM(5+n);
    unsigned int longPieces = readEEPROM(6+n);
    unsigned int markedDrawer1 = readEEPROM(7+n);
    unsigned int markedDrawer2 = readEEPROM(8+n);
    unsigned int markedDrawer3 = readEEPROM(9+n);
    unsigned int markedDrawer4 = readEEPROM(10+n);
    
        __lcd_clear();
        printf("Date:");
        __lcd_newline();
        printf("%02i/%02i/%02i", year, month, day);
        __delay_ms(500);
        while(PORTBbits.RB1 == 0){  
            continue;   
        }
    
        unsigned int minutes = operationTime/60;
        unsigned int seconds = operationTime%60;
        __lcd_clear();
        printf("Operation time:");
        __lcd_newline();
        printf("%i:%02i", minutes, seconds);
        __delay_ms(500);
        
        while(PORTBbits.RB1 == 0){  
            continue;   
        }
        
        __lcd_clear();
        printf("Marked drawers:");
        __lcd_newline();
        
        if (markedDrawer4 == 0 && markedDrawer3 == 0 && markedDrawer2 == 0){
            printf("%i", markedDrawer1);
            __delay_ms(500);
        }
        
        else if (markedDrawer4 == 0 && markedDrawer3 == 0){
            printf("%i, %i", markedDrawer1, markedDrawer2);
            __delay_ms(500);
        }
        
        else if (markedDrawer4 == 0){
            printf("%i, %i, %i", markedDrawer1, markedDrawer2, markedDrawer3);
            __delay_ms(500);
        }
        
        else {
            printf("%i, %i, %i, %i", markedDrawer1, markedDrawer2, markedDrawer3, markedDrawer4);
            __delay_ms(500);
        }
        
        while(PORTBbits.RB1 == 0){  
            continue;   
        }
        
        __lcd_clear();
        printf("Remaining pieces:");
        __lcd_newline();
        printf("R:%i  F:%i  L:%i", roundPieces, flatPieces, longPieces);
        __delay_ms(500);
        
        while(PORTBbits.RB1 == 0){  
            continue;   
        }
        
        __lcd_clear();
        printf("Input Summary:");
        __delay_ms(500);
        
        while(PORTBbits.RB1 == 0){  
            continue;   
        }
        
        __lcd_clear();
        printf("Drawer 1: RF12");
        __lcd_newline();
        printf("Drawer 2: L1");
        __delay_ms(2000);
        
        __lcd_clear();
        printf("Another log?");
        __lcd_newline();
        printf("# = No, D = Yes");
            
        while(PORTBbits.RB1 == 0){  continue;   }
        unsigned char keypress3 = (PORTB & 0xF0) >> 4;
        while(PORTBbits.RB1 == 1){  continue;   }
        return keypress3;
}


/*Used to get the number of digits needed in the food input.*/
unsigned int get_digits(unsigned int sum){
    
    unsigned int count = 0; /*Used to count the digits needed.*/
    
    switch(sum){
        /*Diet R*/
        case 3:
            count = 1;
            break;
            
        /*Diet F*/
        case 7:
            count = 1;
            break;
            
        /*Diet L*/
        case 12:
            count = 1;
            break;
            
        /*Diet RF*/
        case 10:
            count = 2;
            break;
            
        /*Diet RL*/
        case 15:
            count = 2;
            break;
            
        /*Diet FL*/
        case 19:
            count = 2;
            break;
        
        /*Diet RRF*/
        case 13:
            count = 3;
            break;
            
        /*Diet RRL*/
        case 18:
            count = 3;
            break;
            
        /*Diet RFF*/
        case 17:
            count = 3;
            break;
            
        /*Diet RLL*/
        case 27:
            count = 3;
            break;
            
        /*Diet RFL*/
        case 22:
            count = 3;
            break;
            
        /*Diet FFL*/
        case 26:
            count = 3;
            break;
            
        /*Diet FLL*/
        case 31:
            count = 3;
            break;
            
        /*Diet RRFL*/
        case 25:
            count = 4;
            break;
            
        /*Diet RFFL*/
        case 29:
            count = 4;
            break;
            
        /*Diet RFLL*/
        case 34:
            count = 4;
            break;
            
        /*Diet RLLL*/
        case 39:
            count = 4;
            break;
            
        /*Diet FLLL*/
        case 43:
            count = 4;
            break;
            
        /* # Button pressed (no food for this drawer)*/
        default:
            count = 0;
            break;
    }
    return count;
}


/*Function checks food inputs, and stores in EEPROM*/
unsigned int food_press(unsigned int dietSum, unsigned int drawer){
    
    START: __lcd_clear();
    printf("Drawer %i food:", drawer);
    __lcd_newline();
    
    unsigned int tally = 0; /*Used to count how many digits needed*/
    unsigned int count = 0; /*Used to count how many current digits*/
    unsigned int foodTable[4] = {0, 0, 0, 0}; /*Used to store and check diet type*/
    unsigned int isValid = 0; /*Used to determine if input is valid*/
    unsigned int backCheck = 0; /*Checks to see if back button is pressed*/
    tally = get_digits(dietSum);
    
    while(1){
        while(PORTBbits.RB1 == 0){  continue;   }
        unsigned char keypress = (PORTB & 0xF0) >> 4;
        while(PORTBbits.RB1 == 1){  continue;   }
        
        if(keypress == 15){
            break;
        }
        else if (keypress == 14){
            backCheck = 1;
            break;
        }
        else if (keypress == 0 || keypress == 1 || keypress == 2){
            if (count < tally){
                putch(input[keypress]);
                foodTable[count] = keypress + 1;
                count += 1;
            }
        }
        else if (keypress == 12){
            __lcd_clear();
            printf("Drawer %i food:", drawer);
            __lcd_newline();
            count = 0;
            foodTable[0] = 0;
            foodTable[1] = 0;
            foodTable[2] = 0;
            foodTable[3] = 0;
        }
    }
    
    if (backCheck == 1){
        return 100;
    }
    else {
        isValid = check_food(dietSum, foodTable);
    
        if (isValid == 0){
            __lcd_clear();
            printf("Invalid food!");
            __lcd_newline();
            printf("Try again.");
            __delay_ms(1000);
            goto START;
        }
        else {
            return isValid; //Will be changed later
        }
    }
}


/*Function checks the diet type, displays on LCD, and stores in EEPROM*/
unsigned int diet_press(unsigned int drawer){
    /*R=3, F=7, L=12, RF=10, RL=15, FL=19, RRF=13, RRL=18, RFF=17, RLL=27, RFL=22, FFL=26, FLL=31, RRFL=25, RFFL=29, RFLL=34, RLLL=39, FLLL=43*/
    
    START: __lcd_clear();
    printf("Drawer %i diet:", drawer);
    __lcd_newline();
    
    unsigned int count = 0; /*Used to count number of letters in diet*/
    unsigned int previous = 0; /*Keep track of previous button*/
    unsigned int sum = 0; /*Used to count sum of diet input*/
    unsigned int i = 0; /*Random variable for the for loop*/
    unsigned int dietTable[4] = {0, 0, 0, 0}; /*Used to store and check diet type*/
    unsigned int backCheck = 0; /*Used to check if back button is pressed*/
    
    while(1){
        while(PORTBbits.RB1 == 0){  continue;   }
        unsigned char keypress = (PORTB & 0xF0) >> 4;
        while(PORTBbits.RB1 == 1){  continue;   }
        
        if(keypress == 15){
            break;
        }
        else if (keypress == 14){
            backCheck = 1;
            break;
        }
        else if (keypress == 12){
            __lcd_clear();
            printf("Drawer %i diet:", drawer);
            __lcd_newline();
            count = 0;
            dietTable[0] = 0;
            dietTable[1] = 0;
            dietTable[2] = 0;
            dietTable[3] = 0;
        }
        else if (count <= 3){
            if (keypress == 3 || keypress == 7){
                putch(input[keypress]);
                dietTable[count] = keypress;
                count += 1;
            }
            else if (keypress == 11){
                putch(input[keypress]);
                dietTable[count] = keypress + 1;
                count += 1;
            }
        }
    }
    
    if (backCheck == 1){
        sum = 100;
    }
    
    /*L (12)*/
    else if (dietTable[0] == 12 && dietTable[1] == 0 && dietTable[2] == 0 && dietTable[3] == 0){
        sum = 12;
    }
    
    /*R (3)*/
    else if (dietTable[0] == 3 && dietTable[1] == 0 && dietTable[2] == 0 && dietTable[3] == 0){
        sum = 3;
    }
    
    /*F (7)*/
    else if (dietTable[0] == 7 && dietTable[1] == 0 && dietTable[2] == 0 && dietTable[3] == 0){
        sum = 7;
    }
    
    /*RF (10)*/
    else if (dietTable[0] == 3 && dietTable[1] == 7 && dietTable[2] == 0 && dietTable[3] == 0){
        sum = 10;
    }
    
    /*RL (15)*/
    else if (dietTable[0] == 3 && dietTable[1] == 12 && dietTable[2] == 0 && dietTable[3] == 0){
        sum = 15;
    }
    
    /*FL (19)*/
    else if (dietTable[0] == 7 && dietTable[1] == 12 && dietTable[2] == 0 && dietTable[3] == 0){
        sum = 19;
    }
    
    /*RRF (13)*/
    else if (dietTable[0] == 3 && dietTable[1] == 3 && dietTable[2] == 7 && dietTable[3] == 0){
        sum = 13;
    }
    
    /*RRL (18)*/
    else if (dietTable[0] == 3 && dietTable[1] == 3 && dietTable[2] == 12 && dietTable[3] == 0){
        sum = 18;
    }
    
    /*RFF (17)*/
    else if (dietTable[0] == 3 && dietTable[1] == 7 && dietTable[2] == 7 && dietTable[3] == 0){
        sum = 17;
    }
    
    /*RLL (27)*/
    else if (dietTable[0] == 3 && dietTable[1] == 12 && dietTable[2] == 12 && dietTable[3] == 0){
        sum = 27;
    }
    
    /*RFL (22)*/
    else if (dietTable[0] == 3 && dietTable[1] == 7 && dietTable[2] == 12 && dietTable[3] == 0){
        sum = 22;
    }
    
    /*FFL (26)*/
    else if (dietTable[0] == 7 && dietTable[1] == 7 && dietTable[2] == 12 && dietTable[3] == 0){
        sum = 26;
    }
    
    /*FLL (31)*/
    else if (dietTable[0] == 7 && dietTable[1] == 12 && dietTable[2] == 12 && dietTable[3] == 0){
        sum = 31;
    }
    
    /*RRFL (25)*/
    else if (dietTable[0] == 3 && dietTable[1] == 3 && dietTable[2] == 7 && dietTable[3] == 12){
        sum = 25;
    }
    
    /*RFFL (29)*/
    else if (dietTable[0] == 3 && dietTable[1] == 7 && dietTable[2] == 7 && dietTable[3] == 12){
        sum = 29;
    }
    
    /*RFLL (34)*/
    else if (dietTable[0] == 3 && dietTable[1] == 7 && dietTable[2] == 12 && dietTable[3] == 12){
        sum = 34;
    }
    
    /*RLLL (39)*/
    else if (dietTable[0] == 3 && dietTable[1] == 12 && dietTable[2] == 12 && dietTable[3] == 12){
        sum = 39;
    }
    
    /*FLLL (43)*/
    else if (dietTable[0] == 7 && dietTable[1] == 12 && dietTable[2] == 12 && dietTable[3] == 12){
        sum = 43;
    }
    
    /*Default*/
    else{
        __lcd_clear();
        printf("Invalid diet!");
        __lcd_newline();
        printf("Try again.");
        __delay_ms(1000);
        sum = 0;
        goto START;
    }
    return sum;
}


/*Decodes the drawer number from the which_drawer() function*/
unsigned int decode_drawer(unsigned int drawer){
    unsigned int returnValue;
    
    switch(drawer){
        /*Drawer 1*/
        case 20:
            returnValue = 1;
            break;
            
        /*Drawer 2*/
        case 1:
            returnValue = 2;
            break;
            
        /*Drawer 3*/
        case 2:
            returnValue = 3;
            break;
            
        /*Drawer 7*/
        case 8:
            returnValue = 7;
            break;
            
        /*Drawer 8*/
        case 9:
            returnValue = 8;
            break;
            
        /*Drawer 9*/
        case 10:
            returnValue = 9;
            break;
            
        /*Drawer 10*/
        case 33:
            returnValue = 10;
            break;
            
        /*Drawer 11*/
        case 40:
            returnValue = 11;
            break;
            
        /*Drawer 12*/
        case 21:
            returnValue = 12;
            break;
            
        /*Drawer 13*/
        case 22:
            returnValue = 13;
            break;
            
        /*Drawer 14*/
        case 24:
            returnValue = 14;
            break;
            
        /*Drawer 15*/
        case 25:
            returnValue = 15;
            break;
            
        /*Drawer 16*/
        case 26:
            returnValue = 16;
            break;
                    
        default:
            returnValue = drawer;
            break;
    }
    return returnValue;
}


/*Function determines which drawer is being filled.*/
unsigned int which_drawer(void){
    __lcd_display_control(1, 0, 0);
    __lcd_clear();
    printf("Drawer number:");
    __lcd_newline();
    
    unsigned int count = 0; /*Counts how many digits*/
    unsigned int sum = 0; /*Keeps track of the sum of the number*/
    unsigned int backCheck = 0; /*Keeps track of the back button*/
    
    while(1){
        while(PORTBbits.RB1 == 0){  continue;   }
        unsigned char keypress = (PORTB & 0xF0) >> 4;
        while(PORTBbits.RB1 == 1){  continue;   }
        
        if(keypress == 15){
            break;
        }
        else if (keypress == 14){
            backCheck = 100;
            break;
        }
        else if(keypress == 0 || keypress == 1 || keypress == 2 || keypress == 4 || keypress == 5 || keypress == 6 || keypress == 8 || keypress == 9 || keypress == 10 || keypress == 13){
            if (count == 0){
                if (keypress == 1 || keypress == 2 || keypress == 4 || keypress == 5 || keypress == 6 || keypress == 8 || keypress == 9 || keypress == 10){
                    count += 2;
                    putch(input[keypress]);
                    sum += keypress;
                }
                else if (keypress == 0){
                    putch(input[keypress]);
                    count += 1;
                    sum += 20;
                }
            }
            else if (count == 1){
                if (keypress < 8 || keypress == 13){
                    putch(input[keypress]);
                    count += 1;
                    if(keypress == 0){
                        sum += 20;
                    }
                    else {
                        sum += keypress;
                    }
                }
            }
        }
        else if (keypress == 12){
            __lcd_clear();
            printf("Drawer number:");
            __lcd_newline();
            count = 0;
            sum = 0;
        }
    }
    
    if (backCheck == 100){
        return backCheck;
    }
    else{
        unsigned int rand = decode_drawer(sum);
        return rand;
    }
}

unsigned int standbyMode(void){
    unsigned int drawerInformation[8] = {0,0,0,0,0,0,0,0};
    unsigned int roundPieces[8] = {0,0,0,0,0,0,0,0};
    unsigned int flatPieces[8] = {0,0,0,0,0,0,0,0};
    unsigned int longPieces[8] = {0,0,0,0,0,0,0,0};
    unsigned int dietType[8] = {0,0,0,0,0,0,0,0};
    unsigned int drawerCount = 0;      /*Keeps track of the number of drawers inputted*/
    unsigned int i = 0;                /*Random variable used in the for loop*/
    unsigned int validDrawer = 0;      /*Checks if the drawer is valid*/
    unsigned int zero = 0;             /*Checks if no drawer number was pressed*/
    unsigned int firstIteration = 0;   /*Checks to see if it is the first drawer*/
    unsigned int drawer = 0;           /*Used to determine which drawer is requested*/
    unsigned int diet = 0;             /*Used to keep track of diet*/
    unsigned int food = 0;             /*Used to keep track of the food type*/
    unsigned char logNumber = 0;
    unsigned int n = 0;
    unsigned int round = 0;
    unsigned int flat = 0;
    unsigned int lng = 0;
    
    unsigned char time[7];             /*Create a byte array to hold time read from RTC*/
    unsigned int x = 0;
    initEEPROM();
    //RTC_setTime();
    
    __lcd_clear();
    __lcd_display_control(1, 0, 0);
    printf("Standby");
    __delay_ms(2000);
    __lcd_clear();
    printf("Press any key to");
    __lcd_newline();
    printf("begin operation.");
    
    while(PORTBbits.RB1 == 0){  
        continue;   
    }
    
    /* Welcome message */
    __lcd_clear();
    __lcd_display_control(1, 0, 0);
    printf("Welcome to your");
    __lcd_newline();
    printf("cabinet sorter!");
    __delay_ms(1000);
    
    //initEEPROM();
    writeEEPROM(250, 2);
    writeEEPROM(0, 18);
    writeEEPROM(1, 2);
    writeEEPROM(2, 28);
    writeEEPROM(3, 91);
    writeEEPROM(4, 3);
    writeEEPROM(5, 15);
    writeEEPROM(6, 8);
    writeEEPROM(7, 12);
    writeEEPROM(8, 1);
    writeEEPROM(9, 13);
    writeEEPROM(10, 0);
    shiftEEPROM();
    
    /* Print current time */
    //I2C_Master_Start(); // Start condition
    //I2C_Master_Write(0b11010000); // 7 bit RTC address + Write
    //I2C_Master_Write(0x00); // Set memory pointer to seconds
    //I2C_Master_Stop(); // Stop condition

    /* Read current time. */
    I2C_Master_Init(100000); //Initialize I2C Master with 100 kHz clock
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b11010001); // 7 bit RTC address + Read
    for(x = 0; x < 6; x++){
        time[x] = I2C_Master_Read(ACK); // Read with ACK to continue reading
    }
    time[6] = I2C_Master_Read(NACK); // Final Read with NACK
    I2C_Master_Stop(); // Stop condition
    
    __lcd_clear();
    printf("Current date");
    __lcd_newline();
    printf("and time:");
    __delay_ms(2000);
    
    __lcd_clear();
    printf("%02x/%02x/%02x", time[6],time[5],time[4]); // Print date in YY/MM/DD
    __lcd_newline();
    printf("%02x:%02x:%02x", time[2],time[1],time[0]); // HH:MM:SS
    __delay_ms(2000);
    
    /*View logs, or begin operation*/
    __lcd_clear();
    printf("Press # to");
    __lcd_newline();
    printf("view logs,");
    __delay_ms(2000);
    __lcd_clear();
    printf("or press D to");
    __lcd_newline();
    printf("begin operation.");
    
    while(PORTBbits.RB1 == 0){  continue;   }
    unsigned char keypress = (PORTB & 0xF0) >> 4;
    while(PORTBbits.RB1 == 1){  continue;   }
    
    if (keypress == 15){
    
    /*__lcd_clear();
    printf("Great!");
    __delay_ms(1000);
    
    __lcd_clear();
    printf("Here are the");
    __lcd_newline();
    printf("instructions for");
    __delay_ms(2000);
    __lcd_clear();
    printf("drawer inputs...");
    __delay_ms(2000);
    
    __lcd_clear();
    printf("Press A on the");
    __lcd_newline();
    printf("keypad for R,");
    __delay_ms(3000);
    __lcd_clear();
    printf("B is for F,");
    __lcd_newline();
    printf("and C is for L.");
    __delay_ms(3000);
    
    __lcd_clear();
    printf("Press D to");
    __lcd_newline();
    printf("submit the input,");
    __delay_ms(3000);
    __lcd_clear();
    printf("press * to clear");
    __lcd_newline();
    printf("your input,");
    __delay_ms(3000);
    __lcd_clear();
    printf("and press # to");
    __lcd_newline();
    printf("go back.");
    __delay_ms(3000);*/
    
    
    while (1){
        if (firstIteration == 1){
        while (1){
                
            if(drawerCount >= 8){
                __lcd_clear();
                __lcd_display_control(1, 0, 0);
                printf("Maximum of 8");
                __lcd_newline();
                printf("drawers reached!");
                __delay_ms(2000);
                break;
            }
                
            DRAWER_BACK: __lcd_clear();
            __lcd_display_control(1, 0, 0);
            printf("Another drawer?");
            __lcd_newline();
            printf("# = No, D = Yes");
            
            while(PORTBbits.RB1 == 0){  continue;   }
            unsigned char keypress1 = (PORTB & 0xF0) >> 4;
            while(PORTBbits.RB1 == 1){  continue;   }
            
            if (keypress1 == 15){
                DIET_BACK1: __lcd_clear();
                drawer = which_drawer();
                /*Check if pressed back for drawer*/
                if (drawer == 100){
                    drawer = 0;
                    goto DRAWER_BACK;
                }
                
                for (i=0; i<8; i++){
                    if (drawerInformation[i] == drawer){
                        validDrawer = 1;
                        if (drawerInformation[i] == 0){
                            zero = 1;
                        }
                    }
                }
                if (validDrawer == 1){
                    if (zero == 1){
                        __lcd_clear();
                        __lcd_display_control(1, 0, 0);
                        printf("No drawer");
                        __lcd_newline();
                        printf("number pressed.");
                        __delay_ms(2000);
                        zero = 0;
                    }
                    else{
                        __lcd_clear();
                        __lcd_display_control(1, 0, 0);
                        printf("Drawer already");
                        __lcd_newline();
                        printf("in use!");
                        __delay_ms(2000);
                    }
                    validDrawer = 0;
                    continue;
                }
                else{
                    FOOD_BACK1: __lcd_clear();
                    diet = diet_press(drawer);
                    if (diet == 100){
                        diet = 0;
                        goto DIET_BACK1;
                    }
                    
                    food = food_press(diet, drawer);
                    if (food == 100){
                        diet = 0;
                        food = 0;
                        goto FOOD_BACK1;
                    }
                    drawerInformation[drawerCount] = drawer;
                    dietType[drawerCount] = diet;
                    round = getRound(food);
                    flat = getFlat(food);
                    lng = getLong(food);
                    roundPieces[drawerCount] = round;
                    flatPieces[drawerCount] = flat;
                    longPieces[drawerCount] = lng;
                    drawerCount += 1;
                }
            }
            else if (keypress1 == 14){
                break;
            }
            keypress1 = 0;
            }
            break;
        }
            else{
                DIET_BACK2: __lcd_clear();
                drawer = which_drawer();
                /*Check if pressed back for drawer*/
                if (drawer == 100){
                    drawer = 0;
                    firstIteration = 1;
                    goto DRAWER_BACK;
                }
                
                firstIteration = 1;
                for (i=0; i<8; i++){
                    if (drawerInformation[i] == drawer){
                        validDrawer = 1;
                        if (drawerInformation[i] == 0){
                            zero = 1;
                        }
                    }
                }
                if (validDrawer == 1){
                    if (zero == 1){
                        __lcd_clear();
                        __lcd_display_control(1, 0, 0);
                        printf("No drawer");
                        __lcd_newline();
                        printf("number pressed.");
                        __delay_ms(2000);
                        zero = 0;
                    }
                    else{
                        __lcd_clear();
                        __lcd_display_control(1, 0, 0);
                        printf("Drawer already");
                        __lcd_newline();
                        printf("in use!");
                        __delay_ms(2000);
                    }
                    validDrawer = 0;
                    continue;
                }
                else{
                    FOOD_BACK2: __lcd_clear();
                    diet = diet_press(drawer);
                    if (diet == 100){
                        diet = 0;
                        goto DIET_BACK2;
                    }
                    
                    food = food_press(diet, drawer);
                    if (food == 100){
                        diet = 0;
                        food = 0;
                        goto FOOD_BACK2;
                    }
                    drawerInformation[drawerCount] = drawer;
                    dietType[drawerCount] = diet;
                    round = getRound(food);
                    flat = getFlat(food);
                    lng = getLong(food);
                    roundPieces[drawerCount] = round;
                    flatPieces[drawerCount] = flat;
                    longPieces[drawerCount] = lng;
                    drawerCount += 1;
                }
            }
    }
    
    if (drawerCount == 0){
        return 0;
    }
    else{
        logNumber = readEEPROM(250);
        if (logNumber == 4){
            shiftEEPROM();
        }
        for (i=0; i<8; i++){
            if (logNumber == 0 || logNumber == 4){
                n = 5*i;
            }
            else{
                n = (logNumber)*51 + 5*i;
            }
            writeEEPROM(11+n, drawerInformation[i]);
            writeEEPROM(12+n, dietType[i]);
            writeEEPROM(13+n, roundPieces[i]);
            writeEEPROM(14+n, flatPieces[i]);
            writeEEPROM(15+n, longPieces[i]);
        }
        return 1;
    }
}
    else if (keypress == 14){
        while (1){
            unsigned int whichKey = viewLogs();
            if (whichKey == 15){
                continue;
            }
            else if (whichKey == 14){
                __lcd_clear();
                __lcd_display_control(1, 0, 0);
                printf("Rebooting.");
                __delay_ms(2000);
                break;
            }
            else{
                __lcd_clear();
                __lcd_display_control(1, 0, 0);
                printf("Invalid key!");
                __lcd_newline();
                printf("Rebooting.");
                __delay_ms(2000);
                break;
            }
        }
        return 0;
    }
    
    else {
        __lcd_clear();
        __lcd_display_control(1, 0, 0);
        printf("Invalid key!");
        __lcd_newline();
        printf("Rebooting.");
        __delay_ms(2000);
        return 0;
    }
}
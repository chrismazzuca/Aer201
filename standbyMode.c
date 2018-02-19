/*
 * File:   standbyMode.c
 * Author: chrismazzuca
 *
 * Created on February 14, 2018, 4:07 PM
 */

#include "standbyMode.h"

const char input[] = "123R456F789L*0#D";
const char keys[] = "123A456B789C*0#D";
unsigned int drawerInformation[8] = {};

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
            if (foodInput[0] == 1 || foodInput[0] == 2){
                returnValue = 1;
            }
            break;
            
        /*Diet F*/
        case 7:
            if (foodInput[0] == 1 || foodInput[0] == 2){
                returnValue = 2;
            }
            break;
            
        /*Diet L*/
        case 12:
            if (foodInput[0] == 1 || foodInput[0] == 2 || foodInput[0] == 3){
                returnValue = 3;
            }
            break;
            
        /*Diet RF*/
        case 10:
            if (foodInput[0] != 3 && foodInput[1] != 3 && foodInput[0] != 0 && foodInput[1] != 0){
                returnValue = 4;
            }
            break;
            
        /*Diet RL*/
        case 15:
            if (foodInput[1] == 3){
                if (foodInput[0] == 1){
                    returnValue = 5;
                }
            }
            else if (foodInput[0] != 3 && foodInput[0] != 0){
                returnValue = 5;
            }
            break;
            
        /*Diet FL*/
        case 19:
            if (foodInput[1] == 3){
                if (foodInput[0] == 1){
                    returnValue = 6;
                }
            }
            else if (foodInput[0] != 3 && foodInput[0] != 0){
                returnValue = 6;
            }
            break;
        
        /*Diet RRF*/
        case 13:
            if (foodInput[0] == 1 && foodInput[1] == 1){
                if (foodInput[2] == 1 || foodInput[2] == 2){
                    returnValue = 7;
                }
            }
            break;
            
        /*Diet RRL*/
        case 18:
            if (foodInput[0] == 1 && foodInput[1] == 1){
                if (foodInput[2] == 1 || foodInput[2] == 2){
                    returnValue = 8;
                }
            }
            break;
            
        /*Diet RFF*/
        case 17:
            if (foodInput[1] == 1 && foodInput[2] == 1){
                if (foodInput[0] == 1 || foodInput[0] == 2){
                    returnValue = 9;
                }
            }
            break;
            
        /*Diet RLL*/
        case 27:
            if (foodInput[0] == 1 && foodInput[1] == 1 && foodInput[2] == 1){
                returnValue = 10;
            }
            else if (foodInput[0] == 2 && foodInput[1] == 1 && foodInput[2] == 1){
                returnValue = 10;
            }
            else if (foodInput[0] == 1 && foodInput[1] == 2 && foodInput[2] == 1){
                returnValue = 10;
            }
            else if (foodInput[0] == 1 && foodInput[1] == 1 && foodInput[2] == 2){
                returnValue = 10;
            }
            break;
            
        /*Diet RFL*/
        case 22:
            if (foodInput[0] == 1 && foodInput[1] == 1 && foodInput[2] == 1){
                returnValue = 11;
            }
            else if (foodInput[0] == 2 && foodInput[1] == 1 && foodInput[2] == 1){
                returnValue = 11;
            }
            else if (foodInput[0] == 1 && foodInput[1] == 2 && foodInput[2] == 1){
                returnValue = 11;
            }
            else if (foodInput[0] == 1 && foodInput[1] == 1 && foodInput[2] == 2){
                returnValue = 11;
            }
            break;
            
        /*Diet FFL*/
        case 26:
            if (foodInput[0] == 1 && foodInput[1] == 1){
                if (foodInput[2] == 1 || foodInput[2] == 2){
                    returnValue = 12;
                }
            }
            break;
            
        /*Diet FLL*/
        case 31:
            if (foodInput[0] == 1 && foodInput[1] == 1 && foodInput[2] == 1){
                returnValue = 13;
            }
            else if (foodInput[0] == 2 && foodInput[1] == 1 && foodInput[2] == 1){
                returnValue = 13;
            }
            else if (foodInput[0] == 1 && foodInput[1] == 2 && foodInput[2] == 1){
                returnValue = 13;
            }
            else if (foodInput[0] == 1 && foodInput[1] == 1 && foodInput[2] == 2){
                returnValue = 13;
            }
            break;
            
        /*Diet RRFL*/
        case 25:
            if (foodInput[0] == 1 && foodInput[1] == 1 && foodInput[2] == 1 && foodInput[3] == 1){
                returnValue = 14;
            }
            break;
            
        /*Diet RFFL*/
        case 29:
            if (foodInput[0] == 1 && foodInput[1] == 1 && foodInput[2] == 1 && foodInput[3] == 1){
                returnValue = 15;
            }
            break;
            
        /*Diet RFLL*/
        case 34:
            if (foodInput[0] == 1 && foodInput[1] == 1 && foodInput[2] == 1 && foodInput[3] == 1){
                returnValue = 16;
            }
            break;
            
        /*Diet RLLL*/
        case 39:
            if (foodInput[0] == 1 && foodInput[1] == 1 && foodInput[2] == 1 && foodInput[3] == 1){
                returnValue = 17;
            }
            break;
            
        /*Diet FLLL*/
        case 43:
            if (foodInput[0] == 1 && foodInput[1] == 1 && foodInput[2] == 1 && foodInput[3] == 1){
                returnValue = 18;
            }
            break;
            
        /* # Button pressed (no food for this drawer)*/
        default:
            break;
    }
    return returnValue;
}


unsigned int viewLogs(void){
    __lcd_clear();
    printf("5 previous logs");
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
            
            if (keypress2 == 0 || keypress2 == 1 || keypress2 == 2 || keypress2 == 4 || keypress2 == 5){
                putch(keys[keypress2]);
                __delay_ms(1000);
                break;
            }
        }
        
        __lcd_clear();
        printf("Great!");
        __delay_ms(1000);
        __lcd_clear();
        printf("Press any key to");
        __lcd_newline();
        printf("move on.");
        __delay_ms(500);
        
        while(PORTBbits.RB1 == 0){  
            continue;   
        }
        
        __lcd_clear();
        printf("Operation time:");
        __lcd_newline();
        printf("2:39");
        __delay_ms(500);
        
        while(PORTBbits.RB1 == 0){  
            continue;   
        }
        
        __lcd_clear();
        printf("Marked drawers:");
        __lcd_newline();
        printf("1, 13");
        __delay_ms(500);
        
        while(PORTBbits.RB1 == 0){  
            continue;   
        }
        
        __lcd_clear();
        printf("Remaining pieces:");
        __lcd_newline();
        printf("R:3  F:12  L:10");
        __delay_ms(500);
        
        while(PORTBbits.RB1 == 0){  
            continue;   
        }
        
        __lcd_clear();
        printf("Drawer Summary:");
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
        printf("# = Yes, D = No");
            
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
        
        if(keypress == 14){
            break;
        }
        else if (keypress == 15){
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
        
        if(keypress == 14){
            break;
        }
        else if (keypress == 15){
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
        
        if(keypress == 14){
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
        else if (keypress == 15){
            backCheck = 100;
            break;
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

void standbyMode(void){
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
    __delay_ms(2000);
    
    /*View logs, or begin operation*/
    __lcd_clear();
    printf("Press D to");
    __lcd_newline();
    printf("view logs,");
    __delay_ms(2000);
    __lcd_clear();
    printf("or press # to");
    __lcd_newline();
    printf("begin operation.");
    
    while(PORTBbits.RB1 == 0){  continue;   }
    unsigned char keypress = (PORTB & 0xF0) >> 4;
    while(PORTBbits.RB1 == 1){  continue;   }
    
    if (keypress == 14){
    
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
    printf("Press # to");
    __lcd_newline();
    printf("submit the input");
    __delay_ms(3000);
    __lcd_clear();
    printf("and * to clear");
    __lcd_newline();
    printf("your input.");
    __delay_ms(3000);*/
    
    unsigned int drawerCount = 0;      /*Keeps track of the number of drawers inputted*/
    unsigned int i = 0;                /*Random variable used in the for loop*/
    unsigned int validDrawer = 0;      /*Checks if the drawer is valid*/
    unsigned int zero = 0;             /*Checks if no drawer number was pressed*/
    unsigned int firstIteration = 0;   /*Checks to see if it is the first drawer*/
    unsigned int drawer = 0;           /*Used to determine which drawer is requested*/
    unsigned int diet = 0;             /*Used to keep track of diet*/
    unsigned int food = 0;             /*Used to keep track of the food type*/
    
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
            printf("# = Yes, D = No");
            
            while(PORTBbits.RB1 == 0){  continue;   }
            unsigned char keypress1 = (PORTB & 0xF0) >> 4;
            while(PORTBbits.RB1 == 1){  continue;   }
            
            if (keypress1 == 14){
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
                    drawerCount += 1;
                }
            }
            else if (keypress1 == 15){
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
                    drawerCount += 1;
                }
            }
    }
}
    else if (keypress == 15){
        while (1){
            unsigned int whichKey = viewLogs();
            if (whichKey == 14){
                continue;
            }
            else if (whichKey == 15){
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
        
    }
    
    else {
        __lcd_clear();
        __lcd_display_control(1, 0, 0);
        printf("Invalid key!");
        __lcd_newline();
        printf("Rebooting.");
        __delay_ms(2000);
    }
}
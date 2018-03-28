/*
 * File:   main.c
 * Author: chrismazzuca
 *
 * Created on January 13, 2018, 11:11 AM
 */


#include <xc.h>
#include "configBits.h"
#include "standbyMode.h"
#include "operationMode.h"
#include "EEPROMstorage.h"
#include "LCD.h"
#include "UART_PIC.h"

unsigned int firstIteration = 0;

void main(void) {
    
    // <editor-fold defaultstate="collapsed" desc="Machine Configuration">
    /********************************* PIN I/O ********************************/
    /* Write outputs to LATx, read inputs from PORTx. Here, all latches (LATx)
     * are being cleared (set low) to ensure a controlled start-up state. */  
    LATA = 0x00;
    LATB = 0x00; 
    LATC = 0x00;
    LATD = 0x00;
    LATE = 0x00;

    /* After the states of LATx are known, the data direction registers, TRISx
     * are configured. 0 --> output; 1 --> input. Default is  1. */
    TRISA = 0xFF; // All inputs (this is the default, but is explicated here for learning purposes)
    TRISB = 0xFF;
    TRISC = 0x00;
    TRISD = 0x00; // All output mode on port D for the LCD
    TRISE = 0x00;
    
    /************************** A/D Converter Module **************************/
    ADCON0 = 0x00;  // Disable ADC
    ADCON1 = 0b00001111; // Set all A/D ports to digital (pg. 222)
    // </editor-fold>
    
    initLCD();
    __lcd_clear();
    __lcd_display_control(1, 0, 0);
    printf("Standby");
    
    /* Initialize UART. */
    UART_Init(9600);
    
    //verticalStepper(1,1);
    
//    unsigned char arr1[1] = {'7'};
//    unsigned char arr2[1] = {'8'};
//    unsigned char arr3[1] = {'3'};
//    unsigned char arr4[1] = {'4'};
//    uartTransmitBlocking(arr1, 1);
//    __lcd_clear();
//    printf("Opening drawer");
//    __delay_ms(1000);
//    uartTransmitBlocking(arr2, 1);
//    __lcd_clear();
//    printf("Closing drawer");
//    __delay_ms(1000);
//    uartTransmitBlocking(arr3, 1);
//    __lcd_clear();
//    printf("Moving left");
//    __delay_ms(1000);
//    uartTransmitBlocking(arr4, 1);
//    __lcd_clear();
//    printf("Moving right");
//    __delay_ms(1000);
    
    unsigned int operationMode = 0;
    unsigned int i = 0;
    unsigned int l = 0;
    
    TRISDbits.TRISD0 = 0;
    TRISDbits.TRISD1 = 0;
    TRISDbits.TRISD2 = 0;
    TRISDbits.TRISD3 = 0;
    TRISDbits.TRISD4 = 0;
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;
    TRISDbits.TRISD7 = 0;
    TRISCbits.TRISC2 = 0;
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC0 = 0;
    TRISEbits.TRISE2 = 0;
    TRISAbits.TRISA4 = 0;
    TRISAbits.TRISA5 = 0;
    TRISEbits.TRISE0 = 0;
    TRISEbits.TRISE1 = 0;
    LATEbits.LATE2 = 0;
    LATCbits.LATC2 = 0;
    LATCbits.LATC1 = 0;
    LATCbits.LATC0 = 0;
    LATDbits.LATD3 = 0;
    LATDbits.LATD2 = 0;
    LATDbits.LATD1 = 0;
    LATDbits.LATD0 = 0;
    LATDbits.LATD4 = 0;
    LATDbits.LATD5 = 0;
    LATDbits.LATD6 = 0;
    LATDbits.LATD7 = 0;
    LATEbits.LATE1 = 0;
    LATEbits.LATE0 = 0;
    LATAbits.LATA4 = 0;
    LATAbits.LATA5 = 0;
//    
//    while(1){
//    for (i=0; i<500; i++){
//        LATCbits.LATC5 = 0;
//        LATCbits.LATC2 = 0;
//        LATCbits.LATC1 = 1;
//        LATCbits.LATC0 = 0;
//        __delay_ms(2);
//        LATCbits.LATC5 = 0;
//        LATCbits.LATC2 = 1;
//        LATCbits.LATC1 = 0;
//        LATCbits.LATC0 = 0;
//        __delay_ms(2);
//        LATCbits.LATC5 = 1;
//        LATCbits.LATC2 = 0;
//        LATCbits.LATC1 = 0;
//        LATCbits.LATC0 = 0;
//        __delay_ms(2);
//        LATCbits.LATC5 = 0;
//        LATCbits.LATC2 = 0;
//        LATCbits.LATC1 = 0;
//        LATCbits.LATC0 = 1;
//        __delay_ms(2);
//    }
//    
//    for (i=0; i<500; i++){
//        LATCbits.LATC5 = 1;
//        LATCbits.LATC2 = 0;
//        LATCbits.LATC1 = 0;
//        LATCbits.LATC0 = 0;
//        __delay_ms(2);
//        LATCbits.LATC5 = 0;
//        LATCbits.LATC2 = 1;
//        LATCbits.LATC1 = 0;
//        LATCbits.LATC0 = 0;
//        __delay_ms(2);
//        LATCbits.LATC5 = 0;
//        LATCbits.LATC2 = 0;
//        LATCbits.LATC1 = 1;
//        LATCbits.LATC0 = 0;
//        __delay_ms(2);
//        LATCbits.LATC5 = 0;
//        LATCbits.LATC2 = 0;
//        LATCbits.LATC1 = 0;
//        LATCbits.LATC0 = 1;
//        __delay_ms(2);
//    }
//    }
    
//    while(1){
//        LATCbits.LATC5 = 1;
//        LATCbits.LATC2 = 0;
//        __delay_ms(500);
//        LATCbits.LATC5 = 0;
//        LATCbits.LATC2 = 1;
//        __delay_ms(500);
//        LATCbits.LATC5 = 1;
//        LATCbits.LATC2 = 1;
//        __delay_ms(1);
//        LATCbits.LATC5 = 0;
//        LATCbits.LATC2 = 0;
//        __delay_ms(1000);
//    }
    
    unsigned int checkEEPROM = 0;
    
    checkEEPROM = readEEPROM(251);
    
    if (checkEEPROM == 255){
        initEEPROM();
        __lcd_clear();
        printf("Clearing EEPROM");
        __delay_ms(2000);
    }
    
    /* Standby Mode */
    operationMode = standbyMode();
    
    if (operationMode == 1){
        mainOperation();
    }
    
    firstIteration += 1;
    return;
}

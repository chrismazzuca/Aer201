/*
 * File:   main.c
 * Author: chrismazzuca
 *
 * Created on January 13, 2018, 11:11 AM
 */


#include <xc.h>
#include "configBits.h"
#include "standbyMode.h"

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
    
    /*TRISCbits.TRISC4 = 0;
    TRISCbits.TRISC3 = 0;
    LATCbits.LATC3 = 1;
    LATCbits.LATC4 = 1;
    __delay_ms(100);

    while(1){
    LATCbits.LATC3 = 0;
    LATCbits.LATC4 = 1;
    __delay_ms(3000);
    LATCbits.LATC3 = 1;
    LATCbits.LATC4 = 0;
    __delay_ms(3000);
    }*/
    
    /* Standby Mode */
    standbyMode();
    return;
}
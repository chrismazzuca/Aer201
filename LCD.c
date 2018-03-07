/*
 * File:   LCD.c
 * Author: chrismazzuca
 *
 * Created on March 6, 2018, 8:38 PM
 */

#include <xc.h>
#include "LCD.h"

static unsigned char LCDinterruptState;

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

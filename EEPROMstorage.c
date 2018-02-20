/*
 * File:   EEPROMstorage.c
 * Author: chrismazzuca
 *
 * Created on February 19, 2018, 8:56 PM
 */

#include "EEPROMstorage.h"

void writeEEPROM(unsigned char address, unsigned char data){
    while( EECON1bits.WR  ){continue;}      // make sure it's not busy with an earlier write.

    EECON1bits.WREN=1;                      // allow EEPROM writes
    EEADR=address;                          // load address of write to EEPROM
    EEDATA=data;                            // load data to write to EEPROM
    EECON1bits.EEPGD=0;                     // access EEPROM data memory
    EECON1bits.CFGS=0;                      // do not access configuration registers
    INTCONbits.GIE=0;                       // disable interrupts for critical EEPROM write sequence
    // required sequence start
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;
    // required sequence end
    INTCONbits.GIE = 1;                     // enable interrupts, critical sequence complete
    while (EECON1bits.WR==1);               // wait for write to complete
    EECON1bits.WREN=0;                      // do not allow EEPROM writes
}

unsigned char readEEPROM(unsigned char address){
    while( EECON1bits.WR  ){continue;}      // make sure it's not busy with an earlier write.
    
    EEADR = address;                        // load address of EEPROM to read
    EECON1bits.EEPGD = 0;                   // access EEPROM data memory
    EECON1bits.CFGS  = 0;                   // do not access configuration registers
    EECON1bits.RD    = 1;                   // initiate read 
    return( EEDATA );
}

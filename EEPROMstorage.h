/* 
 * File:   EEPROMstorage.h
 * Author: Chris Mazzuca
 * 
 * Created on February 19th, 2018 
 */

#ifndef EEPROMstorage_H
#define	EEPROMstorage_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdio.h>

void writeEEPROM(unsigned char address, unsigned char data);
unsigned char readEEPROM(unsigned char address);
void initEEPROM(void);
void shiftEEPROM(void);

#endif	/* EEPROMstorage_H */

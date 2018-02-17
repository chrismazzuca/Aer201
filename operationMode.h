/* 
 * File:   operationMode.h
 * Author: Chris Mazzuca
 * 
 * Created on February 14th, 2018 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef OPERATION_MODE_H
#define	OPERATION_MODE_H

/********************************** Includes **********************************/
#include <xc.h>
#include <stdio.h>
#include "configBits.h" 

/****************************** Public Interfaces *****************************/
void updateEEPROM(unsigned int data);
unsigned int readEEPROM(void);
void configTimer(unsigned int configure);
void verticalStepper(unsigned int rotations, unsigned int direction);
unsigned int horizontalStepper(unsigned int rotations, unsigned int direction);
unsigned int detectTape(void);
unsigned int configDrawer(unsigned int direction);
unsigned int distributePieces(unsigned int count, unsigned int pieceType);
unsigned int checkCount(unsigned int pieceType);
unsigned int trapDoor(unsigned int type, unsigned int direction);
void mainOperation(void);

#endif	/* OPERATION_MODE_H */

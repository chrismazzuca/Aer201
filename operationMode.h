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
void getUserInputs(void);
void sortDrawerList(void);
void verticalStepper(unsigned int rotations, unsigned int direction);
void horizontalStepper(unsigned int rotations, unsigned int direction);
unsigned int detectTape(void);
unsigned int configDrawer(unsigned int direction);
unsigned int distributeRound(unsigned int count);
unsigned int distributeFlat(unsigned int count);
unsigned int distributeLong(unsigned int count);
unsigned int checkCount(unsigned int pieceType);
unsigned int trapDoor(unsigned int type, unsigned int direction);
void mainOperation(void);

#endif	/* OPERATION_MODE_H */


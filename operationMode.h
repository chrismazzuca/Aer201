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
void getUserInputs(unsigned int drawerTable[8], unsigned int dietTable[8], unsigned int foodTable[8]);
void sortDrawerList(unsigned int drawerTable[8], unsigned int sortedDrawerTable[8]);
unsigned int findDrawerIndex(unsigned int drawerTable[8], unsigned int sortedDrawerValue);
unsigned int getRoundPieces(unsigned int dietType, unsigned int foodType);
unsigned int getFlatPieces(unsigned int dietType, unsigned int foodType);
unsigned int getLongPieces(unsigned int dietType, unsigned int foodType);
void verticalStepper(unsigned int rotations, unsigned int direction);
void horizontalStepper(unsigned int rotations, unsigned int direction);
void drawerStepper(unsigned int direction);
unsigned int moveVertically(unsigned int currentDrawer, unsigned int nextDrawer);
void moveHorizontally(unsigned int currentDrawer, unsigned int nextDrawer);
void initArm(unsigned int currentDrawer);
unsigned int detectTape(void);
void configDrawer(unsigned int direction);
void distributeRound(unsigned int count);
void distributeFlat(unsigned int count);
void distributeLong(unsigned int count);
void distributePieces(unsigned int roundPieces, unsigned int flatPieces, unsigned int longPieces);
unsigned int checkCount(unsigned int pieceType);
void trapDoor(unsigned int direction);
void mainOperation(void);

#endif	/* OPERATION_MODE_H */


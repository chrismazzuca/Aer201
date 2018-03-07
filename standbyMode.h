/* 
 * File:   standbyMode.h
 * Author: Chris Mazzuca
 * 
 * Created on February 14th, 2018 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef STANDBY_MODE_H
#define	STANDBY_MODE_H

/********************************** Includes **********************************/
#include <xc.h>
#include <stdio.h>
#include "configBits.h" 

/****************************** Public Interfaces *****************************/
void initLCD(void);
void putch(char data);
void lcdNibble(char data);
void lcdInst(char data);
void RTC_setTime(void);
unsigned int getRound(unsigned int code);
unsigned int getFlat(unsigned int code);
unsigned int getLong(unsigned int code);
unsigned int check_food(unsigned int sum, unsigned int foodInput[4]);
unsigned int verifyKeypress(unsigned char logNumber, unsigned int keypress);
unsigned int printFoodInput(unsigned int food);
unsigned int displayLogs(unsigned int logNumber);
unsigned int viewLogs(void);
unsigned int get_digits(unsigned int sum);
unsigned int food_press(unsigned int dietSum, unsigned int drawer);
unsigned int diet_press(unsigned int drawer);
unsigned int decode_diet(unsigned int dietType);
unsigned int decode_drawer(unsigned int drawer);
unsigned int which_drawer(void);
unsigned int standbyMode(void);

#endif	/* STANDBY_MODE_H */


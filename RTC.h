/* 
 * File:   EEPROMstorage.h
 * Author: Chris Mazzuca
 * 
 * Created on February 27th, 2018 
 */

#ifndef RTC_H
#define	RTC_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdio.h>

#define __bcd_to_num(num) (num & 0x0F) + ((num & 0xF0)>>4)*10

void RTC_setTime(void);
void tick(unsigned int *hour, unsigned int *minute, unsigned int *second);
signed int tock(unsigned int Hour, unsigned int Minute, unsigned int Second);

#endif	/* RTC_H */
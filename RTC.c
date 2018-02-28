/*
 * File:   RTC.c
 * Author: chrismazzuca
 *
 * Created on February 27, 2018, 5:47 PM
 */

#include "RTC.h"
#include "I2C.h"

const char happynewyear[7] = {  0x00, // Seconds 
                                0x28, // Minutes
                                0x00, // 24 hour mode, set to 23:00
                                0x06, // Sunday is 00
                                0x24, // Days
                                0x02, // Months
                                0x18  // Year
};


void RTC_setTime(void){
    /* Writes the happynewyear array to the RTC memory.
     *
     * Arguments: none
     *
     * Returns: none
     */
    
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b11010000); //7 bit RTC address + Write
    I2C_Master_Write(0x00); // Set memory pointer to seconds
    
    /* Write array. */
    for(char i=0; i<7; i++){
        I2C_Master_Write(happynewyear[i]);
    }
    
    I2C_Master_Stop(); //Stop condition
}


void tick(unsigned int *hour, unsigned int *minute, unsigned int *second){
	// Records the current hour, minute and second from the RTC using the I2C module
	
	I2C_Master_Init(100000);
    
    /* Reset RTC memory pointer. */
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b11010000); // 7 bit RTC address + Write
    I2C_Master_Write(0x00); // Set memory pointer to seconds
    I2C_Master_Stop(); // Stop condition
    
	/* Start reading from RTC */
    I2C_Master_Start(); 
    I2C_Master_Write(0b11010001); // 7 bit RTC address + Read
	*second = I2C_Master_Read(ACK); // Write value in seconds byte of RTC to second and
	*minute = I2C_Master_Read(ACK); // increment pointer to next byte with ACK read.
	*hour = I2C_Master_Read(NACK);  // Final NACK read.
    I2C_Master_Stop();

	return;
}


signed int tock(unsigned int Hour, unsigned int Minute, unsigned int Second){
	// Takes in previously recorded hour, minute and second and outputs the 
	// time in seconds between that and the current time.
	
	I2C_Master_Init(100000);
    
    /* Reset RTC memory pointer. */
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b11010000); // 7 bit RTC address + Write
    I2C_Master_Write(0x00); // Set memory pointer to seconds
    I2C_Master_Stop(); // Stop condition
    
	/* Start reading from RTC */
    I2C_Master_Start(); 
    I2C_Master_Write(0b11010001); // 7 bit RTC address + Read
	unsigned int CurSec = I2C_Master_Read(ACK); // Write value in seconds byte of RTC to second and
	unsigned int CurMin = I2C_Master_Read(ACK); // increment pointer to next byte with ACK read.
	unsigned int CurHour = I2C_Master_Read(NACK);  // Final NACK read.
    I2C_Master_Stop();
	
	// Converts binary coded decimal values to signed int values
	signed int hour = __bcd_to_num(Hour);
	signed int minute = __bcd_to_num(Minute);
	signed int second = __bcd_to_num(Second);
	
	signed int curSec = __bcd_to_num(CurSec);
	signed int curMin = __bcd_to_num(CurMin);
	signed int curHour = __bcd_to_num(CurHour);
	
	int elapsedSeconds = (curHour - hour)*3600 + (curMin - minute)*60 + (curSec - second);
	
	return elapsedSeconds;
}
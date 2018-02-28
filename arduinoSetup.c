/*
 * File:   arduinoSetup.c
 * Author: chrismazzuca
 *
 * Created on February 27, 2018, 8:40 PM
 */

#include "arduinoSetup.h"
#include "I2C.h"

void sendByteToArduino(unsigned char byte){
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b00010000); // 7-bit Arduino slave address + write
    I2C_Master_Write(byte);
    I2C_Master_Stop();
}

void setArduinoToStandby(void){
    sendByteToArduino('M');
    sendByteToArduino('S');
    sendByteToArduino('C');
}
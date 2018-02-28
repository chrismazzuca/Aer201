/* 
 * File:   arduinoSetup.h
 * Author: Chris Mazzuca
 * 
 */

#ifndef ARDUINO_SETUP_H
#define	ARDUINO_SETUP_H

/********************************** Includes **********************************/
#include <xc.h>
#include "configBits.h"
#include "I2C.h"

void sendByteToArduino(unsigned char byte);

void setArduinoToStandby(void);

#endif /* ARDUINO_SETUP_H */

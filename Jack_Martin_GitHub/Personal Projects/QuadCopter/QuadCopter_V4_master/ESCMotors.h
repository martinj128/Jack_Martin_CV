
// ESCMotors.h

#ifndef _ESCMOTORS_h
#define _ESCMOTORS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
	#include <ESC.h>
#else
	#include "WProgram.h"
#endif

void initialiseMotors(void);
void driveMotors(float, float, float, float);
float minMax(float, float, float);

//ESC escFL, escFR, escBL, escBR;


#endif

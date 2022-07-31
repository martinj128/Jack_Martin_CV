// PID.h

#ifndef _PID_h
#define _PID_h

#include "arduino.h"
#include "Wire.h"

void PIDControl(void);
void calculateErrors(void);
void getRCSignals(void);
void convertSetPoints(void);
float setRange(uint16_t, float, float, float, float);

/*extern float Kp[3];
extern float Ki[3];
extern float Kd[3];*/

extern uint16_t setPointsRaw[4];
extern float setPointsPID[4];

extern float psi_pid;                //psi pid value to appl to motor
extern float theta_pid;
extern float phi_pid;

#endif

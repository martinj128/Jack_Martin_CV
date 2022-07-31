#ifndef _MPU6050_GYRO_ACCEL_h
#define _MPU6050_GYRO_ACCEL_h

#include "Arduino.h"

void setupMPU(void);
void getYawPitchRoll(void);

extern float ypr[3];

#endif

// 
//
// 

#include "ESCMotors.h"

//ESC escFR, escBR, escFL, escBL;

static float driveFR = 0;
static float driveBR = 0;
static float driveFL = 0;
static float driveBL = 0;
static uint16_t rollVal = 0;
static uint16_t pitchVal = 0;
static uint16_t yawVal = 0;

ESC  escBR (11, 1000, 2000, 1000);
ESC  escBL (10, 1000, 2000, 900);
ESC  escFR (5, 1000, 2000, 1000);
ESC  escFL (3, 1000, 2000, 1000);


void initialiseMotors() {

  escBR.arm();
  delay(900);
  escFR.arm();
  delay(900);
  escFL.arm();
  delay(900);
  escBL.arm();
  delay(900);

}

void driveMotors(float psi_pid, float theta_pid, float phi_pid, float throttle) {
	
	driveFL = throttle - phi_pid - theta_pid + psi_pid; 
	driveFR = throttle + phi_pid - theta_pid - psi_pid;
	driveBL = throttle - phi_pid + theta_pid - psi_pid;
	driveBR = throttle + phi_pid + theta_pid + psi_pid;

  escFR.speed(driveFR);
  escBR.speed(driveBR);
  escFL.speed(driveFL);
  escBL.speed(driveBL);
	
	//Serial.print("FL: ");Serial.print(driveFL);Serial.print("\tFR: ");Serial.print(driveFR);Serial.print("\tBL: ");Serial.print(driveBL);Serial.print("\tBR: ");Serial.println(driveBR);	

}

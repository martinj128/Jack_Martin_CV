// PID = error.Kp + (integral)error.Ki + (derivative)error.Kd
// 
// 

#include "PID.h"
//#include "MPU6050_Gyro_Accel.h"
#include "MPU6050_JR.h"
#include "ESCMotors.h"

#define psi 0
#define theta 1
#define phi 2
#define elapsedTime 0.004

static float currentErrors[3]=         {0,0,0};			//current error values for PID Contphier
static float previousErrors[3] =       {0,0,0};			//errors from previous PID loop
static float proportionalErrors[3] =   {0,0,0};		  //current/proportional errors in psi, theta, phi
static float integralErrors[3] =       {0,0,0};			//integral errors for PID contphier
static float derivativeErrors[3] =     {0,0,0};			//derivative errors for PID Contphier

static float Kp[3] = {2,1.6,1.25};				//proportional gains tbc 1.5, 1.75, 1.6
static float Ki[3] = {0,0.25 ,0.22};				//integral gains tbc 0.02, 0.06, 0.03
static float Kd[3] = {20,26,28};					//derivative gains tbc 15, 25, 22
float psi_pid = 0;								//psi pid value to appl to motor
float theta_pid = 0;
float phi_pid = 0;

uint16_t setPointsRaw[4] = {0,0,0,0};
float setPointsPID[4] = {0, 0, 0,0};

bool psiFlag = false;
bool thetaFlag = false;
bool phiFlag = false;

void PIDControl(void) {

  getRCSignals();
  convertSetPoints();

  getYawPitchRoll();

	if(setPointsPID[0] >= 1150){

  //getYawPitchRoll();
	calculateErrors();

	if(psiFlag || thetaFlag || phiFlag){psi_pid = (proportionalErrors[psi] * Kp[psi]) + (integralErrors[psi] * Ki[psi]) + (derivativeErrors[psi] * Kd[psi]);psiFlag = false;
	                                    theta_pid = (proportionalErrors[theta] * Kp[theta]) + (integralErrors[theta] * Ki[theta]) + (derivativeErrors[theta] * Kd[theta]);thetaFlag = false;
	                                    phi_pid = (proportionalErrors[phi] * Kp[phi]) + (integralErrors[phi] * Ki[phi]) + (derivativeErrors[phi] * Kd[phi]);phiFlag = false;}
  //phi_pid = 0;

	psi_pid = constrain(psi_pid, -450, 450);      //yaw
	theta_pid = constrain(theta_pid, -600, 600);   //pitch
	phi_pid = constrain(phi_pid, -600, 600);      //roll

	}

	else {psi_pid = 0; theta_pid = 0; phi_pid = 0;}

 //Serial.print("PSI: ");Serial.print(psi_pid);Serial.print("\ttheta: ");Serial.print(theta_pid);Serial.print("\tphi_pid: ");Serial.println(phi_pid);

  driveMotors(psi_pid, theta_pid, phi_pid, setPointsPID[0]);

	}

void calculateErrors(void){

   //cli();

    currentErrors[psi]   = ypr[psi]   - setPointsPID[1];
    currentErrors[theta] = ypr[theta] - setPointsPID[2];
    currentErrors[phi]  = ypr[phi]  - setPointsPID[3];

    if(currentErrors[phi] > 1| currentErrors[phi] <  -1){phiFlag = true;}
    if(currentErrors[theta] > 1 || currentErrors[theta] < -1){ thetaFlag = true;}
    if(currentErrors[psi] > 1 || currentErrors[psi] < -1){psiFlag = true;}

    proportionalErrors[psi] = currentErrors[psi];
    proportionalErrors[theta] = currentErrors[theta];
    proportionalErrors[phi] = currentErrors[phi];
    

    // Calculate sum of errors : Integral coefficients

    
    integralErrors[psi]   += currentErrors[psi];
    integralErrors[theta] += currentErrors[theta];
    integralErrors[phi]  += currentErrors[phi];

    // Keep values in acceptable range
    integralErrors[psi]   = constrain(integralErrors[psi],   -10/Ki[psi],   10/Ki[psi]);
    integralErrors[theta] = constrain(integralErrors[theta], -400, 400);
    integralErrors[phi]  = constrain(integralErrors[phi],  -400, 400);

    // Calculate error delta : Derivative coefficients
    derivativeErrors[psi]   = (currentErrors[psi]   - previousErrors[psi]);
    derivativeErrors[theta] = (currentErrors[theta] - previousErrors[theta]);
    derivativeErrors[phi]  = (currentErrors[phi]  - previousErrors[phi]);

    // Save current error as previous_error for next time
    previousErrors[psi]   = currentErrors[psi];
    previousErrors[theta] = currentErrors[theta];
    previousErrors[phi]  = currentErrors[phi];

    //sei();

    
}

void getRCSignals(){

  Wire.requestFrom(9, 8);

  while(Wire.available()){

  setPointsRaw[0] = (Wire.read() << 8) | Wire.read();   //Throttle
  setPointsRaw[1] = (Wire.read() << 8) | Wire.read();   //Yaw
  setPointsRaw[2] = (Wire.read() << 8) | Wire.read();   //Pitch
  setPointsRaw[3] = (Wire.read() << 8) | Wire.read();   //Roll

  }
  
}

void convertSetPoints(void){

  setPointsPID[0] = constrain(setPointsRaw[0], 1000, 2000);

  for(int i = 1; i<4; i++){

        setPointsRaw[i] = constrain(setPointsRaw[i], 1000, 2000);
        setPointsPID[i] = setRange((float)setPointsRaw[i], 1000, 2000, -8, 8); 
  }
      
}

float setRange(uint16_t x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

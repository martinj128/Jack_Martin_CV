

#define Throttle 0x01
#define Yaw 0x02
#define Pitch 0x04
#define Roll 0x08
#define MPU 0x10
#define Timer 0x20
#define interruptFreq 100 //x ms
#define prescaler 1024
#define clkFreq 16000000

#include "PID.h"
#include "ESCMotors.h"
#include "MPU6050_JR.h"

uint16_t startTime[4], currentTime;
uint8_t interrupt[6] = {Throttle, Yaw, Pitch, Roll, MPU, Timer};
volatile uint16_t rxChannelTimes[4];
volatile uint8_t isrFlagsShared = 0;
uint8_t pinChangeStates = 0;
uint8_t prevPinStates = 0;
uint8_t pinStatesCurr = 0;
uint8_t mpuPinCurr = 0x00;
uint8_t mpuPinPrev = 0x00;
uint8_t mpuPinChange = 0x00;
int i = 0;
int switchInt = 0;
byte slaveReady = 0x00;

//extern bool mpuInterrupt = false;


void setup() {
  
  Wire.begin();
  Wire.setClock(400000);
  Serial.begin(115200);

  //Serial.flush();

  Serial.println("Starting...");
  
  pinMode(LED_BUILTIN, OUTPUT);
  //pinMode(4, INPUT);
  digitalWrite(LED_BUILTIN, LOW);

  while (!slaveReady){

    Wire.requestFrom(9, 1);

    if(Wire.available()){
      slaveReady = Wire.read();
    }

    if(!slaveReady){Serial.println(": Not Ready");}

    delay(100);
  }

 Serial.print(slaveReady, BIN);Serial.println(": Slave Ready");

  //Serial.flush();
  
  //while(!Serial);

  Serial.println("Arduino Master and Slave Ready");

  Serial.println("Starting Motors...");
  initialiseMotors();

  delay(1000);

  //Serial.println("Setting up MPU...");
      
  setupMPU();
  //calibrateMPU(1000);
  //delay(2000);
  //Serial.println("Settling MPU module...");
  delay(1000);
  //Serial.println("Ready to Start");

  cli();

  //Setup Timer Interrupt
  TCCR2A = 0x02;                              //set timer in ctc mode
  TCCR2B = 0x07;                              //select prescaler of 1024
  OCR2A = getInterruptTime(interruptFreq);    //Compare val set for timer 63 = 1ms 
  TIMSK2 |= 0x02;                             //enable timer2A compare interrupt
  ASSR |= 0x20;                               //set clk to mcu
  GTCCR = 0x02;
  TCNT2 = 0;                                  //reset counter
  TIFR2 = 0x02;                             //clear interrupt flag

  sei();

  delay(1000);
  
  digitalWrite(LED_BUILTIN, HIGH);
  
  
}

void loop() {

static uint8_t isrFlags;

  if (isrFlagsShared){
    
    isrFlags = isrFlagsShared;

    if(isrFlags & Timer){PIDControl();switchInt++;}

    isrFlagsShared = 0;
    isrFlags = 0;
      
    }

    if(switchInt = 2000){Serial.print(setPointsPID[0]);Serial.print("\t");Serial.print(setPointsPID[1]);Serial.print("\t");Serial.print(setPointsPID[2]);Serial.print("\t");Serial.println(setPointsPID[3]);switchInt = 0;}
    //Serial.print(("\t Yaw")); Serial.print(ypr[0]);
    //Serial.print(("\t Pitch ")); Serial.print(ypr[1]);
    //Serial.print(("\t Roll ")); Serial.println(ypr[2]);
    //Serial.println();//if(mpuInterrupt){Serial.println("MPU!");}
     //if(mpuInterrupt){Serial.println("MPU!");}

      
    
}

  
ISR(TIMER2_COMPA_vect){

  TIFR2 |= 0x02;

  isrFlagsShared |= Timer;

}


int getInterruptTime(float x){

  float y = clkFreq/(2*x*prescaler);

return (int)y-1;
  
}


 

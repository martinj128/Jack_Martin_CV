#include "Wire.h"
#define Throttle 0x01
#define Yaw 0x02
#define Pitch 0x04
#define Roll 0x08

uint16_t startTime[4], currentTime, throttleVal;
uint8_t interrupt[4] = {Throttle, Yaw, Pitch, Roll};
volatile uint16_t rxChannelTimes[4], rcVals[4];
volatile uint8_t isrFlagsShared = 0;
uint8_t pinChangeStates = 0;
uint8_t prevPinStates = 0;
uint8_t pinStatesCurr = 0;
int i = 0;
int j = 0;
byte slaveReady = 0;

uint8_t rcValsBytes[8] = {};

//extern bool mpuInterrupt = false;

//uint16_t bigNum = 16059;


void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  Wire.begin(9);
  Wire.setClock(400000);
  Wire.onRequest(requestHandler);

  cli();
  
  //Setup interrupt pins for rx Channel
  MCUCR &= (0<<PUD);
  PORTD |= 0b00000000;    //enable pullup resistor
  DDRD |= 0b000000000;     //Set pins out inputs for Port D
  PCICR = (1<<PCIE2);          //Turn on Port D P/C ints
  PCMSK2 |= 0xF0;         //enable p/c interrupts on D4-D7
  PCIFR |= (1<<PCIF2);    //Clear interrupt flag for portD

  //sei();*/

  //delay(1000); 

  slaveReady = 1; 
  
  digitalWrite(LED_BUILTIN, HIGH);

  sei();
  
}

void loop() {

static uint8_t isrFlags;

  if (isrFlagsShared){

    isrFlags = isrFlagsShared;
    
    cli();

    if(isrFlags & Throttle){rcVals[0] = rxChannelTimes[0];}
    if(isrFlags & Yaw){rcVals[1] = rxChannelTimes[1];}
    if(isrFlags & Pitch){rcVals[2] = rxChannelTimes[2];}
    if(isrFlags & Roll){rcVals[3] = rxChannelTimes[3];}
   
    isrFlagsShared = 0;
    isrFlags = 0;

    sei();
      
    }

    //Serial.print(throttleVal);Serial.print("\t");Serial.print(setPointsRaw[0]);Serial.print("\t");Serial.print(setPointsRaw[1]);Serial.print("\t");Serial.println(setPointsRaw[2]);
         
}


ISR(PCINT2_vect){

  PCIFR |= (1<<PCIF2);

  currentTime = (uint16_t)micros();

  __asm__("nop");

  pinStatesCurr = PIND>>4;

  pinChangeStates = pinStatesCurr ^ prevPinStates;

  for(i = 0; i < 4;i++){

    if(pinChangeStates & interrupt[i]){  //if pin has changed state
      if(pinStatesCurr & interrupt[i]){startTime[i] = currentTime;}       //if pin state is high, start time
      else {rxChannelTimes[i] = currentTime - startTime[i];               //if pin state low, record time
            isrFlagsShared |= interrupt[i];}                          //share flag in main loop
    }  
  }

  prevPinStates = pinStatesCurr;
 
}

void requestHandler(int bytesToSend){

 //rcValsBytes[0] = (rxChannelTimes[0]>>8 & 0xFF); rcValsBytes[1] = rxChannelTimes[0] & 0xFF;
 
if (bytesToSend < 8){


  Wire.write(slaveReady);

  }

else converToBytes();Wire.write(rcValsBytes, 8);
  
}

void converToBytes(){

  i = 0;
  j = 0;

  while(i < 4){

    rcValsBytes[j] = (rxChannelTimes[i]>>8 & 0xFF);
    rcValsBytes[j+1] = rxChannelTimes[i] & 0xFF;
    i++;
    j = j+2; 
    
  }
}



 

#ifndef _ESPFFT_H_
#define _ESPFFT_H_

#include "fft.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "I2Ssetup.h"
#include "math.h"
#include "guitar_data.h"

#define NFFT 4096
#define TOTAL_TIME 0.016

void runFFT(void*);
void oneTimeFFT(float*);
void convertInt2Flo(int32_t*, float*);
void determineNote(float);

float testData[256];

extern QueueHandle_t fft_data;



#endif 
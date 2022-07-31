#ifndef _I2Ssetup_H_
#define _I2Ssetup_H_

#include "driver/i2s.h"
#include "freertos/FreeRTOS.h"
#include "esp_system.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_err.h"
#include "freertos/queue.h"
#include "espFFT.h"

#define DMA_BUFFER_SIZE 1024
#define DMA_BUFFER_NUM 16
#define SAMPLE_BUFFER_NUM 
#define I2S_SAMPLE_RATE 4000
#define I2S_NUM 0
#define I2S_CLK GPIO_NUM_19
#define I2S_WS GPIO_NUM_21
#define I2S_DATA_IN GPIO_NUM_18

void setupI2S();
int32_t cvt24bit(uint32_t);
void readMicData(void*);

extern QueueHandle_t fft_data;

#endif
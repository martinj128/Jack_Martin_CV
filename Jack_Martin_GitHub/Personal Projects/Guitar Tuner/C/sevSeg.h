#ifndef _SEVSEG_H_
#define _SEVSEG_H_

#include "freertos/FreeRTOS.h"
#include "esp_system.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "esp_err.h"
#include "driver/gpio.h"
#include "guitar_data.h"

#define CLK_PIN GPIO_NUM_25
#define LATCH_PIN GPIO_NUM_26
#define DATA_PIN GPIO_NUM_27
#define LOW_LED GPIO_NUM_4
#define GOOD_LED GPIO_NUM_22
#define HIGH_LED GPIO_NUM_5

void sendDataTask2(uint8_t);
void setupSevSeg(void);
void sevSegTask2(void*);
void sevSegTask(void*);

extern uint8_t letters[7];

extern QueueHandle_t sev_seg;

#endif
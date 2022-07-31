/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_log.h"
#include "I2Ssetup.h"
#include "sevSeg.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include "espFFT.h"

ledc_timer_config_t clkPinPWM;
ledc_channel_config_t clkPinPWMChannel;

void flashLED();
void togglePins();

void app_main(void)
{
    printf("Hello world!\n");
    setupSevSeg();
    setupI2S();
    vTaskDelay(1000 / portTICK_RATE_MS);

    xTaskCreatePinnedToCore(runFFT, "FFT Task", 20000, NULL, 2, NULL, 1);
    xTaskCreatePinnedToCore(sevSegTask, "sevSeg Task", 2048, NULL, 3, NULL, 1);
    //xTaskCreatePinnedToCore(flashLED, "flash LED Task", 2048, NULL, 2, NULL, 0);
    xTaskCreatePinnedToCore(readMicData, "read I2S Data Task", 20000, NULL, 1, NULL, 0);
    //xTaskCreatePinnedToCore(sevSegTask, "sevSeg Task", 2048, NULL, 1, NULL, 0);
    //xTaskCreatePinnedToCore(togglePins, "Toggle Task", 2048, NULL, 1, NULL, 0);

}

void flashLED(){

    TickType_t delayLED = pdMS_TO_TICKS(1000);

    gpio_set_direction(GPIO_NUM_2, GPIO_MODE_DEF_OUTPUT);

    while(1){

        gpio_set_level(GPIO_NUM_2, 1);
        vTaskDelay(delayLED);
        gpio_set_level(GPIO_NUM_2, 0);
        vTaskDelay(delayLED);

    }
    
}
    
void togglePins(void* param){

    TickType_t delayLED = pdMS_TO_TICKS(1000);

    gpio_set_direction(CLK_PIN, GPIO_MODE_DEF_OUTPUT);

    gpio_set_direction(LATCH_PIN, GPIO_MODE_DEF_OUTPUT);

    gpio_set_direction(DATA_PIN, GPIO_MODE_DEF_OUTPUT);

    uint8_t leds[3] = {25, 26, 27};

    uint8_t ledstate = 0;
    uint8_t led = 0;

    while(1){

        /*switch (ledstate)
        {
        case 0:
            if (led < 1){
                gpio_set_level(leds[led], 1);
            }

            else {
                gpio_set_level(leds[led], 0);
                led++;
                gpio_set_level(leds[led], 1);
            }

            if(led > 1){ledstate = 1;}
            break;

        case 1:
            gpio_set_level(leds[led], 0);
            led--;
            gpio_set_level(leds[led], 1);
            
            if(led < 1){ledstate = 0;}

            break;
        
        default:
            break;
        }*/

        gpio_set_level(CLK_PIN, 1);
        gpio_set_level(LATCH_PIN, 1);
        gpio_set_level(DATA_PIN, 1);
        vTaskDelay(delayLED);

        gpio_set_level(CLK_PIN, 0);
        gpio_set_level(LATCH_PIN, 0);
        gpio_set_level(DATA_PIN, 0);
        vTaskDelay(delayLED);

    }

}
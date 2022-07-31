#include "sevSeg.h"

const char* TAG2 = "sevSeg"; 

//uint8_t letters[8] = {0xE5, 0xA5, 0xF5, 0xB7, 0XF7, 0xC5, 0xD7, 0x08};  //C, D, E , F, G, A, B, #

uint8_t hello[12] = {0xC5, 0xCD, 0xD7, 0xDF, 0xE5, 0xA5, 0xAD, 0xF5, 0xFD, 0xB7, 0xBF, 0xF3};        //C, C#, D, D#, E , F, F#, G, G#, A, A#, B

uint8_t all_on = 0xFF;
uint8_t all_off = 0x00;

QueueHandle_t sev_seg;

void setupSevSeg(void){

    gpio_set_direction(CLK_PIN, GPIO_MODE_DEF_OUTPUT);
    gpio_pullup_dis(CLK_PIN);
    gpio_pulldown_en(CLK_PIN);
    gpio_set_level(CLK_PIN, 0);

    gpio_set_direction(LATCH_PIN, GPIO_MODE_DEF_OUTPUT);
    gpio_pullup_dis(LATCH_PIN);
    gpio_pulldown_dis(LATCH_PIN);
    gpio_set_level(LATCH_PIN, 0);

    gpio_set_direction(DATA_PIN, GPIO_MODE_DEF_OUTPUT);
    gpio_pullup_dis(DATA_PIN);
    gpio_pulldown_en(DATA_PIN);
    gpio_set_level(DATA_PIN, 0);

    gpio_set_direction(LOW_LED, GPIO_MODE_DEF_OUTPUT);
    gpio_pullup_dis(LOW_LED);
    gpio_pulldown_en(LOW_LED);
    gpio_set_level(LOW_LED, 0);

    gpio_set_direction(HIGH_LED, GPIO_MODE_DEF_OUTPUT);
    gpio_pullup_dis(HIGH_LED);
    gpio_pulldown_en(HIGH_LED);
    gpio_set_level(HIGH_LED, 0);

    gpio_set_direction(GOOD_LED, GPIO_MODE_DEF_OUTPUT);
    gpio_pullup_dis(GOOD_LED);
    gpio_pulldown_en(GOOD_LED);
    gpio_set_level(GOOD_LED, 0);

    sendDataTask2(all_on);
    gpio_set_level(LOW_LED, 1);
    gpio_set_level(HIGH_LED, 1);
    gpio_set_level(GOOD_LED, 1);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    sendDataTask2(all_off);
    gpio_set_level(LOW_LED, 0);
    gpio_set_level(HIGH_LED, 0);
    gpio_set_level(GOOD_LED, 0);
    vTaskDelay(1000 / portTICK_PERIOD_MS);

   sev_seg = xQueueCreate(1, sizeof(musicData_t));

}

void sevSegTask(void* param){

    musicData_t noteData;
    TickType_t loopDelay = pdMS_TO_TICKS(1000);
    uint8_t lastNote = 13;

    while (1){

        if(xQueueReceive(sev_seg, &noteData, portMAX_DELAY) == pdPASS){

            if(noteData.note != lastNote){
                sendDataTask2(hello[noteData.note]);
                lastNote = noteData.note;
            }

            printf("state: %d\n", noteData.freq_state);

            switch (noteData.freq_state)
            {
            case 0:
                gpio_set_level(LOW_LED, 0);
                gpio_set_level(HIGH_LED, 1);
                gpio_set_level(GOOD_LED, 0);
                break;

            case 1:
                gpio_set_level(LOW_LED, 1);
                gpio_set_level(HIGH_LED, 0);
                gpio_set_level(GOOD_LED, 0);
                break;

            case 2:
                gpio_set_level(LOW_LED, 0);
                gpio_set_level(HIGH_LED, 0);
                gpio_set_level(GOOD_LED, 1);
                break;
            
            default:
                break;
            }
            
        }

    }

}

void sendDataTask2(uint8_t sevSegData){

    uint8_t i = 7;
    uint8_t dataSent = 0;
   // uint8_t clkLevel = 0;
   TickType_t delayCLK = pdMS_TO_TICKS(1);
   TickType_t loopDelay = pdMS_TO_TICKS(1000);

    gpio_set_level(LATCH_PIN, 0);       //red goes low
    //ESP_LOGI(TAG2, "Latch Pin Low");

    //ledc_timer_resume(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);

    for(int i = 7; dataSent < 8; i--){

    //clkLevel = gpio_get_level(PIN_CLK)
            
         uint8_t bit_mask = (sevSegData) & 0x01<<i;

         //gpio_set_level(CLK_PIN, 1);

         //ESP_LOGI(TAG, "bitmask: %d", bit_mask);

            if(bit_mask){
                gpio_set_level(DATA_PIN, 1);
                //ESP_LOGI(TAG2, "bit: %d\tLevel High", i);
            }
            
            else {gpio_set_level(DATA_PIN, 0);
               // ESP_LOGI(TAG2, "bit: %d\tLevel Low", i);
            }
            
            dataSent++;
            gpio_set_level(CLK_PIN, 1);             //Orange goes high
            //ESP_LOGI(TAG2, "Clk Pin High");
            vTaskDelay(delayCLK);
            gpio_set_level(CLK_PIN, 0);             //Orange goes low
            //ESP_LOGI(TAG2, "Clk Pin Low");
    }

    //gpio_set_level(CLK_PIN, 1);//ledc_stop(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);

    gpio_set_level(LATCH_PIN, 1);
    //ESP_LOGI(TAG2, "Latch Pin High");

}

void sevSegTask2(void* param){

    TickType_t loopDelay = pdMS_TO_TICKS(1000);

    while(1){

        for(int i = 0; i < 12; i++){
        sendDataTask2(hello[i]);
        vTaskDelay(loopDelay);
        }
    }
}
#include "I2Ssetup.h"

static const char *TAG = "I2S";
QueueHandle_t i2s_data;

void setupI2S(){

i2s_config_t i2s_config = {
    
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = I2S_SAMPLE_RATE,                                   //ammend to allow required fbins (resolution) Fs/Ns  
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,          //32Bit size for the size of the I2S Mic words
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,           //only left channel (no stereo), pull L/R to ground
    .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
    .tx_desc_auto_clear = false,
    .dma_buf_count = DMA_BUFFER_NUM,
    .dma_buf_len = DMA_BUFFER_SIZE,                    //sample buffer size 128 * 4 so enough bytes for 32 bit sample
    .use_apll = false,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,  // Interrupt level 1, default 0

};

    gpio_set_direction(I2S_CLK, GPIO_MODE_DEF_OUTPUT);
    gpio_set_direction(I2S_DATA_IN, GPIO_MODE_DEF_INPUT);
    gpio_set_direction(I2S_WS, GPIO_MODE_DEF_OUTPUT);

static const i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_CLK,
    .ws_io_num = I2S_WS,
    .data_out_num = -1,
    .data_in_num = I2S_DATA_IN,
};

ESP_ERROR_CHECK(i2s_driver_install(I2S_NUM, &i2s_config, NULL, NULL));
ESP_ERROR_CHECK(i2s_set_pin(I2S_NUM, &pin_config));

//i2s_data = xQueueCreate(4, sizeof(i2s_event_t));

i2s_stop(I2S_NUM);

float freq_resolution = (float)I2S_SAMPLE_RATE / (float)((DMA_BUFFER_SIZE/4)*(DMA_BUFFER_NUM));
float total_sample_time = (float)(1/(float)I2S_SAMPLE_RATE) * (float)(DMA_BUFFER_SIZE*DMA_BUFFER_NUM)/4;
printf("-----FFT Config with current settings-----\nFreq Bins: %f\tTotal Time: %f\n", freq_resolution, total_sample_time);

vTaskDelay(3000 / portTICK_PERIOD_MS);

//fft_data = xQueueCreate(1, sizeof(float *));

}

void readMicData(void *param){

    uint8_t* i2sRxBuffer = (uint8_t*)calloc((DMA_BUFFER_SIZE*DMA_BUFFER_NUM), sizeof(uint8_t));
    float* fft_sample_buffer = (float*)calloc((DMA_BUFFER_SIZE*DMA_BUFFER_NUM)/4, sizeof(float));
    size_t bytesReadTotal;
    size_t bytesReadPerBuff;
    //esp_err_t err = NULL;
    TickType_t delayRxBuffer = pdMS_TO_TICKS(2048);

    i2s_start(I2S_NUM);

    while(1)
    
        {     
            bytesReadTotal = 0;
            bytesReadPerBuff = 0;

            for(uint8_t i = 0; i < DMA_BUFFER_NUM; i++){

                i2s_read(I2S_NUM, &i2sRxBuffer[i*1024], DMA_BUFFER_SIZE, &bytesReadPerBuff, portMAX_DELAY);
                bytesReadTotal = bytesReadTotal + bytesReadPerBuff;
                
            }
        
                uint16_t samples_read = bytesReadTotal/4;
                printf("Samples Read: %d\n", samples_read);
                
                for(int i = 0; i < samples_read; i++){

                    uint8_t byte2 = i2sRxBuffer[(i * 4) + 1];
                    //uint8_t byte2 = 0x00;
                    uint8_t byte3 = i2sRxBuffer[(i * 4) + 2];
                    uint8_t byte4 = i2sRxBuffer[(i * 4) + 3];
            
                    uint32_t sample32 = (uint32_t)(byte4<<16) + (uint32_t)(byte3<<8) + (uint32_t)(byte2);

                    int32_t sample24 = cvt24bit(sample32);

                    float sample24f = (float)sample24;

                    fft_sample_buffer[i] = sample24f;

            //insert into signed 32 bit int buffer for fft/dsp via queue!

            //printf("Samples Read: %d\n\t Data: %02x:%02x:%02x", samples_read, byte3, byte2, byte1);
            //printf("%0.2f,\n", sample24_float);

                } 

                xQueueSend(fft_data, (void*)&fft_sample_buffer, portMAX_DELAY);
                //free(i2sRxBuffer);
                //free(fft_sample_buffer);
                vTaskDelay(1);

            }

}


int32_t cvt24bit(uint32_t val) {
    val &= 0xffffff;  // limit to 24 bits -- may not be necessary
    if (val >= (UINT32_C(1) << 23))
        return (int32_t)val - (INT32_C(1) << 24);
    else
        return val;
}
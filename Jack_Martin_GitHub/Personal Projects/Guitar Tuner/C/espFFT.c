#include "espFFT.h"

//float fft_input[NFFT];
//float fft_output[NFFT];
//float musicNotes[48];
/*float testData[256] = {47738.00,
42289.00,
24224.00,
-941.00,
-20526.00,
-34646.00,
-44587.00,
-42722.00,
-26751.00,
-1888.00,
20064.00,
37070.00,
47781.00,
45443.00,
28353.00,
5015.00,
-17316.00,
-32308.00,
-43413.00,
-43777.00,
-30313.00,
-5906.00,
16703.00,
34652.00,
46555.00,
46254.00,
32235.00,
8230.00,
-14302.00,
-30571.00,
-42864.00,
-44846.00,
-33268.00,
-11362.00,
12578.00,
31215.00,
44774.00,
47160.00,
35418.00,
12663.00,
-10664.00,
-28838.00,
-40886.00,
-45632.00,
-36991.00,
-15662.00,
8267.00,
27802.00,
42039.00,
46551.00,
37562.00,
16816.00,
-7541.00,
-27158.00,
-40207.00,
-43815.00,
-41747.00,
-22811.00,
6395.00,
23648.00,
39373.00,
46500.00,
37855.00,
18321.00,
-3824.00,
-20616.00,
-37222.00,
-48727.00,
-42385.00,
-24195.00,
-1451.00,
19110.00,
36946.00,
45880.00,
41281.00,
25021.00,
975.00,
-22158.00,
-37830.00,
-46702.00,
-44675.00,
-29634.00,
-4553.00,
18725.00,
35142.00,
44163.00,
44190.00,
28767.00,
3558.00,
-17686.00,
-33317.00,
-43574.00,
-45230.00,
-32781.00,
-8055.00,
15859.00,
32441.00,
45276.00,
46016.00,
33113.00,
9792.00,
-14568.00,
-30568.00,
-42391.00,
-45344.00,
-33483.00,
-11967.00,
11359.00,
30384.00,
43343.00,
46286.00,
35906.00,
13884.00,
-8465.00,
-26100.00,
-40211.00,
-46694.00,
-37778.00,
-16739.00,
8196.00,
27908.00,
42407.00,
47931.00,
39552.00,
17161.00,
-7603.00,
-26176.00,
-39158.00,
-45474.00,
-38833.00,
-19711.00,
3269.00,
22594.00,
38091.00,
47040.00,
41611.00,
22333.00,
-2266.00,
-23162.00,
-38011.00,
-46130.00,
-42301.00,
-26312.00,
-1085.00,
19889.00,
35759.00,
44246.00,
42317.00,
25695.00,
873.00,
-21389.00,
-36600.00,
-46156.00,
-44981.00,
-30556.00,
-5597.00,
16251.00,
32358.00,
43745.00,
43537.00,
28691.00,
4930.00,
-16945.00,
-33461.00,
-45005.00,
-47779.00,
-34732.00,
-11151.00,
13105.00,
31715.00,
44083.00,
45480.00,
31776.00,
9401.00,
-13305.00,
-29501.00,
-42044.00,
-45991.00,
-35276.00,
-13799.00,
9321.00,
28518.00,
43309.00,
47791.00,
37461.00,
14820.00,
-8582.00,
-26028.00,
-39674.00,
-44857.00,
-37984.00,
-17265.00,
7751.00,
27234.00,
42998.00,
49096.00,
40408.00,
19731.00,
-4495.00,
-24509.00,
-37119.00,
-45805.00,
-39789.00,
-20615.00,
2573.00,
23622.00,
40177.00,
48039.00,
42292.00,
24400.00,
-415.00,
-21639.00,
-37104.00,
-45369.00,
-42370.00,
-26352.00,
-2152.00,
19669.00,
35503.00,
45914.00,
43351.00,
27214.00,
1927.00,
-19976.00,
-34231.00,
-44957.00,
-44466.00,
-30833.00,
-6653.00,
16122.00,
33436.00,
45061.00,
44064.00,
30081.00,
6888.00,
-15028.00,
-31511.00,
-43529.00,
-46082.00,
-34471.00,
-10653.00,
13347.00,
31164.00,
44536.00,
46689.00,
33727.00,
10737.00,
-12053.00,
-29079.00,
-41915.00,
-46018.00,
-36622.00,
-15683.00,
8850.00,
28660.00,
42947.00,};*/

QueueHandle_t fft_data;

const char* TAG = "FFT";

void runFFT(void *param){

fft_data = xQueueCreate(1, sizeof(float*));
float* audio_data;
float total_sample_time = (float)(1/(float)I2S_SAMPLE_RATE) * (float)(DMA_BUFFER_SIZE*DMA_BUFFER_NUM)/4;

//float audio_test[NFFT];

//printf("-----FFT Config with current settings-----\nFreq Bins: %2f\tTotal Time: %2f\n", freq_resolution, total_sample_time);
//vTaskDelay(2000 / portTICK_PERIOD_MS);


while(1){

    if(xQueueReceive(fft_data, &audio_data, portMAX_DELAY) == pdPASS){

        float fundamental_freq = 0;
        float max_magnitude = 0;


        //float* fft_input = (float*)malloc(NFFT);
        //float* fft_output = (float*)malloc(NFFT);

        // Create the FFT config structure
        fft_config_t *real_fft_plan = fft_init(NFFT, FFT_REAL, FFT_FORWARD, NULL, NULL);

        // Fill array with some data
        for (int k = 0 ; k < NFFT ; k++)
        {
        real_fft_plan->input[k] = (float)audio_data[k];        //WHAT IS THE PROBLEM HERE!?!?!?
        }

        // Execute transformation
        fft_execute(real_fft_plan);

        // Now do something with the output
        /*printf("DC component : %f\n", fft_output[0]);  // DC is at [0]
        for (int k = 1 ; k < real_fft_plan->size / 2 ; k++){
        printf("%d-th freq : %f+j%f\n", k, fft_output[2*k], fft_output[2*k+1]);
        printf("Middle component : %f\n", fft_output[1]);  // N/2 is real and stored at [1]
        }*/

        for (int k = 1 ; k < real_fft_plan->size / 2 ; k++){

        float mag = sqrt(pow(real_fft_plan->output[2*k],2) + pow(real_fft_plan->output[2*k+1],2));

        float freq = (k*1.0)/total_sample_time;

            if(mag > max_magnitude)
            {
                max_magnitude = mag;
                fundamental_freq = freq;
            }

        }

        // Don't forget to clean up at the end to free all the memory that was allocated
        fft_destroy(real_fft_plan);
        determineNote(fundamental_freq);

        }

    }

}

void oneTimeFFT(float* data_in){

    float freq_resolution = (float)I2S_SAMPLE_RATE / (float)((DMA_BUFFER_SIZE/4)*(DMA_BUFFER_NUM/2));
    float total_sample_time = (float)(1/(float)I2S_SAMPLE_RATE) * (float)(DMA_BUFFER_SIZE*DMA_BUFFER_NUM)/8;
    //float* audio_data_fl = (float*)malloc(DMA_BUFFER_SIZE/4);
    //float audio_test[NFFT];

    //printf("-----FFT Config with current settings-----\nFreq Bins: %f\tTotal Time: %f\n", freq_resolution, total_sample_time);

        //convertInt2Flo(sample_data, audio_data_fl);

        float fundamental_freq = 0;
        float max_magnitude = 0;

        // Create the FFT config structure
        fft_config_t *real_fft_plan = fft_init(NFFT, FFT_REAL, FFT_FORWARD, NULL, NULL);

        // Fill array with some data
        for (int k = 0 ; k < NFFT ; k++)
        {
        real_fft_plan->input[k] = data_in[k];
        }

        // Execute transformation
        fft_execute(real_fft_plan);

       // Now do something with the output
    /*printf("DC component : %f\n", real_fft_plan->output[0]);  // DC is at [0]
    for (int k = 1 ; k < 50 ; k++)
    printf("%d-th freq : %f+j%f\n", k, real_fft_plan->output[2*k], real_fft_plan->output[2*k+1]);
    printf("Middle component : %f\n", real_fft_plan->output[1]);  // N/2 is real and stored at [1]*/

        //remove dc component

        //real_fft_plan->output[0] = 0;

        for (int k = 1 ; k < real_fft_plan->size / 2 ; k++){

        float mag = sqrt(pow(real_fft_plan->output[2*k],2) + pow(real_fft_plan->output[2*k+1],2));

        float freq = (k*1.0)/total_sample_time;

            if(mag > max_magnitude)
            {
                max_magnitude = mag;
                fundamental_freq = freq;
                //k_val = k;
            }

        }

        //float mag2 = sqrt(pow(real_fft_plan->output[(2*k_val)/2],2) + pow(real_fft_plan->output[(2*k_val+1)/2],2));

        //printf("Fundamental Frequency: %0.2f\tMag: %0.2f\tFreq ind: %d\n", fundamental_freq, max_magnitude, k_val);
        //printf("Mag1: %f\tMag2: %f\tCo-eff:%d\n", max_magnitude, mag2, k_val);

    fft_destroy(real_fft_plan);
    if(fundamental_freq > 65.4){determineNote(fundamental_freq);}
        //printf("WHYYYY!?!?!?\n");

}

void convertInt2Flo(int32_t* intBuffer, float* floatBuffer){

    for(int i = 0; i < 128; i++){

        floatBuffer[i] = (float)intBuffer[i];

    }

}

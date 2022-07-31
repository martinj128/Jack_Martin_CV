#include "guitar_data.h"

const float musicNotes[48] = {65.4064,	130.8128,	261.6256,	523.2511,       //C
	                        69.2957,	138.5913,	277.1826,	554.3653,       //C#
	                        73.4162,	146.8324,	293.6648,	587.3295,       //D
	                        77.7817,	155.5635,	311.1270,	622.2540,       //D#
	                        82.4069,	164.8138,	329.6276,	659.2551,       //E
	                        87.3071,	174.6141,	349.2282,	698.4565,       //F
	                        92.4986,	184.9972,	369.9944,	739.9888,       //F#
	                        97.9989,	195.9977,	391.9954,	783.9909,       //G
	                        103.8262,	207.6523,	415.3047,	830.6094,       //G#
	                        110.0000,	220.0000,	440.0000,	880.0000,       //A
                            116.5409,	233.0819,	466.1638,	932.3275,       //A#
            	            123.4708,	246.9417,	493.8833,	987.7666};      //B

const char musicNotesChars[8] = {'C', 'D', 'E', 'F', 'G', 'A', 'B', '#'};

void determineNote(float freq){

    //find initial note
    //uint8_t note, pitch;
    float remainder = 0;
    float noteFreq;
    //float musicData.freq_diff = 0;
    float remainders[48];
    
    musicData_t musicData;

    //detect which note, from getting remainder of frequency multiples
    for(int i = 0; i < 48; i++){

        remainders[i] = abs(freq - musicNotes[i]); //FIX THIS SOMEHOW TO GET ABS VALUE
        //printf("freq: %0.2f\tMusicNote: %0.2f\tremainder: %0.2f\n", freq, musicNotes[i], remainders[i]);

    }

    //determine note
    remainder = remainders[0];
    ///note = 0;
    uint8_t index = 0; 

    for(int i = 1; i < 48; i++){

        if(remainder > remainders[i])
        {
            remainder = remainders[i];
            index = i;
        }
    }

    noteFreq = musicNotes[index];
    musicData.freq_diff = noteFreq - freq;
    musicData.note = index / 4;
    musicData.pitch = (index % 4) + 2;

    if(musicData.freq_diff < -1){    //Over the note
        musicData.freq_state = 0;
    }

    else if (musicData.freq_diff > 1){  //under the note
        musicData.freq_state = 1;
    }

    else if(musicData.freq_diff < 1 && musicData.freq_diff > -1){
        musicData.freq_state = 2;  //its good enough
    }
    
    //pitch = (index % 4)+2;
    //note = index / 4;
    //printf("Note: %d\n", note);

    xQueueSend(sev_seg, (void *)&musicData, portMAX_DELAY);

    switch (musicData.note)
    {
    case 0:
        if(musicData.freq_diff < 1 && musicData.freq_diff > -1){
            printf("Note: C%d - %f\t%f\tGood Enough...\n", musicData.pitch, noteFreq, freq);
        }
        else if(musicData.freq_diff > 1){
            printf("Note: C%d - %f\t%f\tUnder Note\n", musicData.pitch, noteFreq, freq);
        }
        else {
            printf("Note: C%d - %f\t%f\tOver Note\n", musicData.pitch, noteFreq, freq);
        }

        //sendDataTask2(letters[2]);
        break;

    case 1:
        if(musicData.freq_diff < 1 && musicData.freq_diff > -1){
            printf("Note: C#%d - %f\t%f\tGood Enough...\n", musicData.pitch, noteFreq, freq);
        }
        else if(musicData.freq_diff > 1){
            printf("Note: C#%d - %f\t%f\tUnder Note\n", musicData.pitch, noteFreq, freq);
        }
        else {
            printf("Note: C#%d - %f\t%f\tOver Note\n", musicData.pitch, noteFreq, freq);
        }

        //sendDataTask2(letters[2] &= 0xFF);
        break;

    case 2:
        if(musicData.freq_diff < 1 && musicData.freq_diff > -1){
            printf("Note: D%d - %f\t%f\tGood Enough...\n", musicData.pitch, noteFreq, freq);
        }
        else if(musicData.freq_diff > 1){
            printf("Note: D%d - %f\t%f\tUnder Note\n", musicData.pitch, noteFreq, freq);
        }
        else {
            printf("Note: D%d - %f\t%f\tOver Note\n", musicData.pitch, noteFreq, freq);
        }

        //sendDataTask2(letters[3]);
        break;

    case 3:
        if(musicData.freq_diff < 1 && musicData.freq_diff > -1){
            printf("Note: D#%d - %f\t%f\tGood Enough...\n", musicData.pitch, noteFreq, freq);
        }
        else if(musicData.freq_diff > 1){
            printf("Note: D#%d - %f\t%f\tUnder Note\n", musicData.pitch, noteFreq, freq);
        }
        else {
            printf("Note: D#%d - %f\t%f\tOver Note\n", musicData.pitch, noteFreq, freq);
        }

        //sendDataTask2(letters[3] &= 0xFF);
        break;

    case 4:
        if(musicData.freq_diff < 1 && musicData.freq_diff > -1){
            printf("Note: E%d - %f\t%f\tGood Enough...\n", musicData.pitch, noteFreq, freq);
        }
        else if(musicData.freq_diff > 1){
            printf("Note: E%d - %f\t%f\tUnder Note\n", musicData.pitch, noteFreq, freq);
        }
        else {
            printf("Note: E%d - %f\t%f\tOver Note\n", musicData.pitch, noteFreq, freq);
        }

        //sendDataTask2(letters[4]);
        break;

    case 5:
        if(musicData.freq_diff < 1 && musicData.freq_diff > -1){
            printf("Note: F%d - %f\t%f\tGood Enough...\n", musicData.pitch, noteFreq, freq);
        }
        else if(musicData.freq_diff > 1){
            printf("Note: F%d - %f\t%f\tUnder Note\n", musicData.pitch, noteFreq, freq);
        }
        else {
            printf("Note: F%d - %f\t%f\tOver Note\n", musicData.pitch, noteFreq, freq);
        }

        //sendDataTask2(letters[5]);
        break;

    case 6:
        if(musicData.freq_diff < 1 && musicData.freq_diff > -1){
            printf("Note: F#%d - %f\t%f\tGood Enough...\n", musicData.pitch, noteFreq, freq);
        }
        else if(musicData.freq_diff > 1){
            printf("Note: F#%d - %f\t%f\tUnder Note\n", musicData.pitch, noteFreq, freq);
        }
        else {
            printf("Note: F#%d - %f\t%f\tOver Note\n", musicData.pitch, noteFreq, freq);
        }

        //sendDataTask2(letters[5] &= 0xFF);
        break;

    case 7:
        if(musicData.freq_diff < 1 && musicData.freq_diff > -1){
            printf("Note: G%d - %f\t%f\tGood Enough...\n", musicData.pitch, noteFreq, freq);
        }
        else if(musicData.freq_diff > 1){
            printf("Note: G%d - %f\t%f\tUnder Note\n", musicData.pitch, noteFreq, freq);
        }
        else {
            printf("Note: G%d - %f\t%f\tOver Note\n", musicData.pitch, noteFreq, freq);
        }

        //sendDataTask2(letters[6]);
        break;

    case 8:
        if(musicData.freq_diff < 1 && musicData.freq_diff > -1){
            printf("Note: G#%d - %f\t%f\tGood Enough...\n", musicData.pitch, noteFreq, freq);
        }
        else if(musicData.freq_diff > 1){
            printf("Note: G#%d - %f\t%f\tUnder Note\n", musicData.pitch, noteFreq, freq);
        }
        else {
            printf("Note: G#%d - %f\t%f\tOver Note\n", musicData.pitch, noteFreq, freq);
        }

        //sendDataTask2(letters[6] &= 0xFF);
        break;

    case 9:
        if(musicData.freq_diff < 1 && musicData.freq_diff > -1){
            printf("Note: A%d - %f\t%f\tGood Enough...\n", musicData.pitch, noteFreq, freq);
        }
        else if(musicData.freq_diff > 1){
            printf("Note: A%d - %f\t%f\tUnder Note\n", musicData.pitch, noteFreq, freq);
        }
        else {
            printf("Note: A%d - %f\t%f\tOver Note\n", musicData.pitch, noteFreq, freq);
        }

        //sendDataTask2(letters[0]);
        break;
    
    case 10:
        if(musicData.freq_diff < 1 && musicData.freq_diff > -1){
            printf("Note: A#%d - %f\t%f\tGood Enough...\n", musicData.pitch, noteFreq, freq);
        }
        else if(musicData.freq_diff > 1){
            printf("Note: A#%d - %f\t%f\tUnder Note\n", musicData.pitch, noteFreq, freq);
        }
        else {
            printf("Note: A#%d - %f\t%f\tOver Note\n", musicData.pitch, noteFreq, freq);
        }

        //sendDataTask2(letters[0] &= 0xFF);
        break;

    case 11:
        if(musicData.freq_diff < 1 && musicData.freq_diff > -1){
            printf("Note: B%d - %f\t%f\tGood Enough...\n", musicData.pitch, noteFreq, freq);
        }
        else if(musicData.freq_diff > 1){
            printf("Note: B%d - %f\t%f\tUnder Note\n", musicData.pitch, noteFreq, freq);
        }
        else {
            printf("Note: B%d - %f\t%f\tOver Note\n", musicData.pitch, noteFreq, freq);
        }

        //sendDataTask2(letters[1]);
        break;

    default:
        break;
    }

}
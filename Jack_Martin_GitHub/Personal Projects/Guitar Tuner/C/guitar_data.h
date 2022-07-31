#ifndef _GUITAR_DATA_H_
#define _GUITAR_DATA_H_

#include "esp_system.h"
#include "math.h"
#include "sevSeg.h"

void determineNote(float);

extern const float musicNotes[48];

typedef struct musicData{
    uint8_t note;
    uint8_t pitch;
    float freq_diff;
    uint8_t freq_state;
} musicData_t;

#endif
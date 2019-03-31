#ifndef FREQUENCY_H
#define FREQUENCY_H


#include "audio.h"

void getFrequencyTest(int channel, Audio *audio);
double getFrequencies(int channel, Audio *audio, int subLength, double **frequenciesAtTime);
// void getFrequency(int channel, Audio *audio);


#endif

#ifndef FREQUENCY_H
#define FREQUENCY_H

#include "audio.h"


// esta funcion retorna un array de frecuencias, cada frecuencia es extraida de los array que
// quedan como resultado de dividir todos los datos del audio en subarray de longitud subLength
int getFrequencies(int channel, Audio *audio, int subLength, double **frequenciesAtTime);

#endif

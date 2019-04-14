// uso de malloc
#include <stdlib.h>

// libreria que realiza la transformada
#include <fftw3.h>

// uso de la funcion amplitude
#include "miscellaneous.h"
// referencia a la estructura de audio y a la funcion getChannelAt
#include "audio.h"


void fillFrequencies(int length, int framerate, double **frequencies){
    *frequencies = (double *) malloc(sizeof(double) * length);

    int N = (length % 2) == 0 ? length / 2 : (length - 1) / 2;

    double deltaF = (1.0 * framerate) / length;

    for (int i = 0; i < length; i++){
        if(i < N){
            (*frequencies)[i] = i * deltaF;
        }else{
            (*frequencies)[i] = (-2 * N + i) * deltaF;
        }
    }
}


double getFrequencies(int channel, Audio *audio, int subLength, double **frequenciesAtTime){
    fftw_complex *in, *out;
    fftw_plan my_plan;

    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * subLength);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * subLength);

    // FFTW_ESTIMATE, FFTW_MEASURE
    my_plan = fftw_plan_dft_1d(subLength, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

    *frequenciesAtTime = (double *) malloc(sizeof(double) * ((*audio).frames / (*audio).framerate));

    double *freq;
    fillFrequencies(subLength, (*audio).framerate, &freq);

    int *dataAt = (int *) malloc(sizeof(int) * subLength);

    for (int i = 0; i < (*audio).frames; i+=subLength){
        getChannelAt(channel, audio, i, subLength, &dataAt, False);

        for (int j = 0; j < subLength; j++){
            in[j][0] = dataAt[j];
            in[j][1] = 0;
        }

        fftw_execute(my_plan);

        // se busca el indice de la mayor amplitude
        int indexMax = 0;
        for (int j = 1; j < subLength; j++){
            if(amplitude(out[indexMax][0], out[indexMax][1]) < amplitude(out[j][0], out[j][1])){
                indexMax = j;
            }
        }

        (*frequenciesAtTime)[i / subLength] = freq[indexMax];
    }

    fftw_destroy_plan(my_plan);
    fftw_free(in);
    fftw_free(out);  

    free(dataAt);
    free(freq);

    return frames2seconds(subLength, (*audio).framerate);
}

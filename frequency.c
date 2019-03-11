#include <stdlib.h>

#include <math.h>
#include <fftw3.h>

#include "audio.h"
#include "frequency.h"


double amplitude(double a, double b){
    return sqrt(a * a + b * b);
}

void getFrequency(int channel, Audio *audio){
    // arrays para almacenamiento de los datos de entrada y salida de la
    // transformada de fourier
    fftw_complex *input, *output;
    // para hacer la transformada primero se hace un plan
    fftw_plan plan;

    // para evitar lineas largas por escribir audio -> frames
    int frames = audio -> frames;

    // allocando espacio para los datos
    input = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * frames);
    output = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * frames);

    // se crea el plan para la transformada, FFTW_MEASURE, FFTW_ESTIMATE
    plan = fftw_plan_dft_1d(frames, input, output, FFTW_FORWARD, FFTW_ESTIMATE);

    // se copian los datos del canal dado
    int *data;
    getChannel(0, audio, &data);
    for (int i = 0; i < frames; i++){
        input[i][0] = data[i];
        input[i][1] = 0;
    }

    // se realiza la transformada
    fftw_execute(plan);

    // se busca el indice con mayor amplitude
    int indexMax = 0;
    for (int i = 1; i < frames; ++i){
        if(amplitude(output[indexMax][0], output[indexMax][1]) < amplitude(output[i][0], output[i][1])){
            indexMax = i;
        }
    }

    // se escoge la frecuencia de mayor amplitud
    double dt = 1.0 / audio -> framerate;
    double *freq = (double *) malloc(sizeof(double) * frames);
    int index = 0;
    for (double i = 0; i < 1.0 / (dt); i += 1.0 / (dt * frames)){
        freq[index] = i;
        index++;
    }

    // se imprime la frecuencia
    printf("%lf\n", freq[indexMax]);

    // se libera memoria
    fftw_destroy_plan(plan);
    fftw_free(input);
    fftw_free(output);
}
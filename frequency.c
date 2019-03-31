#include <stdlib.h>

#include <math.h>
#include <fftw3.h>

#include "audio.h"
#include "frequency.h"


double amplitude(double a, double b){
    return sqrt(a * a + b * b);
}


void __transform(int length, int *data, double *result){
    // arrays para almacenamiento de los datos de entrada y salida de la
    // transformada de fourier
    fftw_complex *input, *output;

    // para hacer la transformada primero se hace un plan
    fftw_plan plan;

    // allocando espacio para los datos
    input = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * length);
    output = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * length);

    // se crea el plan para la transformada, FFTW_MEASURE, FFTW_ESTIMATE
    plan = fftw_plan_dft_1d(length, input, output, FFTW_FORWARD, FFTW_ESTIMATE);

    // se copian los datos
    for (int i = 0; i < length; i++){
        input[i][0] = data[i];
        input[i][1] = 0;
    }

    // se realiza la transformada
    fftw_execute(plan);

    // se copia la amplitud en el array de entrada
    for (int i = 0; i < length; ++i){
        result[i] = amplitude(output[i][0], output[i][1]);
    }

    // se libera memoria
    fftw_destroy_plan(plan);
    fftw_free(input);
    fftw_free(output);
}


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

    // FFTW_ESTIMATE, FFTW_MEASURED
    my_plan = fftw_plan_dft_1d(subLength, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

    double *freq;
    fillFrequencies(subLength, (*audio).framerate, &freq);

    int *dataAt = (int *) malloc(sizeof(int) * subLength);

    FILE *fout = fopen("datos.txt", "w");

    // se imprimen los datos
    for (int i = 0; i < (*audio).frames; i+=subLength){
        getChannelAt(channel, audio, i, subLength, &dataAt, False);
        for (int j = 0; j < subLength; j++){
            in[j][0] = dataAt[j];
            in[j][1] = 0;
        }

        fftw_execute(my_plan);

        // se busca el indice con mayor amplitude
        int indexMax = 0;
        printf("========================================");
        for (int j = 1; j < subLength; j++){
            // printf("%lf - %lf\n", freq[j], amplitude(out[j][0], out[j][1]));
            if(amplitude(out[indexMax][0], out[indexMax][1]) < amplitude(out[j][0], out[j][1])){
                indexMax = j;
            }
        }

        printf("Frequencie: %lf\n", freq[indexMax]);
    }

    fftw_destroy_plan(my_plan);
    fftw_free(in);
    fftw_free(out);  

    free(dataAt);
    free(freq);
}


void getFrequencyTest(int channel, Audio *audio){
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
    getChannel(0, audio, &data, True);
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
    double *freq;
    fillFrequencies(frames, audio -> framerate, &freq);
    // se imprime la frecuencia
    printf("%lf, %lf, %lf\n", freq[indexMax - 3], freq[indexMax - 2], freq[indexMax]);

    // se libera memoria
    fftw_destroy_plan(plan);
    fftw_free(input);
    fftw_free(output);

    free(data);
}
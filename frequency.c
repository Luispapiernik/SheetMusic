// uso de malloc
#include <stdlib.h>
#include <math.h>

// libreria que realiza la transformada
#include <fftw3.h>

// uso de la funcion amplitude
#include "miscellaneous.h"
// referencia a la estructura de audio y a la funcion getChannelAt
#include "audio.h"


// esta funcion llena el array de frecuencias con las frecuencias correspondientes
// a la amplitud, como la trasnformada de fourier de fftw deja la frecuencia cero,
// en la primera componente del array, se necesita hacer un shift para poner esta
// frecuencia en el cero
void fillFrequencies(int length, int framerate, double **frequencies){
    // se aloca memoria para el array de frecuenncias
    *frequencies = (double *) malloc(sizeof(double) * length);

    // se consigue la mitad del array, si es par entonces es divisible por 2,
    // si es impar se resta uno para que sea divisible por 2
    int N = (length % 2) == 0 ? length / 2 : (length - 1) / 2;

    // separacion entre cada frecuenncia
    double deltaF = (1.0 * framerate) / length;

    // se itera sobre el numero de frecuencias, los condicionales son usados para
    // poder implementar el shift
    for (int i = 0; i < length; i++){
        if(i < N){
            (*frequencies)[i] = i * deltaF;
        }else{
            (*frequencies)[i] = (-2 * N + i) * deltaF;
        }
    }
}

// esta funcion retorna un array de frecuencias, cada frecuencia es extraida de los array que
// quedan como resultado de dividir todos los datos del audio en subarray de longitud subLength
int getFrequencies(int channel, Audio *audio, int subLength, double **frequenciesAtTime){
    // se crea array de entrada y salida para la realizacion de la transformada
    fftw_complex *in, *out;
    // se crea la variable que guarda informacion relevante para la realizacion de la transformada
    fftw_plan my_plan;

    // se pide espacio para los array de entrada y salida
    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * subLength);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * subLength);

    // FFTW_ESTIMATE, FFTW_MEASURE
    // se realiza un plan para la transformada, esto es, se genera la informacion necesaria,
    // como array de entradas y salidad, longitud de los vectores de entrada y salidad y demas
    my_plan = fftw_plan_dft_1d(subLength, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

    // se consigue la longitud(numero de frecuencias), del vector de salidad
    int length = (*audio).frames / subLength;
    // se pide espacio para el vector de frecuencias de salida
    *frequenciesAtTime = (double *) malloc(sizeof(double) * length);

    // se crea vector de frecuencias de las transformadas
    double *freq;
    // se llena el vector de frecuencias
    fillFrequencies(subLength, (*audio).framerate, &freq);

    // se pide espacio para el vector donde se guardaran los datos del audio
    int *dataAt = (int *) malloc(sizeof(int) * subLength);

    // se itera sobre los datos
    for (int i = 0; i < (*audio).frames - subLength; i+=subLength){
        // se consiguen los datos de cada subdivision del audio
        getChannelAt(channel, audio, i, subLength, &dataAt, False);

        // se itera sobre dataAt y se copian los datos en el vector de
        // entrada de la transformada
        for (int j = 0; j < subLength; j++){
            in[j][0] = dataAt[j];
            in[j][1] = 0;  // como es una señal real, la parte imaginaria vale 0
        }

        // se realiza la transformada
        fftw_execute(my_plan);

        // se busca el indice de la mayor amplitude
        int indexMax = 0;  // se supone que es el primero
        for (int j = 1; j < subLength; j++){
            if(amplitude(out[indexMax][0], out[indexMax][1]) < amplitude(out[j][0], out[j][1])){
                indexMax = j;
            }
        }

        // se guarda la frecuencia con mayor amplitud
        (*frequenciesAtTime)[i / subLength] = fabs(freq[indexMax]);
    }

    // se libera memoria
    // para tipos de datos declarados en fftw
    fftw_destroy_plan(my_plan);
    fftw_free(in);
    fftw_free(out);  

    // para tipos de datos estandar
    free(dataAt);
    free(freq);

    // se retorna el numero de frecuencias
    return length;
}

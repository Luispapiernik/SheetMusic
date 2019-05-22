// estas son guradas, evitan que el archivo sea incluido mas
// de 2 veces en un mismo archivo
#ifndef AUDIO_H
#define AUDIO_H


// tiene referencia a la estructura Bool
#include "miscellaneous.h"

// sobrenombre para los canales 1 y 2 de archivos de audio
#define CHANNEL1 0
#define CHANNEL2 1

// Estructura que representa audio
typedef struct{
    // nombre del archivo desde el que se cargan los datos
    char *filename;
    // datos del audio, son amplitudes de alguna perturbacion
    int *data;

    // numero de frames,
    int frames;
    // numero de canales del audio, mono 1 canal, estereo 2 canales
    int channels;
    // taza de muestreo, frames o datos por segundos
    int framerate;
    // duracion en segundos del audio
    double seconds;
}Audio;  // se crea un sinonimo de tipo


// funcion que lee un archivo de audio y carga la estructura audio con
// la informacion relevante
void readWavFile(Audio *audio);

// funcion que muestra informacion de el audio
void showInfo(Audio *audio);

// funcion que obtiene todos los datos de un canal
void getChannel(int channel, Audio *audio, int **data, Bool allocate);

// funcion que obtiene los datos de un canal desde una posicion determinada
void getChannelAt(int channel, Audio *audio, int initialFrame, int length, int **data, Bool allocate);

#endif

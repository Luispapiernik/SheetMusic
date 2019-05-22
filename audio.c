// para usar malloc
#include <stdlib.h>

// la libreria sndfile y la funcion print necesita stdio.h
#include <stdio.h>
#include <sndfile.h>

// para la referencia a la estructura Bool
#include "miscellaneous.h"
// para la referencia a la estructura audio
#include "audio.h"


// funcion que lee un archivo de audio y carga la estructura audio con
// la informacion relevante
void readWavFile(Audio *audio){
    // puntero para abrir el archivo de audio
    SNDFILE *inFile;
    // sf_open, carga la informacion del archivo de audio en esta estructura
    SF_INFO info;

    // como se va a abrir el archivo en solo lectura esto debe ser cero
    info.format = 0;
    // se abre el archivo de nombre audio -> filename como lectura
    inFile = sf_open(audio -> filename, SFM_READ, &info);
    // si no se puede abrir el archivo, se sale del programa
    if (inFile == NULL){
        printf("Failed to open the file %s.\n", audio -> filename);
        exit(-1);
    }

    // se llena la estructura audio con los datos del archivo de audio
    int frames = audio -> frames = info.frames;
    int channels = audio -> channels = info.channels;
    audio -> framerate = info.samplerate;
    audio -> seconds = (double) info.frames / info.samplerate;

    // se alloca memoria para guardar los datos en la estructura, se necesitan
    // frames * channels espacios de tamaño int
    audio -> data = (int *) malloc(frames * channels * sizeof(int));
    // se escriben los datos en la estructura, son frames * channels datos
    sf_read_int(inFile, audio -> data, frames * channels);
    // se cierra el archivo
    sf_close(inFile);
}

// funcion que muestra informacion de el audio
void showInfo(Audio *audio){
    // se imprime informacion relevante de la estructura audio
    printf("Filename: %s\n", audio -> filename);
    printf("Channels: %d\n", audio -> channels);
    printf("Frames: %d\n", audio -> frames);
    printf("Framerate: %d\n", audio -> framerate);
    printf("Durations: %lf\n", audio -> seconds);
}

// funcion que obtiene todos los datos de un canal
void getChannel(int channel, Audio *audio, int **data, Bool allocate){
    // se alloca memoria para los datos
    if(allocate)
        *data = (int *) malloc(sizeof(int) * audio -> frames);

    // se itera sobre el numero de datos
    for (int i = 0; i < audio -> frames * audio -> channels; i += audio -> channels){
            // se escoge el dato en el canal especificado y se guarda en data
            (*data)[i / (audio -> channels)] = audio -> data[i + channel];
    }
}

// funcion que obtiene un determinado numero de datos de un canal desde una posicion determinada
void getChannelAt(int channel, Audio *audio, int initialFrame, int length, int **data, Bool allocate){
    // se alloca memoria para los datos
    if(allocate)
        *data = (int *) malloc(sizeof(int) * length);

    // se itera sobre la cantidad de datos que se quieren leer
    for (int i = 0; i < length; i++){
        // se escoge el canal y se guarda en data
        (*data)[i] = audio -> data[(initialFrame + i) * audio -> channels + channel];
    }
}
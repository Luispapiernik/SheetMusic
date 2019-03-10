// para usar malloc
#include <stdlib.h>

// la libreria sndfile y la funcion print necesita stdio.h
#include <stdio.h>
#include <sndfile.h>

// para la referencia a la estructura audio
#include "audio.h"


void readWavFile(Audio *audio){
    // puntero para abrir el archivo de audio
    SNDFILE *inFile;
    // sf_open, carga la informacion del archivo de audio en esta estructura
    SF_INFO info;

    // como se va a abrir el archivo en solo lectura esto debe ser cero
    info.format = 0;
    // se abre el archivo de nombre audio -> filename
    inFile = sf_open(audio -> filename, SFM_READ, &info);
    // si no se puede abrir el archivo, se sale del programa
    if (inFile == NULL){
        printf("Failed to open the file.\n");
        exit(-1);
    }

    // se llena la estructura audio con los datos del archivo de audio
    int frames = audio -> frames = info.frames;
    int channels = audio -> channels = info.channels;
    audio -> framerate = info.samplerate;

    // se alloca memoria para guardar los datos en la estructura, se necesitan
    // frames * channels espacios de tamaño int
    audio -> data = (int *) malloc(frames * channels * sizeof(int));
    // se escriben los datos en la estructura, son frames * channels datos
    sf_read_int(inFile, audio -> data, frames * channels);
    // se cierra el archivo
    sf_close(inFile);
}


void showInfo(Audio *audio){
    // se imprime informacion relevante de la estructura audio
    printf("Filename: %s\n", audio -> filename);
    printf("Channels: %d\n", audio -> channels);
    printf("Frames: %d\n", audio -> frames);
    printf("Framerate: %d\n", audio -> framerate);

}
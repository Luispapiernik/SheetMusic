#include <stdlib.h>
#include <stdio.h>

#include "audio.h"
#include "frequency.h"
#include "miscellaneous.h"


int main(int argc, char *argv[]){
    // se crea la estructura audio
    Audio audio;

    // se recibe el nombre del archivo por entrada de comandos
    audio.filename = argv[1];

    // se lee el archivo de audio y se carga en la estructura audio
    readWavFile(&audio);

    // se muestra informacion del audio
    showInfo(&audio);

    double *frequencies;
    double seconds = getFrequencies(CHANNEL1, &audio, audio.framerate, &frequencies);

    printf("Segundos: %lf\n", seconds);

    for(int i = 0; i < audio.frames / audio.framerate; i++){
        printf("Frequency: %lf\n", frequencies[i]);
    }

    // se libera memoria
    free(frequencies);
    
    return 0;
}
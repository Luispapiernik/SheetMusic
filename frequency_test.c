#include <stdlib.h>
#include <stdio.h>

#include "miscellaneous.h"
#include "audio.h"
#include "frequency.h"


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
    int length = getFrequencies(CHANNEL1, &audio, audio.framerate, &frequencies);

    printf("Length: %d\n", length);

    for(int i = 0; i < audio.frames / audio.framerate; i++){
        printf("Frequency: %lf\n", frequencies[i]);
    }

    length = getFrequencies(CHANNEL1, &audio, audio.framerate / 2, &frequencies);

    printf("Length: %d\n", length);

    for(int i = 0; i < audio.frames * 2 / audio.framerate; i++){
        printf("Frequency: %lf\n", frequencies[i]);
    }

    // se libera memoria
    free(frequencies);
    
    return 0;
}
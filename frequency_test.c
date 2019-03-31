#include <stdio.h>

#include "audio.h"
#include "frequency.h"


int main(int argc, char const *argv[]){
    // se crea la estructura audio
    Audio audio;

    // se recibe el nombre del archivo por entrada de comandos
    audio.filename = argv[1];

    // se lee el archivo de audio y se carga en la estructura audio
    readWavFile(&audio);

    // se muestra informacion del audio
    showInfo(&audio);

    int *data;
    getChannel(0, &audio, &data, True);

    getFrequencyTest(0, &audio);

    double *frequencies;
    getFrequencies(0, &audio, 0.4 * audio.framerate, &frequencies);

    return 0;
}
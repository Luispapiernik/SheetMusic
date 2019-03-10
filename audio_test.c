#include <stdlib.h>
#include <stdio.h>

#include "audio.h"


int main(int argc, char const *argv[]){
    // se crea la estructura audio
    Audio audio;

    // se recibe el nombre del archivo por entrada de comandos
    audio.filename = argv[1];

    // se lee el archivo de audio y se carga en la estructura audio
    readWavFile(&audio);

    // se muestra informacion del audio
    showInfo(&audio);

    // copia los datos del canal 1 en data
    int *data;
    getChannel(0, &audio, &data);

    // se imprimen los datos
    for (int i = 0; i < audio.frames * audio.channels; i += audio.channels){
        printf("%d - %d\n", data[i / audio.channels], audio.data[i]);
    }

    // se libea la memoria
    free(audio.data);

    return 0;
}

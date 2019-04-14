#include <stdlib.h>
#include <stdio.h>

#include "audio.h"


int main(int argc, char *argv[]){
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
    getChannel(CHANNEL1, &audio, &data, True);

    int *dataAt = (int *) malloc(sizeof(int) * audio.framerate);

    // se imprimen los datos
    for (int i = 0; i < audio.frames; i++){
        if(i % audio.framerate == 0)
            getChannelAt(CHANNEL1, &audio, i, audio.framerate, &dataAt, False);

        printf("%d - %d - %d\n", audio.data[i * audio.channels], data[i], dataAt[i % audio.framerate]);
    }

    // se libea la memoria
    free(audio.data);
    free(data);
    free(dataAt);

    return 0;
}

#include <stdlib.h>

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

    // se libea la memoria
    free(audio.data);

    return 0;
}

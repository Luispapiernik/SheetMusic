#include <stdio.h>

#include "audio.h"
#include "frequency.h"


int main(int argc, char const *argv[]){
    Audio audio;

    audio.filename = argv[1];

    readWavFile(&audio);

    int *data;
    getChannel(0, &audio, &data);

    getFrequency(0, &audio);

    return 0;
}
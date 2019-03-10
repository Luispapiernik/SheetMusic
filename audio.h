#ifndef AUDIO_H
#define AUDIO_H


// Estructura que representa audio
typedef struct important{
    // nombre del archivo desde el que se cargan los datos
    char *filename;
    // datos del archivo
    int *data;

    // numero de frames,
    int frames;
    // canales del audio, mono 1 canal, estereo 2 canales
    int channels;
    // taza de muestreo, frames por segundos
    int framerate;

}Audio;  // se crea un sinonimo de tipo

// funcion que lee un archivo de audio y carga la estructura audio con datos
void readWavFile(Audio *audio);
// funcion que muestra informacion de el audio
void showInfo(Audio *audio);
// funcion que retorna los datos de un canal
void getChannel(int channel, Audio *audio, int **data);

#endif

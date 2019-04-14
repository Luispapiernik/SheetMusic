// para obtener entrada del usuario
#include <stdlib.h>
#include <argp.h>

// para la generacion de la partitura
#include "miscellaneous.h"
#include "audio.h"
#include "frequency.h"
#include "parser.h"

// grupos de los argumentos
#define INPUT 0


// se usa para guardar argumentos ingresados por el usuario
typedef struct{
    char *filename;
    int tempo;
    double dt;
    int channel;
}Arguments;

void slave(Arguments arguments);

error_t parse_opt(int key, char *arg, struct argp_state *state){
    Arguments *arguments = state -> input;

    switch (key){
        case 'f':
            arguments -> filename = arg;
            break;
        case 't':
            arguments -> tempo = atoi(arg);
            break;
        case 'd':
            arguments -> dt = atof(arg);
            break;
        case 'c':
            arguments -> channel = atoi(arg);
            break;

        default:
            return ARGP_ERR_UNKNOWN;
    }

    return 0;
}


int main(int argc, char **argv){
    static char doc[] =
    "Este programa genera una partitura a partir de un arvhivo\
    de audio\
    \vEste programa recibe un archivo de audio en el formato WAV\
    en el que esta grabado el sonido de algun instrumento y a\
    parti de este, se genera un partitura que representa el sonido\
    de lo que se grabo.";

    // opciones 
    static struct argp_option options[] = {
        // archivo de entrada
        {"filename", 'f', "FILENAME", 0, "Archivo de audio", INPUT},
        {"tempo", 't', "TEMPO", 0, "Tempo del audio", INPUT},
        {"dt", 'd', "DT", 0, "Division temporal del audio", INPUT},
        {"channel", 'c', "CHANNEL", 0, "Canal a analizar", INPUT}
    };

    Arguments arguments;

    // se crea el parseador
    static struct argp argp = {options, parse_opt, 0, doc};

    // se parsean los argumentos
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    slave(arguments);

    return 0;
}

void slave(Arguments arguments){
    // se verifica que los argumentos llegaron bien
    printf("filename: %s\n", arguments.filename);
    printf("tempo: %d\n", arguments.tempo);
    printf("dt: %lf\n", arguments.dt);
    printf("channel: %d\n", arguments.channel);

    // lectura del audio
    Audio audio;

    audio.filename = arguments.filename;
    readWavFile(&audio);

    // extraccion de las frecuencias
    double *frequencies;
    int channel = arguments.channel;
    int subLength = seconds2frames(arguments.dt, audio.framerate);
    int length = getFrequencies(channel, &audio, subLength, &frequencies);

    // construccion del parser
    Register reg;
    fillRegister(&reg, 1, 7);

    char string[4];
    for(int i = 0; i < length; i++){
        parseNote(frequencies[i], reg, string);
        printf("Frequency: %lf, Note: %s\n", frequencies[i], string);
    }
}
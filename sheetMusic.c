// para obtener entrada del usuario
#include <stdlib.h>
#include <argp.h>

// para la generacion de la partitura
#include "miscellaneous.h"
#include "audio.h"
#include "frequency.h"
#include "parser.h"

// grupos de los argumentos
#define IO 0
#define FREQUENCY 1
#define PARSER 2


// se usa para guardar argumentos ingresados por el usuario
typedef struct{
    // input output
    char *inputfile;
    char *outputfile;

    // get frequencies
    int channel;
    double dt;
    double roundRatio;

    // parser
    char *title;
    int tempo;
    int notes;
    int measure;
    int initial_eighth;
    int final_eighth;
}Arguments;

void slave(Arguments arguments);

error_t parse_opt(int key, char *arg, struct argp_state *state){
    Arguments *arguments = state -> input;

    switch (key){
        case 'i':
            arguments -> inputfile = arg;
            break;
        case 'o':
            arguments -> outputfile = arg;
            break;
        case 'c':
            arguments -> channel = atoi(arg);
            break;
        case 'd':
            arguments -> dt = atof(arg);
            break;
        case 'z':
            arguments -> roundRatio = atof(arg);
            break;
        case 0:
            arguments -> title = arg;
            break;
        case 't':
            arguments -> tempo = atoi(arg);
            break;
        case 'n':
            arguments -> notes = atoi(arg);
            break;
        case 'm':
            arguments -> measure = atoi(arg);
            break;
        case 'p':
            arguments -> initial_eighth = atoi(arg);
            break;
        case 'f':
            arguments -> final_eighth = atoi(arg);
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
        {"inputfile", 'i', "FILENAME", 0, "archivo de audio", IO},
        {"outputfile", 'o', "FILENAME", 0, "nombre del archivo de salida", IO},
        {"channel", 'c', "CHANNEL", 0, "canal a analizar", FREQUENCY},
        {"dt", 'd', "DT", 0, "division temporal del audio", FREQUENCY},
        {"round", 'z', "ROUNDRATIO", 0, "especifica a como redondear el tiempo", FREQUENCY},
        {"title", 0, "TITLE", 0, "titulo de la partitura", PARSER},
        {"tempo", 't', "TEMPO", 0, "tempo de la partitura", PARSER},
        {"notes", 'n', "NOTES", 0, "notas por compas", PARSER},
        {"measure", 'm', "MEASURE", 0, "medida del compas", PARSER},
        {"initial-eighth", 'p', "INITIAL_EIGHTH", 0, "Tempo del audio", PARSER},
        {"final-eighth", 'f', "FINAL_EIGHTH", 0, "Tempo del audio", PARSER}
    };

    // se crea el parseador
    static struct argp argp = {options, parse_opt, 0, doc};

    Arguments arguments;
    // valores por defecto
    arguments.outputfile = "output";
    arguments.channel = 0;
    arguments.dt = 0.0625;
    arguments.roundRatio = 1;
    arguments.title = "Title";
    arguments.tempo = 60;
    arguments.notes = 4;
    arguments.measure = 4;
    arguments.initial_eighth = 1;
    arguments.final_eighth = 6;

    // se parsean los argumentos
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    slave(arguments);

    return 0;
}

void slave(Arguments arguments){
    // se verifica que los argumentos llegaron bien
    printf("===================informacion argumentos==================\n");
    printf("inputfile: %s\n", arguments.inputfile);
    printf("outputfile: %s\n", arguments.outputfile);
    printf("channel: %d\n", arguments.channel);
    printf("dt: %lf\n", arguments.dt);
    printf("roundRatio: %lf\n", arguments.roundRatio);
    printf("tempo: %d\n", arguments.tempo);
    printf("initial eighth: %d\n", arguments.initial_eighth);
    printf("final eighth: %d\n", arguments.final_eighth);

    // lectura del audio
    Audio audio;

    audio.filename = arguments.inputfile;
    readWavFile(&audio);

    printf("================informacion audio===================\n");
    showInfo(&audio);
    printf("====================================================\n");

    // extraccion de las frecuencias
    double *frequencies;
    int channel = arguments.channel;
    int subLength = seconds2frames(arguments.dt, audio.framerate);
    int length = getFrequencies(channel, &audio, subLength, &frequencies);

    // construccion del parser
    Register reg;
    fillRegister(&reg, arguments.initial_eighth, arguments.final_eighth);

    // informacion del music sheet
    MusicSheetInfo info;
    info.filename = arguments.outputfile;
    info.title = arguments.title;
    info.tempo = arguments.tempo;
    info.notes = arguments.notes;
    info.measure = arguments.measure;

    parseFrequencies(info, length, arguments.dt, frequencies, arguments.roundRatio, reg);

    free(frequencies);

    char comand1[500];
    char comand2[500];
    sprintf(comand1, "lilypond %s", arguments.outputfile);
    system(comand1);

    sprintf(comand2, "evince %s.pdf", arguments.outputfile);
    system(comand2);
}
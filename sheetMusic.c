#include <argp.h>

// grupos de los argumentos
#define INPUT 0


// se usa para guardar argumentos ingresados por el usuario
typedef struct{
    char *filename;
}Arguments;


error_t parse_opt(int key, char *arg, struct argp_state *state){
    Arguments *arguments = state -> input;

    switch (key){
        case 'f':
            arguments -> filename = arg;
            break;

        default:
            return ARGP_ERR_UNKNOWN;
    }

    return 0;
}


void getArgumentsValues(int argc, char **argv){
    
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
        {"filename", 'f', "FILENAME", 0, "Produce verbose output", INPUT},
    };

    Arguments arguments;

    // se crea el parseador
    static struct argp argp = {options, parse_opt, 0, doc};

    // se parsean los argumentos
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    printf("filename: %s\n", arguments.filename);

    return 0;
}
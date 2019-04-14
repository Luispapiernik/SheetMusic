#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parser.h"
#include "miscellaneous.h"

void getNote(int note, int eighth, char *string);


void fillRegister(Register *reg, int initial_eighth, int final_eighth){
    int n = final_eighth - initial_eighth + 1;

    reg -> length = n * 12;

    reg -> frequencies[0] = (double *) malloc(sizeof(double) * n * 12);
    reg -> frequencies[1] = (double *) malloc(sizeof(double) * n * 12);

    reg -> notes = (char **) malloc(sizeof(char *) * n * 12);

    double freq = 0;
    for(int eighth = initial_eighth; eighth < final_eighth + 1; eighth++){
        for(int note = 1; note < 13; note++){
            freq = frequency(note, eighth);

            reg -> frequencies[0][(eighth - initial_eighth) * 12 + note - 1] = freq;
            reg -> frequencies[1][(eighth - initial_eighth) * 12 + note - 1] = freq;
            
            reg -> notes[(eighth - initial_eighth) * 12 + note - 1] = (char *) malloc(sizeof(char) * 4);
            getNote(note, eighth, reg -> notes[(eighth - initial_eighth) * 12 + note - 1]);
        }
    }

    (reg -> notNote)[0] = ' ';
    (reg -> notNote)[0] = '\0';

    double upperLimit;
    for(int i = 0; i < reg -> length - 1; i++){
        upperLimit = (reg -> frequencies[0][i + 1] + reg -> frequencies[0][i]) / 2.0;
        reg -> frequencies[1][i] = upperLimit;
        reg -> frequencies[0][i + 1] = upperLimit;
    }
}


void parseNote(double frequency, Register reg, char *string){
    for(int i = 0; i < reg.length; i++){
        if((reg.frequencies[0][i] <= frequency) && (frequency < reg.frequencies[1][i])){
            strcpy(string, reg.notes[i]);
            return;
        }
    }

   strcpy(string, reg.notNote);
}


void getNote(int note, int eighth, char *string){
    switch (note){
        case C:
            sprintf(string, "C%d", eighth);
            break;
        case CS:
            sprintf(string, "C#%d", eighth);
            break;
        case D:
            sprintf(string, "D%d", eighth);
            break;
        case DS:
            sprintf(string, "D#%d", eighth);
            break;
        case E:
            sprintf(string, "E%d", eighth);
            break;
        case F:
            sprintf(string, "F%d", eighth);
            break;
        case FS:
            sprintf(string, "F#%d", eighth);
            break;
        case G:
            sprintf(string, "G%d", eighth);
            break;
        case GS:
            sprintf(string, "G#%d", eighth);
            break;
        case A:
            sprintf(string, "A%d", eighth);
            break;
        case AS:
            sprintf(string, "A#%d", eighth);
            break;
        case B:
            sprintf(string, "B%d", eighth);
            break;
    }
}

MusicalTime seconds2MusicalTime(double seconds, int tempo){
    MusicalTime t;
    t.seconds = seconds;
    t.tempo = tempo;

    // equivalente de semifusa en segundos
    double semifusaSeconds = (0.0625 * 60) / tempo;

    int semifusaNumber = intQuotient(seconds, semifusaSeconds);

    // en una redonda hay 64 semifusas
    t.REDONDA = semifusaNumber / 64;
    semifusaNumber = semifusaNumber % 64;

    // en una blanca hay 32 semifusas
    t.BLANCA = semifusaNumber / 32;
    semifusaNumber = semifusaNumber % 32;

    // en una negra hay 16 semifusas
    t.NEGRA = semifusaNumber / 16;
    semifusaNumber = semifusaNumber % 16;

    // en una corchea hay 8 semifusas
    t.CORCHEA = semifusaNumber / 8;
    semifusaNumber = semifusaNumber % 8;

    // en una semicorchea hay 4 semifusas
    t.SEMICORCHEA = semifusaNumber / 4;
    semifusaNumber = semifusaNumber % 4;

    // en una fusa hay 2 semifusas
    t.FUSA = semifusaNumber / 2;
    semifusaNumber = semifusaNumber % 2;

    t.SEMIFUSA = semifusaNumber;

    return t;
}


void showMusicalTime(MusicalTime t){
    printf("Seconds: %lf\n", t.seconds);
    printf("Tempo: %d\n", t.tempo);

    printf("REDONDAS: %d\n", t.REDONDA);
    printf("BLANCAS: %d\n", t.BLANCA);
    printf("NEGRAS: %d\n", t.NEGRA);
    printf("CORCHEAS: %d\n", t.CORCHEA);
    printf("SEMICORCHEAS: %d\n", t.SEMICORCHEA);
    printf("FUSAS: %d\n", t.FUSA);
    printf("SEMIFUSAS: %d\n", t.SEMIFUSA);
}


void parseFrequencies(int length, double *frequencies, char **filechar){

}
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parser.h"
#include "miscellaneous.h"


void note2string(Note note, char *string){
    switch (note.note){
        case C:
            sprintf(string, "C%d", note.eighth);
            break;
        case CS:
            sprintf(string, "C#%d", note.eighth);
            break;
        case D:
            sprintf(string, "D%d", note.eighth);
            break;
        case DS:
            sprintf(string, "D#%d", note.eighth);
            break;
        case E:
            sprintf(string, "E%d", note.eighth);
            break;
        case F:
            sprintf(string, "F%d", note.eighth);
            break;
        case FS:
            sprintf(string, "F#%d", note.eighth);
            break;
        case G:
            sprintf(string, "G%d", note.eighth);
            break;
        case GS:
            sprintf(string, "G#%d", note.eighth);
            break;
        case A:
            sprintf(string, "A%d", note.eighth);
            break;
        case AS:
            sprintf(string, "A#%d", note.eighth);
            break;
        case B:
            sprintf(string, "B%d", note.eighth);
            break;
        default:
            strcpy(string, " ");
            break;
    }
}


void showNote(Note note){
    char string[5];

    note2string(note, string);

    printf("%s\n", string);
}

void showRegister(Register reg){
    printf("length: %d\n", reg.length);

    char string[5];

    for(int i = 0; i < reg.length; i++){
        note2string(reg.notes[i], string);
        printf("%s - ", string);
        printf("%lf %lf\n", reg.frequencies[0][i], reg.frequencies[1][i]);
    }
}

void showMusicalTime(MusicalTime t){
    printf("TIME\n");
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


void fillRegister(Register *reg, int initial_eighth, int final_eighth){
    int n = final_eighth - initial_eighth + 1;

    reg -> length = n * 12;

    reg -> frequencies[0] = (double *) malloc(sizeof(double) * n * 12);
    reg -> frequencies[1] = (double *) malloc(sizeof(double) * n * 12);

    reg -> notes = (Note *) malloc(sizeof(Note) * n * 12);

    double freq = 0;
    for(int eighth = initial_eighth; eighth < final_eighth + 1; eighth++){
        for(int note = 1; note < 13; note++){
            freq = frequency(note, eighth);

            reg -> frequencies[0][(eighth - initial_eighth) * 12 + note - 1] = freq;
            reg -> frequencies[1][(eighth - initial_eighth) * 12 + note - 1] = freq;
            
            reg -> notes[(eighth - initial_eighth) * 12 + note - 1] = (Note) {note, eighth};
        }
    }

    reg -> notNote = (Note) {0, 0};

    double upperLimit;
    for(int i = 0; i < reg -> length - 1; i++){
        upperLimit = (reg -> frequencies[0][i + 1] + reg -> frequencies[0][i]) / 2.0;
        reg -> frequencies[1][i] = upperLimit;
        reg -> frequencies[0][i + 1] = upperLimit;
    }
}


Note parseFrequency(double frequency, Register reg){
    for(int i = 0; i < reg.length; i++){
        if((reg.frequencies[0][i] <= frequency) && (frequency < reg.frequencies[1][i])){
            return reg.notes[i];
        }
    }

  return reg.notNote;
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


void parseNote(Note note, MusicalTime t, char *string){
    string = (char *) malloc(sizeof(char) * 10);
}

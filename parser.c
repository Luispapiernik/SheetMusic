#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parser.h"
#include "miscellaneous.h"

void getNote(int note, int eighth, char *string);


void fillRegister(Register *reg){
    int n = reg -> final_eighth - reg -> initial_eighth + 1;

    reg -> length = n * 12;

    reg -> frequencies[0] = (double *) malloc(sizeof(double) * n * 12);
    reg -> frequencies[1] = (double *) malloc(sizeof(double) * n * 12);

    reg -> notes = (char **) malloc(sizeof(char *) * n * 12);

    double freq = 0;
    for(int eighth = reg -> initial_eighth; eighth < reg -> final_eighth + 1; eighth++){
        for(int note = 1; note < 13; note++){
            freq = frequency(note, eighth);

            reg -> frequencies[0][(eighth - reg -> initial_eighth) * 12 + note] = freq;
            reg -> frequencies[1][(eighth - reg -> initial_eighth) * 12 + note] = freq;
            
            reg -> notes[(eighth - reg -> initial_eighth) * 12 + note] = (char *) malloc(sizeof(char) * 4);
            getNote(note, eighth, reg -> notes[(eighth - reg -> initial_eighth) * 12 + note]);

            printf("%lf\n", freq);
        }
    }

    (reg -> notNote)[0] = ' ';
    (reg -> notNote)[0] = '\0';

    double upperLimit;
    for(int i = 0; i < reg -> length - 1; i++){
        upperLimit = (reg -> frequencies[0][i + 1] + reg -> frequencies[0][i]) / 2.0;
        reg -> frequencies[1][i] = upperLimit;
    }

    for(int i = 1; i < reg -> length; i++){
        reg -> frequencies[0][i] = reg -> frequencies[1][i - 1];
    }
}


void parseNote(double frequency, Register reg, char *string){
    for(int i = 1; i < reg.length; i++){
        if((reg.frequencies[0][i] <= frequency) && (frequency < reg.frequencies[1][i])){
            strcpy(string, reg.notes[i]);
            return;
        }
    }

   strcpy(string, reg.notNote);
}


void getNote(int note, int eighth, char *string){
    switch (note){
        case 1:
            sprintf(string, "C%d", eighth);
            break;
        case 2:
            sprintf(string, "C#%d", eighth);
            break;
        case 3:
            sprintf(string, "D%d", eighth);
            break;
        case 4:
            sprintf(string, "D#%d", eighth);
            break;
        case 5:
            sprintf(string, "E%d", eighth);
            break;
        case 6:
            sprintf(string, "F%d", eighth);
            break;
        case 7:
            sprintf(string, "F#%d", eighth);
            break;
        case 8:
            sprintf(string, "G%d", eighth);
            break;
        case 9:
            sprintf(string, "G#%d", eighth);
            break;
        case 10:
            sprintf(string, "A%d", eighth);
            break;
        case 11:
            sprintf(string, "A#%d", eighth);
            break;
        case 12:
            sprintf(string, "B%d", eighth);
            break;
    }
}
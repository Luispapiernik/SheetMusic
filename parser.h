#ifndef PARSER_H
#define PARSER_H

#include "miscellaneous.h"

// definicion de las notas musicales
#define NOTNOTE 0
#define C 1
#define CS 2
#define D 3
#define DS 4
#define E 5
#define F 6
#define FS 7
#define G 8
#define GS 9
#define A 10
#define AS 11
#define B 12

typedef struct{
    int note;
    int eighth;
}Note;

typedef struct{
    int length;

    double *frequencies[2];
    Note *notes;

    Note notNote;
}Register;

typedef struct{
    double seconds;
    double remainder;
    int tempo;

    int REDONDA;
    int BLANCA;
    int NEGRA;
    int CORCHEA;
    int SEMICORCHEA;
    int FUSA;
    int SEMIFUSA;
}MusicalTime;

void showNote(Note note);
void showRegister(Register reg);
void showMusicalTime(MusicalTime t);

void fillRegister(Register *reg, int initial_eighth, int final_eighth);
Note parseFrequency(double frequency, Register reg);

// esta funcion recibe tiempo en segundos y lo convierte a tiempo musical
MusicalTime seconds2MusicalTime(double seconds, int tempo);

void parseNote(Note note, MusicalTime t, char *string);

#endif

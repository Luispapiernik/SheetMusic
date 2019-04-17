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

// definicion de tiempo musical
#define REDONDA_ 0
#define REDONDA 1
#define BLANCA_ 2
#define BLANCA 3
#define NEGRA_ 4
#define NEGRA 5
#define CORCHEA_ 6
#define CORCHEA 7
#define SEMICORCHEA_ 8
#define SEMICORCHEA 9
#define FUSA_ 10
#define FUSA 11
#define SEMIFUSA 12

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

    int redonda;
    int blanca;
    int negra;
    int corchea;
    int semicorchea;
    int fusa;
    int semifusa;
}MusicalTime;

void showNote(Note note);
void showRegister(Register reg);
void showMusicalTime(MusicalTime t);

void fillRegister(Register *reg, int initial_eighth, int final_eighth);
Note parseFrequency(double frequency, Register reg);

// esta funcion recibe tiempo en segundos y lo convierte a tiempo musical
MusicalTime seconds2MusicalTime(double seconds, int tempo);

void parseNote(Note note, MusicalTime t, char **string);

void parseFrequencies(char *filename, int length, double seconds, int tempo, double *frequencies, Register reg);

#endif

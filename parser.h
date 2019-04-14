#ifndef PARSER_H
#define PARSER_H

// definicion de las notas musicales
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
    int length;

    double *frequencies[2];
    char **notes;
    char notNote[2];
}Register;

typedef struct{
    double seconds;
    int tempo;

    int REDONDA;
    int BLANCA;
    int NEGRA;
    int CORCHEA;
    int SEMICORCHEA;
    int FUSA;
    int SEMIFUSA;
}MusicalTime;

void fillRegister(Register *reg, int initial_eighth, int final_eighth);
void parseNote(double frequency, Register reg, char *string);

// esta funcion recibe tiempo en segundos y lo convierte a tiempo musical
MusicalTime seconds2MusicalTime(double seconds, int tempo);

void showMusicalTime(MusicalTime t);

#endif

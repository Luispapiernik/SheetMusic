// estas son guradas, evitan que el archivo sea incluido mas
// de 2 veces en un mismo archivo
#ifndef PARSER_H
#define PARSER_H

#include "miscellaneous.h"

// definicion de las notas musicales, para
// lejibilidad e el codigo
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

// definicion de tiempo musical, para
// legibilidad en el codigo
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

// estructura que representa una nota musical
typedef struct{
    // numero de la nota(C = 1, C# = 2,..., B = 12)
    int note;
    // numero que representa la octava de la nota
    // va desde 1 en adelante
    int eighth;
}Note;  // se crea sinonimo de tipo

// estructura que representa un registro en musica, esto es
// un rango de notas, se usa para traducir de frecuencias a
// notas musicales
typedef struct{
    // numero de notas
    int length;

    // array con frecuencias minimas y maximas entre las cuales
    // esta cada nota
    double *frequencies[2];
    // notas del registro
    Note *notes;

    // representacion de falta de nota, es decir, no hay nota
    Note notNote;
}Register;  // se crea sinonimo de tipo

// estructura que representa tiempo musical
typedef struct{
    // tiempo en segundos
    double seconds;
    // cantidad de tiempo que no puede ser mapeada a tiempo musical
    double remainder;
    // tempo(notas ejecutadas por minuto, ej: 4 = 60 es 60 negras por minuto,
    // 8 = 90 es 90 corcheas por minuto)
    int tempo;

    // numero de semifusas que representan el tiempo
    int semifusaNumber;

    // enteros que representan cuantas figuras de cada nota cubren el tiempo
    int redonda;
    int blanca;
    int negra;
    int corchea;
    int semicorchea;
    int fusa;
    int semifusa;
}MusicalTime;  // se crea sinonimo de tipo

// estructura que guarda informacion del archivo de salidad del parse
typedef struct{
    // nombre del archivo de salida
    char *filename;
    // version de lilypond usada
    char *version;

    // header
    char *title;  // titulo del archivo de salida
    char *composer;  // compositor
    
    // body
    int tempo;  // tempo de la partitura
    int notes;  // numero de notas por compas
    int measure;  // medida del tiempo
}MusicSheetInfo;  // se crea sinonimo de tipo

// estas funciones muestran en pantalla informacon de las estructuras
void showNote(Note note);
void showRegister(Register reg);
void showMusicalTime(MusicalTime t);

// esta funcion llena un registro musical con notas que estan entre las
// octavas especificadas
void fillRegister(Register *reg, int initial_eighth, int final_eighth);
// esta funcion convierte una frecuencia en una nota musical
Note parseFrequency(double frequency, Register reg);

// esta funcion recibe tiempo en segundos y lo convierte a tiempo musical
MusicalTime seconds2MusicalTime(double seconds, int tempo, double round);

// esta funcion recibe una nota y un tiempo musical y retorna la
// representacion en sintasix de lilypond
void parseNote(Note note, MusicalTime t, char **string);

// esta funcion genera la partitura a partir de informacion y frecuencias
void parseFrequencies(MusicSheetInfo info, int length, double seconds, double *frequencies, double roundRatio, Register reg);


// solo para test
int MusicalTimeToSemiFusa(MusicalTime t);
int getNumberOfNotes(MusicalTime t, int *times);
int intToSemiFusa(int note);
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "parser.h"
#include "miscellaneous.h"


void note2string(Note note, char **string){
    *string = (char *) malloc(sizeof(char) * (4 + abs(note.eighth - 4)));

    // se escribe el nombre
    switch (note.note){
        case C:
            strcpy(*string, "c");
            break;
        case CS:
            strcpy(*string, "cis");
            break;
        case D:
            strcpy(*string, "d");
            break;
        case DS:
            strcpy(*string, "dis");
            break;
        case E:
            strcpy(*string, "e");
            break;
        case F:
            strcpy(*string, "f");
            break;
        case FS:
            strcpy(*string, "fis");
            break;
        case G:
            strcpy(*string, "g");
            break;
        case GS:
            strcpy(*string, "gis");
            break;
        case A:
            strcpy(*string, "a");
            break;
        case AS:
            strcpy(*string, "ais");
            break;
        case B:
            strcpy(*string, "b");
            break;
        default:
            strcpy(*string, "r");
            return;
    }

    // se le agrega identificador de la octava
    for(int i = 0; i < abs(note.eighth - 4); i++){
        if (note.eighth < 4) {
            strcat(*string, ",");
        }else{
            strcat(*string, "'");
        }
    }
}


void showNote(Note note){
    char *string;
    note2string(note, &string);

    printf("%s\n", string);

    free(string);
}

void showRegister(Register reg){
    printf("length: %d\n", reg.length);

    char *string;

    for(int i = 0; i < reg.length; i++){
        note2string(reg.notes[i], &string);
        printf("%s - ", string);
        printf("%lf %lf\n", reg.frequencies[0][i], reg.frequencies[1][i]);
    }

    free(string);
}

void showMusicalTime(MusicalTime t){
    printf("TIME\n");
    printf("Seconds: %lf\n", t.seconds);
    printf("Remainder: %lf\n", t.remainder);
    printf("Tempo: %d\n", t.tempo);

    printf("REDONDAS: %d\n", t.redonda);
    printf("BLANCAS: %d\n", t.blanca);
    printf("NEGRAS: %d\n", t.negra);
    printf("CORCHEAS: %d\n", t.corchea);
    printf("SEMICORCHEAS: %d\n", t.semicorchea);
    printf("FUSAS: %d\n", t.fusa);
    printf("SEMIFUSAS: %d\n", t.semifusa);
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

    t.remainder = remainder(seconds, semifusaSeconds);
    int semifusaNumber = intQuotient(seconds, semifusaSeconds);

    // en una redonda hay 64 semifusas
    t.redonda = semifusaNumber / 64;
    semifusaNumber = semifusaNumber % 64;

    // en una blanca hay 32 semifusas
    t.blanca = semifusaNumber / 32;
    semifusaNumber = semifusaNumber % 32;

    // en una negra hay 16 semifusas
    t.negra = semifusaNumber / 16;
    semifusaNumber = semifusaNumber % 16;

    // en una corchea hay 8 semifusas
    t.corchea = semifusaNumber / 8;
    semifusaNumber = semifusaNumber % 8;

    // en una semicorchea hay 4 semifusas
    t.semicorchea = semifusaNumber / 4;
    semifusaNumber = semifusaNumber % 4;

    // en una fusa hay 2 semifusas
    t.fusa = semifusaNumber / 2;
    semifusaNumber = semifusaNumber % 2;

    t.semifusa = semifusaNumber;

    return t;
}


int getNumberOfNotes(MusicalTime t, int *times){
    int number = 0;

    if (t.redonda){
        number += t.redonda;
        times[REDONDA] = t.redonda;
        if(t.blanca){
            number += t.blanca - 1;
            times[BLANCA] = t.blanca - 1;
            times[REDONDA_] = 1;
            times[REDONDA]--;
            t.blanca = 0;
        }
    }

    if (t.blanca){
        number += t.blanca;
        times[BLANCA] = t.blanca;
        if(t.negra){
            number += t.negra - 1;
            times[NEGRA] = t.blanca - 1;
            times[BLANCA_] = 1;
            times[BLANCA]--;
            t.negra = 0;
        }
    }

    if (t.negra){
        number += t.negra;
        times[NEGRA] = t.negra;
        if(t.corchea){
            number += t.corchea - 1;
            times[CORCHEA] = t.blanca - 1;
            times[NEGRA_] = 1;
            times[NEGRA]--;
            t.corchea = 0;
        }
    }

    if (t.corchea){
        number += t.corchea;
        times[CORCHEA] = t.corchea;
        if(t.semicorchea){
            number += t.semicorchea - 1;
            times[SEMICORCHEA] = t.blanca - 1;
            times[CORCHEA_] = 1;
            times[CORCHEA]--;
            t.semicorchea = 0;
        }
    }

    if (t.semicorchea){
        number += t.semicorchea;
        times[SEMICORCHEA] = t.semicorchea;
        if(t.fusa){
            number += t.fusa - 1;
            times[FUSA] = t.blanca - 1;
            times[SEMICORCHEA_] = 1;
            times[SEMICORCHEA]--;
            t.fusa = 0;
        }
    }

    if (t.fusa){
        number += t.fusa;
        times[FUSA] = t.fusa;
        if(t.semifusa){
            number += t.semifusa - 1;
            times[SEMIFUSA] = t.blanca - 1;
            times[FUSA_] = 1;
            times[SEMIFUSA]--;
            t.semifusa = 0;
        }
    }

    number += t.semifusa;
    times[SEMIFUSA] = t.semifusa;

    return number;
}


void parseNote(Note note, MusicalTime t, char **string){
    // En la longitud del string se considera el nombre de la nota
    // si es sostenida o bemol, el tiempo y la octava en la que esta
    int times[13] = {0};
    int noteNumbers = getNumberOfNotes(t, times);

    int stringLength = (1 + 2 + 3 + abs(note.eighth - 4)) * noteNumbers + 1;

    *string = (char *) malloc(sizeof(char) * stringLength);
    char *noteName;
    note2string(note, &noteName);

    // se inicializa el string
    strcpy(*string, "");

    if (times[REDONDA_]){
        // se escribe el nombre de la nota
        strcat(*string, noteName);
        strcat(*string, "1. ");
    }

    if (times[REDONDA]){
        for(int i = 0; i < times[REDONDA]; i++){
            // se escribe el nombre de la nota
            strcat(*string, noteName);
            strcat(*string, "1 ");
        }
    }

    if (times[BLANCA_]){
        // se escribe el nombre de la nota
        strcat(*string, noteName);
        strcat(*string, "2. ");
    }

    if (times[BLANCA]){
        for(int i = 0; i < times[BLANCA]; i++){
            // se escribe el nombre de la nota
            strcat(*string, noteName);
            strcat(*string, "2 ");
        }
    }

    if (times[NEGRA_]){
        // se escribe el nombre de la nota
        strcat(*string, noteName);
        strcat(*string, "4. ");
    }

    if (times[NEGRA]){
        for(int i = 0; i < times[NEGRA]; i++){
            // se escribe el nombre de la nota
            strcat(*string, noteName);
            strcat(*string, "4 ");
        }
    }

    if (times[CORCHEA_]){
        // se escribe el nombre de la nota
        strcat(*string, noteName);
        strcat(*string, "8. ");
    }

    if (times[CORCHEA]){
        for(int i = 0; i < times[CORCHEA]; i++){
            // se escribe el nombre de la nota
            strcat(*string, noteName);
            strcat(*string, "8 ");
        }
    }

    if (times[SEMICORCHEA_]){
        // se escribe el nombre de la nota
        strcat(*string, noteName);
        strcat(*string, "16. ");
    }

    if (times[SEMICORCHEA]){
        for(int i = 0; i < times[SEMICORCHEA]; i++){
            // se escribe el nombre de la nota
            strcat(*string, noteName);
            strcat(*string, "16 ");
        }
    }

    if (times[FUSA_]){
        // se escribe el nombre de la nota
        strcat(*string, noteName);
        strcat(*string, "32. ");
    }

    if (times[FUSA]){
        for(int i = 0; i < times[FUSA]; i++){
            // se escribe el nombre de la nota
            strcat(*string, noteName);
            strcat(*string, "32 ");
        }
    }

    if (times[SEMIFUSA]){
        for(int i = 0; i < times[SEMIFUSA]; i++){
            // se escribe el nombre de la nota
            strcat(*string, noteName);
            strcat(*string, "64 ");
        }
    }

    free(noteName);
}


void joinFrequencies(int length, double *frequencies, int *account){
    int index = 0;

    for(int i = 0; i < length; i++){
        account[i] = 0;
    }
    

    while(index < length){
        for(int i = index; i < length; i++){
            if (frequencies[i] != frequencies[i + 1]){
                if (frequencies[i] != frequencies[i + 1])
                    account[index]++;

                index = i;
                break;
            }
            account[index]++;
        }
        index++;
    }
}


void parseFrequencies(MusicSheetInfo info, int length, double seconds, double *frequencies, Register reg){
    FILE *file = fopen(info.filename, "w");

    fprintf(file, "\\version \"2.18.2\"\n");
    fprintf(file, "{\n");
    fprintf(file, "\\tempo 4 = %d\n", info.tempo);

    int account[length];
    joinFrequencies(length, frequencies, account);

    for(int i = 0; i < length; i++){
        if (account[i]){
            char *string;

            Note note = parseFrequency(frequencies[i], reg);
            MusicalTime t = seconds2MusicalTime(account[i] * seconds, info.tempo);
            parseNote(note, t, &string);
            fprintf(file, "%s", string);

            free(string);
        }
    }

    fprintf(file, "\n}\n");
    fclose(file);
}
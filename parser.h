#ifndef PARSER_H
#define PARSER_H

typedef struct{
    int length;

    int initial_eighth;
    int final_eighth;

    double *frequencies[2];
    char **notes;
    char notNote[2];
}Register;

void fillRegister(Register *reg);
void parseNote(double frequency, Register reg, char *string);

#endif

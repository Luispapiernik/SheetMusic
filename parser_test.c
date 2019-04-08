#include <stdio.h>
#include "parser.h"


int main(int argc, char *argv[]){
    Register reg;

    reg.initial_eighth = 1;
    reg.final_eighth = 5;

    fillRegister(&reg);

    printf("length: %d", reg.length);

    for(int i = 0; i < reg.length; i++){
        printf("%s - ", reg.notes[i]);
        printf("%lf %lf\n", reg.frequencies[0][i], reg.frequencies[1][i]);
    }

    char string[4];
    parseNote(33, reg, string);
    printf("%s\n", string);
}
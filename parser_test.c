#include <stdio.h>
#include "parser.h"


int main(int argc, char *argv[]){
    // test parse frequency
    Register reg;

    reg.initial_eighth = 1;
    reg.final_eighth = 3;

    fillRegister(&reg);

    printf("Test Frequency Parser\n");
    printf("length: %d\n", reg.length);

    for(int i = 0; i < reg.length; i++){
        printf("%s - ", reg.notes[i]);
        printf("%lf %lf\n", reg.frequencies[0][i], reg.frequencies[1][i]);
    }

    char string[4];
    parseNote(33, reg, string);
    printf("%lf %s\n", 33.0, string);

    // test parse time
    printf("\nTest Time Parser\n");
    showMusicalTime(seconds2MusicalTime(1, 60));
    showMusicalTime(seconds2MusicalTime(1.75, 120));

}
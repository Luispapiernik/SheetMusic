#include <stdio.h>
#include "parser.h"


int main(int argc, char *argv[]){
    // test parse frequency
    Register reg;

    fillRegister(&reg, 1, 3);

    printf("Test Frequency Parser\n");
    showRegister(reg);

    Note note = parseFrequency(33, reg);
    showNote(note);

    note = parseFrequency(20, reg);
    showNote(note);

    // test parse time
    printf("\nTest Time Parser\n");
    showMusicalTime(seconds2MusicalTime(1, 60));
    showMusicalTime(seconds2MusicalTime(1.75, 120));

    // test para parse note
    char *string;
    parseNote(parseFrequency(33, reg), seconds2MusicalTime(1.75, 120), &string);

    printf("%s\n", string);

}
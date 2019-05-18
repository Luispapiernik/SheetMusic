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

    printf("===========================================\n");
    // test parser con checkeo de compas
    MusicalTime t = seconds2MusicalTime(0.187500, 60);

    showMusicalTime(t);

    printf("tiempo musical: %d\n", MusicalTimeToSemiFusa(t));

    int times[13] = {0};

    int length = getNumberOfNotes(t, times);
    printf("longitud de notas: %d\n", length);
    for (int i = 0; i < 13; i++){
        printf("tiempo: %d\n", times[i]);
    }

    printf("================================");
    printf("test int to semifusa\n");
    printf("redonda: %d\n", intToSemiFusa(1));
    printf("blanca: %d\n", intToSemiFusa(2));
    printf("negra: %d\n", intToSemiFusa(4));
    printf("corchea: %d\n", intToSemiFusa(8));
    printf("semicorchea: %d\n", intToSemiFusa(16));
    printf("fusa: %d\n", intToSemiFusa(32));
    printf("semifusa: %d\n", intToSemiFusa(64));

    t = seconds2MusicalTime(2.1, 70);
    showMusicalTime(t);

    printf("- semifusas: %d\n", MusicalTimeToSemiFusa(t));

    // equivalente de semifusa en segundos
    double semifusaSeconds = (0.0625 * 60) / t.tempo;

    MusicalTime t2 = seconds2MusicalTime(MusicalTimeToSemiFusa(t) * semifusaSeconds, t.tempo);
    showMusicalTime(t2);
    printf("- semifusas: %d\n", MusicalTimeToSemiFusa(t2));

}
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "parser.h"
#include "miscellaneous.h"


// esta funcion retorna por referencia la representacion en string de una nota
void note2string(Note note, char **string){
    *string = (char *) malloc(sizeof(char) * (4 + abs(note.eighth - 4)));

    // se escribe el nombre de la nota
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

// funcion que muestra informacion de variables de tipo Note
void showNote(Note note){
    char *string;
    note2string(note, &string);

    printf("%s\n", string);

    free(string);
}

// funcion que muestra informacion de variables de tipo Register
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

// funcion que muestra informacion de variables de tipo MusicalTime
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

// esta funcion llena un registro musical con notas que estan entre las
// octavas especificadas
void fillRegister(Register *reg, int initial_eighth, int final_eighth){
    // separacion entre las octavas
    int n = final_eighth - initial_eighth + 1;

    // el numero de notas del registro es el numero de notas por octava
    // multiplicado por el numero de octavas entre las octavas inicial y final
    reg -> length = n * 12;

    // se consigue espacio para las frecuencias, frequencies[0] array con
    // frecuencias minimas, frequencies[1] array con frecuencias maximas
    reg -> frequencies[0] = (double *) malloc(sizeof(double) * n * 12);
    reg -> frequencies[1] = (double *) malloc(sizeof(double) * n * 12);

    // se consigue espacio para el array de notas
    reg -> notes = (Note *) malloc(sizeof(Note) * n * 12);

    // se inicializa variable donde se guardaran las frecuencias
    double freq = 0;
    // se itera sobre el numero de octavas
    for(int eighth = initial_eighth; eighth < final_eighth + 1; eighth++){
        // se itera sobre el numero de notas
        for(int note = 1; note < 13; note++){
            // se consigue la frecuencia de la nota note en de la octaba eighth
            freq = frequency(note, eighth);

            // se guardan las frecuencias, aqui frecuencia minima es igual a la maxima
            reg -> frequencies[0][(eighth - initial_eighth) * 12 + note - 1] = freq;
            reg -> frequencies[1][(eighth - initial_eighth) * 12 + note - 1] = freq;
            
            // se guardan las notas
            reg -> notes[(eighth - initial_eighth) * 12 + note - 1] = (Note) {note, eighth};
        }
    }

    // referencia a no nota
    reg -> notNote = (Note) {0, 0};

    double upperLimit;
    // se itera sobre el numero de frecuencias
    for(int i = 0; i < reg -> length - 1; i++){
        // se recalcula la frecuencia minima y la frecuencia maxima, cumpliendo que
        // frecuencies[0][i] <= frequency(notes[i].note, notes[i].eighth) < frecuencies[1][i]
        upperLimit = (reg -> frequencies[0][i + 1] + reg -> frequencies[0][i]) / 2.0;
        reg -> frequencies[1][i] = upperLimit;
        reg -> frequencies[0][i + 1] = upperLimit;
    }
}

// esta funcion convierte frecuencias a notas musicales
Note parseFrequency(double frequency, Register reg){
    // se itera sobre el numero de notas en el registro
    for(int i = 0; i < reg.length; i++){
        // si la frecuencias esta en el rando especificado, entonces se retorna la nota
        if((reg.frequencies[0][i] <= frequency) && (frequency < reg.frequencies[1][i])){
            return reg.notes[i];
        }
    }

    // si no se retorno nada, es porque la frecuencia no esta en los limites establecidos
    // esto se intepreta como, no hay nota
    return reg.notNote;
}

// esta funcion convierte numero de semifusas a MusicalTime
MusicalTime semiFusaToMusicalTime(int semifusaNumber, int tempo){
    // equivalente de una semifusa en segundos
    double semifusaSeconds = (0.0625 * 60) / tempo;

    MusicalTime t;

    t.tempo = tempo;
    t.seconds = semifusaSeconds * semifusaNumber;
    // no sobra nada, pues se parte de tiempo musical en semifusas
    t.remainder = 0;

    t.semifusaNumber = semifusaNumber;

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

// esta funcion convierte tiempo en segundos a MusicalTime
MusicalTime seconds2MusicalTime(double seconds, int tempo, double roundRatio){
    // equivalente de semifusa en segundos
    double semifusaSeconds = (0.0625 * 60) / tempo;

    // numero de semifusas
    int semifusaNumber = intQuotient(seconds, semifusaSeconds);
    // sobrante
    double r = remainder(seconds, semifusaSeconds);

    // si se debe redondear el tiempo sobrante
    if(r >= (semifusaSeconds / roundRatio)){
        // el tiempo sobrante siempre sera r < semifusaSeconds, por tanto
        // solo se debe sumar 1 a semifusaNumber y r pasa a ser cero
        semifusaNumber++;
        r = 0;
    }

    // se crea tiempo musical
    MusicalTime t = semiFusaToMusicalTime(semifusaNumber, tempo);

    t.seconds = seconds;
    t.tempo = tempo;
    t.remainder = r;

    return t;
}

// esta funcion retorna el numero de notas que se necesitan para representar el
// tiempo musical pasado como argumento, ademas de devolver por referencia un
// array en el que especifica cuantas figuras de cada tiempo(redondas,
// blancas,..., semifusas) hay en el tiempo musical
int getNumberOfNotes(MusicalTime t, int *times){
    // inicialmente el numero de notas es cero
    int number = 0;

    // si hay una redonda en el tiempo musical
    if (t.redonda){
        // se agrega el numero de redondas del tiempo musical
        // en el numero de notas
        number += t.redonda;
        // se agrega el numero de redondas al numero de redondas en times
        times[REDONDA] = t.redonda;
        // si hay al menos un blanca se puede hacer una nota con puntillo
        if(t.blanca){
            // se aumenta el numero de notas en el numero de blancas menos 1(por
            // que se quita la blanca que se usara para formar la nota con puntillo) 
            number += t.blanca - 1;
            // se inicializa el numero de blancas en times
            times[BLANCA] = t.blanca - 1;
            // se pone como 1 el nummero de redondas con puntillo
            times[REDONDA_] = 1;
            // se debe disminuir un redonda, pues una ha sido usada para la nota
            // con puntillo y no debe ser contada 2 veces
            times[REDONDA]--;
            // las blancas se establecen a cero, porque ya se ha contado todas las blancas
            t.blanca = 0;
        }
    }

    // comentarios anteriores se extrapolan para las siguientes lineas
    if (t.blanca){
        number += t.blanca;
        times[BLANCA] = t.blanca;
        if(t.negra){
            number += t.negra - 1;
            times[NEGRA] = t.negra - 1;
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
            times[CORCHEA] = t.corchea - 1;
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
            times[SEMICORCHEA] = t.semicorchea - 1;
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
            times[FUSA] = t.fusa - 1;
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
            times[SEMIFUSA] = t.semifusa - 1;
            times[FUSA_] = 1;
            times[FUSA]--;
            t.semifusa = 0;
        }
    }

    // este programa considera la semifusa como la nota de más pequeña duracion
    // por tanto no pueden haber semifusas con puntillo
    number += t.semifusa;
    times[SEMIFUSA] = t.semifusa;

    return number;
}

// esta funcion retorna True si despues de una posicion dada en el array times,
// existe algun elemento no nulo, retorna False en caso contrario 
Bool thereAreNotes(int *times, int time, Note note){
    // si la nota es referencia a no nota entonces no hay nota
    if (note.note == 0 && note.eighth == 0)
        return False;

    // se itera sobre el numero de elementos en times
    for(int i = (time + 1); i < 13; i++){
        // si hay uno diferente de 1, se retorna True
        if(times[i])
            return True;
    }
    
    // se retorna False si no se encuentra nada
    return False;
}

// esta funcion recibe una nota y un tiempo musical y retorna por referencia
// la representacion en sintasix de lilypond
void parseNote(Note note, MusicalTime t, char **string){
    int times[13] = {0};  // array que guardara el numero de notas de cada tipo
    // se obtiene el numero de notas necesarias para representar el tiempo musical
    int noteNumbers = getNumberOfNotes(t, times);

    // en la longitud del string se considera el nombre de la nota
    // si es sostenida o bemol, el tiempo y la octava en la que esta
    int stringLength = (1 + 2 + 3 + abs(note.eighth - 4)) * noteNumbers + 1;

    // se consigue espacio para el string de salida
    *string = (char *) malloc(sizeof(char) * stringLength);
    // string que guardara el nombre de la nota
    char *noteName;
    // se consigue el nombre de la nota
    note2string(note, &noteName);

    // variable que especifica cuando poner o no ligaduras
    Bool withLigature;

    // se inicializa el string
    strcpy(*string, "");

    // si hay una redonda con puntillo
    if (times[REDONDA_]){
        // en principio no debe ponerse ligadura
        withLigature = False;
        // si hay mas notas ademas de la redonda con
        // puntillo debe ponerse ligadura
        if(thereAreNotes(times, REDONDA_, note))
            withLigature = True;

        // se escribe el nombre de la nota
        strcat(*string, noteName);
        // se escribe el tiempo de la nota, con o sin ligadura
        // segun corresponda
        if(withLigature && note.note)
            strcat(*string, "1.~ ");
        else
            strcat(*string, "1. ");
    }

    // si hay una redonda
    if (times[REDONDA]){
        // en principio no debe ponerse ligadura
        withLigature = False;
        // se itera sobre el numero de redondas
        for(int i = 0; i < times[REDONDA]; i++){
            // si es la ultima redonda, no se debe poner ligadura
            if((i + 2) > times[REDONDA])
                withLigature = False;

            // si hay mas notas ademas de la redonda, se debe poner ligadura
            if(thereAreNotes(times, REDONDA, note))
                withLigature = True;

            // se escribe el nombre de la nota
            strcat(*string, noteName);
            
            // se escribe el tiempo de la nota, con o sin ligadura
            // segun corresponda
            if(withLigature && note.note)
                strcat(*string, "1~ ");
            else
                strcat(*string, "1 ");
        }
    }

    // los comentarios de las lineas anteriores se extrapolan
    // para las lineas siguientes
    if (times[BLANCA_]){
        withLigature = False;
        if(thereAreNotes(times, BLANCA_, note))
            withLigature = True;

        // se escribe el nombre de la nota
        strcat(*string, noteName);
        
        if(withLigature && note.note)
            strcat(*string, "2.~ ");
        else
            strcat(*string, "2. ");
    }

    if (times[BLANCA]){
        withLigature = False;
        for(int i = 0; i < times[BLANCA]; i++){
            if((i + 2) > times[BLANCA])
                withLigature = False;

            if(thereAreNotes(times, BLANCA, note))
                withLigature = True;

            // se escribe el nombre de la nota
            strcat(*string, noteName);
            
            if(withLigature && note.note)
                strcat(*string, "2~ ");
            else
                strcat(*string, "2 ");
        }
    }

    if (times[NEGRA_]){
        withLigature = False;
        if(thereAreNotes(times, NEGRA_, note))
            withLigature = True;

        // se escribe el nombre de la nota
        strcat(*string, noteName);
        
        if(withLigature && note.note)
            strcat(*string, "4.~ ");
        else
            strcat(*string, "4. ");
    }

    if (times[NEGRA]){
        withLigature = False;
        for(int i = 0; i < times[NEGRA]; i++){
            if((i + 2) > times[NEGRA])
                withLigature = False;

            if(thereAreNotes(times, NEGRA, note))
                withLigature = True;

            // se escribe el nombre de la nota
            strcat(*string, noteName);
            
            if(withLigature && note.note)
                strcat(*string, "4~ ");
            else
                strcat(*string, "4 ");
        }
    }

    if (times[CORCHEA_]){
        withLigature = False;
        if(thereAreNotes(times, CORCHEA_, note))
            withLigature = True;

        // se escribe el nombre de la nota
        strcat(*string, noteName);
        
        if(withLigature && note.note)
            strcat(*string, "8.~ ");
        else
            strcat(*string, "8. ");
    }

    if (times[CORCHEA]){
        withLigature = False;
        for(int i = 0; i < times[CORCHEA]; i++){
            if((i + 2) > times[CORCHEA])
                withLigature = False;

            if(thereAreNotes(times, CORCHEA, note))
                withLigature = True;

            // se escribe el nombre de la nota
            strcat(*string, noteName);
            
            if(withLigature && note.note)
                strcat(*string, "8~ ");
            else
                strcat(*string, "8 ");
        }
    }

    if (times[SEMICORCHEA_]){
        withLigature = False;
        if(thereAreNotes(times, SEMICORCHEA_, note))
            withLigature = True;

        // se escribe el nombre de la nota
        strcat(*string, noteName);
        
        if(withLigature && note.note)
            strcat(*string, "16.~ ");
        else
            strcat(*string, "16. ");
    }

    if (times[SEMICORCHEA]){
        withLigature = False;
        for(int i = 0; i < times[SEMICORCHEA]; i++){
            if((i + 2) > times[SEMICORCHEA])
                withLigature = False;

            if(thereAreNotes(times, SEMICORCHEA, note))
                withLigature = True;

            // se escribe el nombre de la nota
            strcat(*string, noteName);
            
            if(withLigature && note.note)
                strcat(*string, "16~ ");
            else
                strcat(*string, "16 ");
        }
    }

    if (times[FUSA_]){
        withLigature = False;
        if(thereAreNotes(times, FUSA_, note))
            withLigature = True;

        // se escribe el nombre de la nota
        strcat(*string, noteName);
        
        if(withLigature && note.note)
            strcat(*string, "32.~ ");
        else
            strcat(*string, "32. ");
    }

    if (times[FUSA]){
        withLigature = False;
        for(int i = 0; i < times[FUSA]; i++){
            if((i + 2) > times[FUSA])
                withLigature = False;

            if(thereAreNotes(times, FUSA, note))
                withLigature = True;

            // se escribe el nombre de la nota
            strcat(*string, noteName);
            
            if(withLigature && note.note)
                strcat(*string, "32~ ");
            else
                strcat(*string, "32 ");
        }
    }

    if (times[SEMIFUSA]){
        withLigature = True;
        for(int i = 0; i < times[SEMIFUSA]; i++){
            // se escribe el nombre de la nota
            strcat(*string, noteName);

            if((i + 2) > times[SEMIFUSA])
                withLigature = False;
            
            if(withLigature && note.note)
                strcat(*string, "64~ ");
            else
                strcat(*string, "64 ");
        }
    }

    // se libera memoria
    free(noteName);
}

// esta funcion retorna True si 2 frecuencias no representan la misma nota
// se retorna False en caso contrario
Bool areDifferent(double freq1, double freq2, Register reg){
    Note note1 = parseFrequency(freq1, reg);
    Note note2 = parseFrequency(freq2, reg);

    return !(note1.note == note2.note && note1.eighth == note2.eighth);
}

// esta funcion recibe un entero representado la longitud de un array de frecuencias y
// de enteros que recibe como parametros, y retorna por referencia en el array account
// para cada frecuencia en la posicion i cuantas frecuencias iguales(que representen la
// misma nota) hay despues de ella, se pone cero si la frecuencia en la posicion i es
// igual a la frecuencia en la posicion i - 1
void joinFrequencies(int length, double *frequencies, int *account, Register reg){
    int index = 0;

    // se inicializa a cero el array de conteo
    for(int i = 0; i < length; i++){
        account[i] = 0;
    }
    
    // se itera el array de frecuencias
    while(index < length){
        // se itera sobre el array de frecuencias desde el indice en el que se va
        for(int i = index; i < length; i++){
            // si son diferentes se debe romper el ciclo y continuar
            if (areDifferent(frequencies[i], frequencies[i + 1], reg)){
                account[index]++;

                index = i;
                break;
            }
            account[index]++;
        }
        index++;
    }
}

// esta funcion recibe la duraccion de una nota y retorna cuantas semifusas
// cubren la nota completa, notas vienen determinadas por 1 = redonda, 2 = blanca,
// 4 = negra, 8 = corchea, 16 = semicorchea, 32 = fusa y 64 = semifusa
int intToSemiFusa(int note){
    int power = 1;
    while(note * power < 64)
        power *= 2;

    return power;
}

// esta funcion retorna el numero de semifusas que cubren al tiempo musical
// t pasado como argumento
int MusicalTimeToSemiFusa(MusicalTime t){
    return t.semifusaNumber;
}

// esta funcion se encarga de hacer la comprobacion de compas, es decir, verificar que
// cada tiempo que se vaya a agregar, cumpla con que el compas tenga la duracion establecida
// en caso de que no se cumpla, se particiona el tiempo
int checkMeasureTime(int totalTime, int measure_time, MusicalTime t, MusicalTime **ts){
    // equivalente en semifusas del tiempo musical
    int time = MusicalTimeToSemiFusa(t);

    // se inicializa el array de tiempos musicales de salida
    (*ts) = (MusicalTime *) malloc(sizeof(MusicalTime));

    // si el numero de semifusas del tiempo es menor que el tiempo de semifusas que le
    // caben al compas, al nota se puede agregar sin problemas
    if(time <= (measure_time - totalTime)){
        (*ts)[0] = t;
        return 1;
    }
    
    // si el tiempo es mayor, se debe particionar el tiempo musical para cumplir las condiciones
    time -= measure_time - totalTime;
    // se llena el compas
    (*ts)[0] = semiFusaToMusicalTime(measure_time - totalTime, t.tempo);

    // se halla la longitud del array de tiempos musicales de salida
    int length = (time % measure_time == 0) ?
                        (time / measure_time) :
                        (time / measure_time + 1);

    // se reubica el array con el nuevo espacio
    (*ts) = (MusicalTime *) realloc(*ts, sizeof(MusicalTime) * (length + 1));
    int i = 1;  // contador, indice del array de salida
    // mientras hayan semifusas se debe seguir particionando el tiempo musical
    while(time > 0){
        // si el tiempo es mayor que el tiempo del compas, se puede crear un tiempo
        // del tamaño del compas
        if (time > measure_time){
            time -= measure_time;
            (*ts)[i++] = semiFusaToMusicalTime(measure_time, t.tempo);
        }else{
            (*ts)[i++] = semiFusaToMusicalTime(time, t.tempo);
            time = 0;
        }
    }

    // se retorn el numero de tiempos musicales creados
    return length + 1;
}

void parseFrequencies(MusicSheetInfo info, int length, double seconds, double *frequencies, double roundRatio, Register reg){
    // se abre el archivo de salid
    FILE *file = fopen(info.filename, "w");

    // se escribe la version
    fprintf(file, "\\version \"2.18.2\"\n");

    // se escribe el header
    fprintf(file, "\\header{\n title = %s\n}\n", info.title);

    // se escribe el body
    fprintf(file, "\\score{\n\n\\new Staff {\n");
    fprintf(file, "\\tempo 4 = %d\n", info.tempo);
    fprintf(file, "\\time %d/%d\n", info.notes, info.measure);

    // array que guarda conteo de las frecuencias consecutivas repetidas
    int account[length];
    // se hace conteo de frecuencias
    joinFrequencies(length, frequencies, account, reg);

    // se consigue tiempo de medida de compas en semifusas
    int measure_time = intToSemiFusa(info.measure) * info.notes;
    // no se han sumado notas, entonces el tiempo debe ser cero
    int totalTime = 0;

    // se imprime tiempo que se pudo parsear
    printf("total time: %lf\n", length * seconds);
    printf("====================================================\n");

    // se itera sobre numero de frecuencias
    for(int i = 0; i < length; i++){
        // si hay una frecuencia
        if (account[i]){
            // string que guardara la representacion de la nota
            char *string;

            // se consigue la nota
            Note note = parseFrequency(frequencies[i], reg);
            // se consigue el tiempo musical
            MusicalTime t = seconds2MusicalTime(account[i] * seconds, info.tempo, roundRatio);
            MusicalTime *ts;
            // se hace checkeo de compas
            int length = checkMeasureTime(totalTime, measure_time, t, &ts);

            // se itera sobre el numero de tiempos musicales
            for(int j = 0; j < length; j++){
                // se consigue representacion en sintasix lilypond de la nota
                parseNote(note, ts[j], &string);

                // se aumenta el tiempo, se estan poniendo notas
                totalTime += MusicalTimeToSemiFusa(ts[j]);                

                // si el totalTime es mayor, se debe poner barra de compas y
                // establecer la cuenta a cero
                if(totalTime >= measure_time){
                    // si hay mas notas del mismo tipo, estan deben esta ligadas
                    if(j + 2 <= length && note.note){
                        fprintf(file, "%s~ | ", string);
                        }
                    else{
                        fprintf(file, "%s| ", string);
                        }

                    totalTime = 0;
                }else{
                    fprintf(file, "%s", string);
                }
            }

            // se libera memoria
            free(string);
            free(ts);
        }
    }

    // se cierran llaves
    fprintf(file, "\n}\n");

    fprintf(file, "\\layout { }\n");
    fprintf(file, "\\midi { }\n");
    fprintf(file, "\n}\n");

    // se cierra el archivo
    fclose(file);
}

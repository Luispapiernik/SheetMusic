#ifndef MISCELLANEOUS_H
#define MISCELLANEOUS_H

// Para valores booleanos, esto es mas
// legible que pasar 1 o 0 a una funcion
typedef enum{
    False,
    True
}Bool;


// esta funcion retorna la frecuencia de una nota en una octava dada
// teniendo en cuenta que C=1, C#=2, D=3, D#=4,...
double frequency(int note, int eighth);

// esta funcion retorna la magnitud de un numero complejo con
// parte real a y parte imaginaria b
double amplitude(double a, double b);

// esta  funcion convierte de frames a segundos
double frames2seconds(int frames, int framerate);

// esta funcion retorna el numero de veces que b esta en a
int intQuotient(double a, double b);

// esta funcion retorna el sobrante de meter b en a las veces
// retornadas por quotient
double remainder(double a, double b);

#endif

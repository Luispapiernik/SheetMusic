#include <math.h>


// esta funcion retorna la magnitud de un numero complejo con
// parte real a y parte imaginaria b
double amplitude(double a, double b){
    return sqrt(a * a + b * b);
}

// esta funcion retorna la frecuencia de una nota en una octava dada
// teniendo en cuenta que C=1, C#=2, D=3, D#=4,... y las octavas van
// desde 1
double frequency(int note, int eighth){
    return 440 * exp((eighth - 4 + (note - 10) / 12.0) * log(2));
}

// esta  funcion convierte de frames a segundos
double frames2seconds(int frames, int framerate){
    return (double) frames / framerate;
}

// esta funcion convierte segundos a frames
int seconds2frames(double seconds, int framerate){
    return (int) (seconds * framerate);
}


// esta funcion realiza una division entre numeros reales, esta funcion
// retorna por referencia cuantas veces cabe b en a y lo que sobra de
// realizar esta operacion. Ej: a = 9.4, b = 3.1 -> quotient = 3,
// remainder = 0.1
void division(double a, double b, int *quotient, double *remainder){
    // se inicializa el cociente y lo que sobra en cero
    *quotient = 0;
    *remainder = 0;

    // si b es mayor que a, entonces b esta 0 veces en a
    if (b > a)
        return;

    // mientras a sea mayor que b se va aumentando el cociente entre a y b
    // en 1 y se disminuye "a" una cantidad "b"
    while(a >= b){
        (*quotient)++;
        a -= b;
    }

    // en a esta lo que sobra de el resultado de meter b en a
    *remainder = a;
}

// esta funcion retorna el numero de veces que b esta en a
int intQuotient(double a, double b){
    int q;
    double r;

    division(a, b, &q, &r);

    return q;
}

// esta funcion retorna el sobrante de meter b en a las veces
// retornadas por intQuotient
double remainder(double a, double b){
    int q;
    double r;

    division(a, b, &q, &r);

    return r;
}

#include <math.h>


double amplitude(double a, double b){
    return sqrt(a * a + b * b);
}

double frequency(int note, int eighth){
    return 440 * exp((eighth - 4 + (note - 10) / 12.0) * log(2));
}

double frames2seconds(int frames, int framerate){
    return (double) frames / framerate;
}

int seconds2frames(double seconds, int framerate){
    return (int) (seconds * framerate);
}


void division(double a, double b, int *quotient, double *remainder){
    *quotient = 0;
    *remainder = 0;

    if (b > a)
        return;

    while(a >= b){
        (*quotient)++;
        a -= b;
    }

    *remainder = a;
}

int intQuotient(double a, double b){
    int q;
    double r;

    division(a, b, &q, &r);

    return q;
}

double remainder(double a, double b){
    int q;
    double r;

    division(a, b, &q, &r);

    return r;
}
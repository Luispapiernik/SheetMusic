#include <math.h>


double frequency(int note, int eighth){
    return 440 * exp((eighth - 4 + (note - 10) / 12.0) * log(2));
}
#include <stdlib.h>

#include <math.h>
#include <fftw3.h>

#include "audio.h"
#include "frequency.h"


double amplitude(double a, double b){
    return sqrt(a * a + b * b);
}

void getFrequency(int channel, Audio *audio){
    fftw_complex *in, *out;
    fftw_plan p;

    int frames = audio -> frames;

    in = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * frames);
    out = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * frames);
    p = fftw_plan_dft_1d(frames, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

    int *data;
    getChannel(0, audio, &data);
    for (int i = 0; i < frames; i++){
        in[i][0] = data[i];
        in[i][1] = 0;
    }

    fftw_execute(p);

    int indexMax = 0;
    for (int i = 1; i < frames; ++i){
        // printf("(%lf, %lf)\n", out[i][0], out[i][1]);
        if(amplitude(out[indexMax][0], out[indexMax][1]) < amplitude(out[i][0], out[i][1])){
            indexMax = i;
        }
    }

    double dt = 1.0 / audio -> framerate;
    double *freq = (double *) malloc(sizeof(double) * frames);
    int index = 0;
    for (double i = -1.0 / (2 * dt) ; i < 1.0 / (2 * dt) ; i += 1.0 / (dt * audio -> frames)){
        freq[index] = i;
        index++;
    }

    printf("%lf\n", freq[indexMax]);

    FILE *fp = fopen("datos.txt", "w");
    for (int i = 0; i < frames; ++i){
        fprintf(fp, "%lf %lf\n", freq[i], amplitude(out[i][0], out[i][1]));
    }
    fclose(fp);



    fftw_destroy_plan(p);
    fftw_free(in);
    fftw_free(out);
}
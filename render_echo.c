#include <stdio.h>
#include "io.h"
#include "wave.h"
#include <stdlib.h>

void render_echo(int argc, char** argv) {

  int  delay = 0.0;
  float amplitude = 0.0;
  if(argc != 5) {
    printf("Enter appropriate command line arguments");
  }
  else {
    unsigned num_samples;
    
    FILE *in = fopen(argv[1],"r");
    delay = atoi(argv[3]);
    amplitude = atof(argv[4]);
    read_wave_header(in,&num_samples);
    int16_t *data=calloc(num_samples*2, sizeof(int16_t));
    read_s16_buf(in, data, num_samples);
    float a = (float)SAMPLES_PER_SECOND/delay;

    for (int i=a,j=0;i < (int)num_samples; i++,j++) {
      data[i] += (data[j]*amplitude);//check if formula is right
    }
    FILE *out=fopen(argv[2],"w");
    write_wave_header(out,num_samples);
    write_s16_buf(out, data, num_samples);

    fclose(in);
    fclose(out);
    free(data);
  }




}
      
    

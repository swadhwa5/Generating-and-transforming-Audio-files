#include<stdio.h>
#include<stdlib.h>
#include "io.h"
#include "wave.h"

int main(int argc, char **argv) {
  if(argc!=6){
    fprintf(stderr, "Incorrect argument, Please enter appropriate argument!\n");
    exit(1);//do we need exit?
  }
  //record wave values according to format
  unsigned voice=atoi(argv[1]);
  float freq=atof(argv[2]);
  float amp=atof(argv[3]);
  unsigned num_samples=atoi(argv[4]);
  FILE *out = fopen(argv[5], "wb");
  printf("Frequency: %f Number of Samples: %d\n", freq, num_samples);
  //handle file error
  if(out == NULL){
    fatal_error("Error: Couldn't open wavefileout"); 
  }
  
  int16_t *buf=calloc(num_samples*2u, sizeof(int16_t)); //allocate memory for buf with 2*length of samples because stereo
  render_voice_stereo(buf, num_samples, freq, amp, voice);
  write_wave_header(out, num_samples); //write header
  write_s16_buf(out, buf, num_samples*2); //write buf

  fclose(out);
  free(buf);
  return 0;
}

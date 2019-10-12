#include<stdio.h>
#include<stdlib.h>
#include "io.h"
#include "wave.h"

int main(int argc, char **argv) {
  if(argc!=6){
    fprintf(stderr, "incorrect argument\n");
    exit(1);
  }
  //record wave values according to format
  unsigned voice=atoi(argv[1]);
  float freq=atof(argv[2]);
  float amp=atof(argv[3]);
  unsigned numsamp=atoi(argv[4]);
  FILE *out = fopen(argv[5], "wb");
  printf("freq: %f nums: %d\n", freq, numsamp);
  //handle file error
  if(out==NULL){
    fatal_error("cannot open wavefileout"); 
  }
  
  int16_t *buf=calloc(numsamp*2, sizeof(int16_t)); //allocate memory for buf with 2*length of samples because stereo
  render_voice_stereo(buf, numsamp*2, freq, amp, voice);
  write_wave_header(out, numsamp); //write header
  write_s16_buf(out, buf, numsamp*2); //write buf

  fclose(out);
  free(buf);
  return 0;
}

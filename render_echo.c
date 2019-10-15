#include <stdio.h>
#include "io.h"
#include "wave.h"
#include <stdlib.h>

int main(int argc, char** argv) {
  if(argc != 5) {
    printf("Enter appropriate command line arguments\n");
    return 1;
  }
  else { 
    unsigned num_samples;
    int  delaysamp = 0;
    float amplitude = 0.0;
    FILE *in = fopen(argv[1],"rb");
    if(in==NULL){
      fatal_error("unable to open file\n"); //handle file open error
    }
    delaysamp = atoi(argv[3]);
    amplitude = atof(argv[4]);
    read_wave_header(in,&num_samples); //record num samples

    int final_length=num_samples*2+delaysamp*2; 
    int16_t *orig=calloc(final_length, sizeof(int16_t));
    read_s16_buf(in, orig, num_samples*2); //create array for original input
    int16_t *delay=calloc(final_length, sizeof(int16_t)); //creat array for just the delay part
    int16_t *final=calloc(final_length, sizeof(int16_t)); //creat array to add orig and delay together for the final file

  for(int d=delaysamp, i=0; i<final_length; i++){ //start to fill delay array at index d with the content from orig at index 0 
    if(d<final_length){
    delay[d]=amplitude*orig[i]; //fill in delay array
    d++;
    }
    final[i]=clampcheck(delay[i], orig[i]); //add delay track and orig track to form final track
  }

  FILE *out=fopen(argv[2],"wb"); //write wave file
    write_wave_header(out,num_samples+delaysamp);
    write_s16_buf(out, final, final_length);

    fclose(in);
    fclose(out);
    free(orig);
    free(delay);
    free(final);
    return 0;
  }
}
      
    

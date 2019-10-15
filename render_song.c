//default wave:sine wave
//default amplitude: 0.1
//N:Notes,C:chords, P:Pause

#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "wave.h"

int main(int argc, char** argv) {
  if(argc != 3) {
  fatal_error("Enter appropriate command line arguments");
  exit(1);
  }

  unsigned long int num_samples;
  unsigned long int num_samples_per_beat;
  FILE *in = fopen(argv[1],"r");
  FILE *out = fopen(argv[2],"wb");
  fscanf(in,"%lu%lu",&num_samples,&num_samples_per_beat);


  int voice = 0;
  float amplitude = 0.1;
  int16_t *data = (int16_t *)calloc(num_samples*2u,sizeof(int16_t));
  char type;
	 
  float num_beats;
  float note_number;
  float new_num_samples;
  float freq_hz;
  char *message;
  unsigned long int var = 0;
  
  
  
  while (!feof(in)) {
    fscanf(in," %c",&type);
    switch(type) {

    case 'N':
      
      fscanf(in,"%f",&num_beats);
      
      fscanf(in,"%f",&note_number);

      freq_hz = 440*pow(2,((note_number-69) / 12));
      new_num_samples = num_beats * num_samples_per_beat;
 
      render_voice_stereo(&data[var], new_num_samples, freq_hz, amplitude, voice);


      var += new_num_samples*2;
      
      break;

    case 'C':

      
      fscanf(in,"%f",&num_beats);
      
      fscanf(in,"%f",&note_number);
      while(note_number != 999) {
	

	freq_hz = 440*pow(2,((note_number-69) / 12));
	new_num_samples = num_beats * num_samples_per_beat;

	render_voice_stereo(&data[var], new_num_samples, freq_hz, amplitude, voice);


	fscanf(in,"%f",&note_number);
      }
      var +=new_num_samples*2;
	break;

    case 'P':
      fscanf(in,"%f",&num_beats);
      new_num_samples = num_beats * num_samples_per_beat;
      var +=new_num_samples*2;
      break;
      
    case 'V':fscanf(in,"%d",&voice);
      break;

    case 'A':fscanf(in,"%f",&amplitude);
      break;

    default:message = "Wrong Directive";//check for end of file
      fatal_error(message);
      
      break;
    }
   
    
  }

  write_wave_header(out, num_samples);
  write_s16_buf(out, data, num_samples*2u);
    
  fclose(in);
  fclose(out);
  
}
  
	 

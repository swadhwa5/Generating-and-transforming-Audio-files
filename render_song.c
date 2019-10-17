//default wave:sine wave
//default amplitude: 0.1
//N:Notes,C:chords, P:Pause

#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "wave.h"

int main(int argc, char** argv) {
  if(argc != 3) {
  fatal_error("Enter appropriate command line arguments\n");
  
  }

  unsigned num_samples;
  unsigned num_samples_per_beat;
  FILE *in = fopen(argv[1],"r");
  if(in == NULL) {
    fatal_error("Unable to open input file\n");
  }

  FILE *out = fopen(argv[2],"wb");
  if(out == NULL) {
    fatal_error("Unable to open output file\n");
  }
  
  if(fscanf(in,"%u%u",&num_samples,&num_samples_per_beat) != 2){
    fatal_error("Malformed input\n");
  }


  int voice = 0;//default voice
  float amplitude = 0.1;//default amplitude
  int16_t *data = (int16_t *)calloc(num_samples*2u,sizeof(int16_t));//buffer to be filled
  char type;//N or C or P or V or A
	 
  float num_beats;//no of beats for which directive has to be implemented
  float note_number;//MIDI note number
  float new_num_samples;// number of samples for which directive has to be implemented
  float freq_hz;
  
  unsigned var = 0;//counter to keep track of current position
  
  
  
  while (!feof(in)) {//checking for end of file
    fscanf(in," %c",&type);
    switch(type) {

    case 'N':
      
      fscanf(in,"%f",&num_beats);
      
      fscanf(in,"%f",&note_number);

      freq_hz = 440*pow(2,((note_number-69) / 12));
      new_num_samples = num_beats * num_samples_per_beat;
 
      render_voice_stereo(&data[var], new_num_samples, freq_hz, amplitude, voice);


      var += new_num_samples*2;//updating current position in buffer
      
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
      var +=new_num_samples*2;//updating current position in buffer
	break;

    case 'P':
      fscanf(in,"%f",&num_beats);
      new_num_samples = num_beats * num_samples_per_beat;
      var +=new_num_samples*2;//updating current position in buffer
      break;
      
    case 'V':fscanf(in,"%d",&voice);
      break;

    case 'A':fscanf(in,"%f",&amplitude);
      break;

    default: //check for Malformed input
      fatal_error("Wrong Directive\n");
      
      break;
    }
   
    
  }

  write_wave_header(out, num_samples);//writing header to output file
  write_s16_buf(out, data, num_samples*2u);//writing buffer to output file
    
  fclose(in);
  fclose(out);
  free(data);
  return 0;
}
  
	 

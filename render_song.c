//default wave:sine wave
//default amplitude: 0.1
//N:Notes,C:chords, P:Pause

#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "wave.h"

int main(int argc, char** argv) {
  if(argc != 3) {
  fatal_error("Malformed Input");
  exit(1);
  }

  unsigned long int num_samples;
  unsigned long int num_samples_per_beat;
  FILE *in = fopen(argv[1],"r");
  scanf("%lu%lu",&num_samples,&num_samples_per_beat);


  int voice = 0;
  float amplitude = 0.1;
  int16_t *data = (int16_t *)calloc(num_samples*2u,sizeof(int16_t));
  char type;
	 
  float num_beats;
  float note_number;
  float new_num_samples;
  float freq_hz;
  int16_t address;
  float currvalue;
  
  
  while (!feof(in)) {
    scanf(" %c",&type);
    switch(type) {

    case 'N':
      
      scanf("%f",&num_beats);
      
      scanf("%f",&note_number);

      freq_hz = 440*pow(2,((note_number-69) / 12));
      new_num_samples = num_beats * num_samples_per_beat;
      render_voice_stereo(data, new_num_samples, freq_hz, amplitude, voice);
      break;

    case 'C':

      address = &data;
      scanf("%f",&num_beats);
      
      scanf("%f",&note_number);
      while(note_number != 999) {
	&data = address;

	freq_hz = 440*pow(2,((note_number-69) / 12));
	new_num_samples = num_beats * num_samples_per_beat;
	render_voice_stereo(data, new_num_samples, freq_hz, amplitude, voice);
	scanf("%f",&note_number);
      }
	break;

    case 'P':
      scanf("%f",&num_beats);
      currvalue = *data;
      for (int i = 0; i< num_beats;i++) {
	
	*data = currvalue;
	data++;
      }
      break;
      
    case 'V':scanf("%d",&voice);
      break;

    case 'A':scanf("%f",&amplitude);
      break;

    default:printf("Wrong Directive");
      break;
    }
   
    
  }
}
  
	 

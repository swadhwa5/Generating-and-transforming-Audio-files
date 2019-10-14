//default wave:sine wave
//default amplitude: 0.1
//N:Notes,C:chords, P:Pause


render_song(int argc, char** argv) {
  if(argc != 3) {
  fatal_error("Malformed Input");
  exit(1);
  }

  unsigned num_samples;
  unsigned  num_samples_per_beat;
  FILE *in = fopen(argv[1],"r");
  fscanf(stdin,"%u","%u",num_samples,num_samples_per_beat);//do we have to use read_wave_header
  float song_dur = num_samples/SAMPLES_PER_SECOND;
  float beat_dur = num_samples_per_beat/SAMPLES_PER_SECOND;
  int voice = 0;
  float amplitude = 0.1;
  int16_t *data = (int16_t *)calloc(sizeof(int16_t)*num_samples*2);
  char type;
  int16_t *data = calloc(num_samples*2u, sizeof(int16_t));


  while (!feof(in)) {
    fscanf(" %c",type);
    switch(type) {

    case 'N':float num_beats;
      fcanf(stdin, "%f",num_beats);
      float note_number;
      fcanf(stdin, "%f",note_number);
      float freq_hz;
      freq_hz = 440*pow(2,((n-69) / 12));
      render_voice_stereo(data, num_samples, freq_hz, amplitude, voice);
      break;

    case 'C':
      break;

    case 'P':
      break;
      
    case 'V':fcanf(stdin,"%d",voice);
      break;

    case 'A':fcanf(stdin,"%d",amplitude);
      break;

    default:printf("Wrong Directive");
      break;
    }
   
    
  }
}
  
	 

//default wave:sine wave
//default amplitude: 0.1
//N:Notes,C:chords, P:Pause


render_song(int argc, char** argv) {
  unsigned num_samples;
  unsigned  num_samples_per_beat;
  FILE *in = fopen(argv[1],"r");
  fscanf("%f","%f",num_samples,num_samples_per_beat);//do we have to use read_wave_header
  int16_t *data = (int16_t *)calloc(sizeof(int16_t)*num_samples*2);
  char type = " ";
  float beats = 0.0;
  int note_number = 0;
  while (!feof(in)) {
    fscanf(" %c","%f","%d",type,beats, note_number);
    //fatal_error
    //render_voide_stereo
  }
}
  
	 

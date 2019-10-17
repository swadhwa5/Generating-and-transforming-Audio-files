#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "io.h"
#include "wave.h"

/*
 * Write a WAVE file header to given output stream.
 * Format is hard-coded as 44.1 KHz sample rate, 16 bit
 * signed samples, two channels.
 *
 * Parameters:
 *   out - the output stream
 *   num_samples - the number of (stereo) samples that will follow
 */
void write_wave_header(FILE *out, unsigned num_samples) {
  /*
   * See: http://soundfile.sapp.org/doc/WaveFormat/
   */

  uint32_t ChunkSize, Subchunk1Size, Subchunk2Size;
  uint16_t NumChannels = NUM_CHANNELS;
  uint32_t ByteRate = SAMPLES_PER_SECOND * NumChannels * (BITS_PER_SAMPLE/8u);
  uint16_t BlockAlign = NumChannels * (BITS_PER_SAMPLE/8u);

  /* Subchunk2Size is the total amount of sample data */
  Subchunk2Size = num_samples * NumChannels * (BITS_PER_SAMPLE/8u);
  Subchunk1Size = 16u;
  ChunkSize = 4u + (8u + Subchunk1Size) + (8u + Subchunk2Size);

  /* Write the RIFF chunk descriptor */
  write_bytes(out, "RIFF", 4u);
  write_u32(out, ChunkSize);
  write_bytes(out, "WAVE", 4u);

  /* Write the "fmt " sub-chunk */
  write_bytes(out, "fmt ", 4u);       /* Subchunk1ID */
  write_u32(out, Subchunk1Size);
  write_u16(out, 1u);                 /* PCM format */
  write_u16(out, NumChannels);
  write_u32(out, SAMPLES_PER_SECOND); /* SampleRate */
  write_u32(out, ByteRate);
  write_u16(out, BlockAlign);
  write_u16(out, BITS_PER_SAMPLE);

  /* Write the beginning of the "data" sub-chunk, but not the actual data */
  write_bytes(out, "data", 4);        /* Subchunk2ID */
  write_u32(out, Subchunk2Size);
}

/*
 * Read a WAVE header from given input stream.
 * Calls fatal_error if data can't be read, if the data
 * doesn't follow the WAVE format, or if the audio
 * parameters of the input WAVE aren't 44.1 KHz, 16 bit
 * signed samples, and two channels.
 *
 * Parameters:
 *   in - the input stream
 *   num_samples - pointer to an unsigned variable where the
 *      number of (stereo) samples following the header
 *      should be stored
 */
void read_wave_header(FILE *in, unsigned *num_samples) {
  char label_buf[4];
  uint32_t ChunkSize, Subchunk1Size, SampleRate, ByteRate, Subchunk2Size;
  uint16_t AudioFormat, NumChannels, BlockAlign, BitsPerSample;

  read_bytes(in, label_buf, 4u);
  if (memcmp(label_buf, "RIFF", 4u) != 0) {
    fatal_error("Bad wave header (no RIFF label)");
  }

  read_u32(in, &ChunkSize); /* ignore */

  read_bytes(in, label_buf, 4u);
  if (memcmp(label_buf, "WAVE", 4u) != 0) {
    fatal_error("Bad wave header (no WAVE label)");
  }

  read_bytes(in, label_buf, 4u);
  if (memcmp(label_buf, "fmt ", 4u) != 0) {
    fatal_error("Bad wave header (no 'fmt ' subchunk ID)");
  }

  read_u32(in, &Subchunk1Size);
  if (Subchunk1Size != 16u) {
    fatal_error("Bad wave header (Subchunk1Size was not 16)");
  }

  read_u16(in, &AudioFormat);
  if (AudioFormat != 1u) {
    fatal_error("Bad wave header (AudioFormat is not PCM)");
  }

  read_u16(in, &NumChannels);
  if (NumChannels != NUM_CHANNELS) {
    fatal_error("Bad wave header (NumChannels is not 2)");
  }

  read_u32(in, &SampleRate);
  if (SampleRate != SAMPLES_PER_SECOND) {
    fatal_error("Bad wave header (Unexpected sample rate)");
  }

  read_u32(in, &ByteRate); /* ignore */

  read_u16(in, &BlockAlign); /* ignore */

  read_u16(in, &BitsPerSample);
  if (BitsPerSample != BITS_PER_SAMPLE) {
    fatal_error("Bad wave header (Unexpected bits per sample)");
  }

  read_bytes(in, label_buf, 4u);
  if (memcmp(label_buf, "data", 4u) != 0) {
    fatal_error("Bad wave header (no 'data' subchunk ID)");
  }

  /* finally we're at the Subchunk2Size field, from which we can
   * determine the number of samples */
  read_u32(in, &Subchunk2Size);
  *num_samples = Subchunk2Size / NUM_CHANNELS / (BITS_PER_SAMPLE/8u);
}


void render_square_wave(int16_t buf[], unsigned num_samples, unsigned channel,
                        float freq_hz, float amplitude) {//write square wave sample values into buf given the specified parameters of the characteristics

  if (channel == 0) {//left=0

    float t = 0.0;                                  
    for(unsigned int i = 0, j=0; i<(num_samples*2); j++,i+=2) {
      t = (float)j/SAMPLES_PER_SECOND;//using j to increment one sample at a time
    float  p = amplitude * (32767) * sin((t * freq_hz)* (2 * PI));                                                                                                       
      if (p >= 0) { //assign max amp if p is non negative
	int16_t curramp=amplitude*(32768);
        buf[i] = clampcheck(buf[i], curramp);//checking for clamping of values
      }
      else if (p < 0) { //assign min amp if p is negative
	int16_t curramp=amplitude*(-32767);
        buf[i] = clampcheck(buf[i], curramp);//checking for clamping of values
      }
  
    }
  }
  if (channel == 1) {//right=1

    float t=0.0;
    for(unsigned int i = 1,j=0; i<(num_samples*2);j++, i+=2) {
      t = (float)j/SAMPLES_PER_SECOND;//using j to increment one sample at a time
      float  p = amplitude * (32767) * sin((t * freq_hz)* (2 * PI));//check p and t and amplitude                                                                                                       
      if (p >= 0) { //if p has a non negative val, assign max amp
	int16_t curramp=amplitude*(32767);
        buf[i] = clampcheck(buf[i], curramp);//checking for clamping of values
      }
      else if (p < 0) { //if p has negative cal, assign min amp
	int16_t curramp=amplitude*(-32768);
        buf[i] = clampcheck(buf[i], curramp);//checking for clamping of values
      }
         
    }
  }
}

void render_square_wave_stereo(int16_t buf[], unsigned num_samples,
                               float freq_hz, float amplitude) {//render stereo square wave into buf for both channels
  render_square_wave(buf, num_samples, 0, freq_hz, amplitude);//for left channel
  render_square_wave(buf, num_samples, 1, freq_hz, amplitude);//for right channel
}

void render_sine_wave(int16_t buf[], unsigned num_samples, unsigned channel,
                        float freq_hz, float amplitude) {//write sine wave sample values into buf given the specified parameters of the characteristics

  if (channel == 0) {//left=0                                                                                                                                           
    float t=0.0;
    for(int i = 0, j=0;i<(int)(num_samples*2);j++,i+=2) {
      t = (float)j/SAMPLES_PER_SECOND;//using j to increment one sample at a time
      float p = amplitude * (32767) * sin((t * freq_hz)* (2 * PI));                                                                      
      buf[i] =clampcheck(buf[i], (int16_t)p);//check for additive rendering then assign val to buf
         
    }
  }
  if (channel == 1) {//right=1                                                                                                                                         
    float t=0.0;                                                           
    for(int i = 1,j=0;i<(int)(num_samples*2);j++, i+=2) {
      t = (float)j/SAMPLES_PER_SECOND;//using j to increment one sample at a time
      float p = amplitude * (32767) * sin((t * freq_hz)* (2 * PI));                                                                      
      buf[i] =clampcheck(buf[i],(int16_t)p);//check for additive rendering then assign val to buf
        
    }
  }
}

void render_sine_wave_stereo(int16_t buf[], unsigned num_samples,
			     float freq_hz, float amplitude) {//render stereo sine wave into buf for both channels
  render_sine_wave(buf, num_samples, 0, freq_hz, amplitude);//left channel
  render_sine_wave(buf, num_samples, 1, freq_hz, amplitude);//right channel
}

void render_saw_wave(int16_t buf[], unsigned num_samples, unsigned channel, float freq_hz, float amplitude){//write saw wave sample values into buf given the specified parameters of the characteristics
  double cycle_length_s=1.0/freq_hz; //length per cycle in seconds
  double shifted_max_amp=amplitude*32767*2; //this value is used to calculate for p before assign neg/pos
  if(channel==0){ //left
    float t=0.0;
    for(int i=0, j=0; i<(int)(num_samples*2);j++, i+=2){
      t = (float)j/SAMPLES_PER_SECOND;//using j to increment one sample at a time
      float position=t/cycle_length_s; //sample pos in one cycle
      int16_t curramp= (position - floor(position))*shifted_max_amp-32767;//use floor of position and subtract floor vaue of position from position
      buf[i]=clampcheck(buf[i], curramp); //add previous & curramp and assign to buf
     
    }
  }
  if(channel==1){ //left
    float t=0.0;
    for(int i=1,j=0; i<(int)(num_samples*2);j++, i+=2){
      t = (float)j/SAMPLES_PER_SECOND;//using j to increment one sample at a time
      float position=t/cycle_length_s; //sample pos in one cycle
      int16_t curramp= (position- floor(position))*shifted_max_amp-32767;//use floor of position and subtract floor vaue of position from position
      buf[i]=clampcheck(buf[i], curramp); //add previous & curramp and assign to buf
      
    }
  }
}

void render_saw_wave_stereo(int16_t buf[], unsigned num_samples, float freq_hz, float amplitude){//render stereo saw wave into buf for both channels
  render_saw_wave(buf, num_samples, 0, freq_hz, amplitude);//left
  render_saw_wave(buf, num_samples, 1, freq_hz, amplitude);//right 
}

int16_t clampcheck(int16_t orig, int16_t add){//add two sample values and clamp them to max or min if necessary, return their sum
  int32_t sum=(int32_t)orig+(int32_t)add;
  if(sum>32767){ //exceeds max amp
    sum=32767; //clamp to max amp
  }
  else if(sum<-32768){ //exceeds min amp
    sum=-32768; //clamp to min amp
  }
  return (int16_t)sum; //return sum as 16bit
}




void render_voice(int16_t buf[], unsigned num_samples, unsigned channel, float freq_hz, float amplitude, unsigned voice){
  //call render wave function according to voice, clampcheck is used in all wave functions
  if(voice==0){
    render_sine_wave(buf, num_samples, channel, freq_hz, amplitude);
  }
  else if(voice==1){
    render_square_wave(buf, num_samples, channel, freq_hz, amplitude);
  }
  else if(voice==2){
    render_saw_wave(buf, num_samples, channel, freq_hz, amplitude);
  }
}

void render_voice_stereo(int16_t buf[], unsigned num_samples, float freq_hz, float amplitude, unsigned voice){//render stereo wave into buf for both channels, wave type depend on voice
  render_voice(buf, num_samples, 0, freq_hz, amplitude, voice);//left
  render_voice(buf, num_samples, 1, freq_hz, amplitude, voice);//right
}

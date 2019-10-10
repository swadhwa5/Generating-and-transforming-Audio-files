#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "io.h"

/* TODO: define the functions declared in io.h... */
void fatal_error(const char *message){
  fprintf(stderr, "Error: %s", message); //print msg to stderr
  exit(1);
}

void write_byte(FILE *out, char val){
  char write=fputc(val, out);
  if(write==EOF) {
    const char *message="failed to write byte\n";
    fatal_error(message);
  }
}

void write_bytes(FILE *out, const char data[], unsigned n){
  for(int i=0; i<(int)n; i++){ //using for loop to write each char into output
    write_byte(out, data[i]);
  }
}

void write_u16(FILE *out, uint16_t value){
  char lo=value%256;
  char hi=(value/256)%256;
  write_byte(out, lo); //write the least significant byte
  write_byte(out, hi); //write the more significant byte
}

void write_u32(FILE *out, uint32_t value){
  char input[4]; //each element is one byte of value, in little endian format
  input[0] = value & 0xFF;
  input[1] = (value >> 8) & 0xFF;
  input[2] = (value >> 16) & 0xFF;
  input[3] = (value >> 24) & 0xFF;
  for(int i=0; i<4; i++){
    write_byte(out, input[i]); //write each byte to output
  }
}

void write_s16(FILE *out, int16_t value){ 
  char lo=value%256;
  char hi=(value/256)%256;
  write_byte(out, lo); //write the least significant byte
  write_byte(out, hi); //write the more significant byte
}

void write_s16_buf(FILE *out, const int16_t buf[], unsigned n){
  for(int i=0; i<(int)n; i++){ //using for loop to write individual int16_t to output
    write_s16(out, buf[i]);
  }
}

void read_byte(FILE *in, char *val){
  char read=fgetc(in);
  if(read==EOF){
    const char *message="failed to read byte\n";
    fatal_error(message);
  }
  else{
    *val=read; //assign val in file to to the val pointer points to 
  }
}

void read_bytes(FILE *in, char data[], unsigned n){ 
  for(int i=0; i<(int)n; i++){
    char *currChar=&(data[i]); //creat a currChar pointer that points to the an element of data array
    read_byte(in, currChar); //pass the char pointer to readbyte function
  }
}

void read_u16(FILE *in, uint16_t *val){
  char v1=fgetc(in); //least sig
  char v2=fgetc(in); //most sig
  if(v1==EOF||v2==EOF){
    char *message="failed to read byte\n";
    fatal_error(message);
  }
  else{
    *val = v1 | (v2 << 8); //shift v2 since its more sig then assign comb val to the address
  }
}

void read_u32(FILE *in, uint32_t *val){ 
  char data[4];
  read_bytes(in, data, 4); //read 4 bytes and store in data array in big endian
  uint32_t comb=data[0]+(data[1] * 256)+(data[2] * 65536)+(data[3] * 16777216); //combine all bytes in little endian format
  *val = comb; //assign reconstructed unit32 to the address of val
}

void read_s16(FILE *in, int16_t *val){
  char data[2];
  read_bytes(in, data, 2); //read 2 bytes and store in data array in big endian format
  *val=data[0] + (data[1]*256); //assign comb val to address
}

void read_s16_buf(FILE *in, int16_t buf[], unsigned n){
  for(int i=0; i<(int)n; i++){
    int16_t *curr=&(buf[i]); //create pointer points to an element of buf array
    read_s16(in, curr);
  }
}



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
  //printf("trying to write this value: %c\n", val);
  
  if(write == EOF) {
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
  unsigned char lo= value & 0xFF;
  unsigned char hi= (value >> 8) & 0xFF;
  write_byte(out, (char)lo); //write the least significant byte
  write_byte(out, (char)hi); //write the more significant byte
}

void write_u32(FILE *out, uint32_t value){
  unsigned char input[4]; //each element is one byte of value, in little endian format
  input[0] = value & 0xFF;
  input[1] = (value >> 8) & 0xFF;
  input[2] = (value >> 16) & 0xFF;
  input[3] = (value >> 24) & 0xFF;
  for(int i=0; i<4; i++){
    write_byte(out, (char)input[i]); //write each byte to output
  }
}

void write_s16(FILE *out, int16_t value){ 
  char lo = value & 0xFF;
  char hi = (value >> 8) & 0xFF;
  write_byte(out, lo); //write the least significant byte
  write_byte(out, hi); //write the more significant byte
}

void write_s16_buf(FILE *out, const int16_t buf[], unsigned n){
  for(unsigned int i=0; i<n; i++){ //using for loop to write individual int16_t to output
    printf("i: %d",i);
    //printf("buf value passed is: %d\n", buf[i]);
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
unsigned char data[2];  //check if it needs to be a poiner                                                                                                                
 read_bytes(in,(char*) data, 2); //read 2 bytes and store in data array in little endian format                                                                        
  *val=data[0] | (data[1] << 8); //assign reconstructed int16 to val   
}

void read_u32(FILE *in, uint32_t *val){ 
 unsigned char data[4];//check if it needs to be a pointer
 read_bytes(in, (char*) data, 4); //read 4 bytes and store in data array in big endian
 uint32_t comb=data[0] | (data[1]) << 8 | (data[2]) << 16 | (data[3]) << 24; //combine all bytes in little endian format
  *val = comb; //assign reconstructed unit32 to val
}

void read_s16(FILE *in, int16_t *val){
  char data[2];//check if it needs to be a poiner
  read_bytes(in, data, 2); //read 2 bytes and store in data array in little endian format
  *val=data[0] | (data[1] << 8); //assign reconstructed int16 to val
}

void read_s16_buf(FILE *in, int16_t buf[], unsigned n){
  for(int i=0; i<(int)n; i++){
    int16_t *curr=&(buf[i]); //create pointer points to an element of buf array
    read_s16(in, curr);
  }
}



#include <stdio.h>
 
 
 
 
 
int main() {
  int big = 0x876543FF;
  char buf[4];
 
  buf[0] = big & 0xFF;
  buf[1] = big >> 8 & 0xFF;
  buf[2] = big >> 16 & 0xFF;
  buf[3] = big >> 24 & 0xFF;
 
  int comb = buf[0] | buf[1] << 8 | buf[2] << 16 | buf[3] << 24;
 
  printf("%x\n", comb);
 
}

#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "wave.h"

int main(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <wave file>\n", argv[0]);
    exit(1);
  }

  FILE *in = fopen(argv[1], "rb");
  FILE *out = fopen(argv[2], "wb");
  if (in == NULL) {
    fatal_error("Couldn't open wave file");
  }

  unsigned num_samples;
  read_wave_header(in, &num_samples);
  printf("Wave file has %u stereo samples\n", num_samples);
  write_wave_header(out, num_samples);
  fclose(in);
  fclose(out);

  return 0;
}

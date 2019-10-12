CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra -g

render_tone: render_tone.o wave.o io.o
        $(CC) -o render_tone render_tone.o wave.o io.o
render_tone.o: render_tone.c wave.c wave.h io.c io.h
        $(CC) $(CFLAGS) -c render_tone.c wave.c io.c
wave.o: wave.c wave.h
        $(CC) $(CFLAGS) -c wave.c
io.o: io.c io.h
	$(CC) $(CFLAGS) -c io.c


render_song:


render_echo:



clean:
        rm -f *.o render_tone #how to add for render_song and render_echo


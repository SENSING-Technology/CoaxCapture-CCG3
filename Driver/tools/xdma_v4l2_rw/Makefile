CC ?= gcc

all: video_rw

video_rw: video_rw.o
	$(CC) -o $@ $<

%.o: %.c
	$(CC) -c -std=c99 -o $@ $< -D_FILE_OFFSET_BITS=64 -D_GNU_SOURCE -D_LARGE_FILE_SOURCE

clean:
	rm -rf video_rw *.o ../video_rw

install:
	cp video_rw  ../
CC ?= gcc

all: reg_rw

reg_rw: reg_rw.o
	$(CC) -o $@ $<

%.o: %.c
	$(CC) -c -std=c99 -o $@ $< -D_FILE_OFFSET_BITS=64 -D_GNU_SOURCE -D_LARGE_FILE_SOURCE

clean:
	rm -rf reg_rw *.o ../reg_rw

install:
	cp reg_rw  ../
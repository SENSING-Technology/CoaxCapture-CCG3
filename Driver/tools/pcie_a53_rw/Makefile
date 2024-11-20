CC ?= gcc

all: serdes_cfg 

serdes_cfg: main.o serdes_data.o element_process.o link_margin.o arm_pcie_protocol.o
	$(CC) -lrt -o $@ $^ -D_FILE_OFFSET_BITS=64 -D_GNU_SOURCE -D_LARGE_FILE_SOURCE	

%.o: %.c %.h
	$(CC) -c -std=c99 -o $@ $< -D_FILE_OFFSET_BITS=64 -D_GNU_SOURCE -D_LARGE_FILE_SOURCE

clean:
	rm -rf serdes_cfg *.o ../serdes_cfg

install:
	cp serdes_cfg ../

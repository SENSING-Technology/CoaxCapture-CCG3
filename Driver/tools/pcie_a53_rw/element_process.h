#ifndef ELEMENT_PROCESS_H
#define ELEMENT_PROCESS_H 

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <byteswap.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <termios.h>

#include <sys/types.h>
#include <sys/mman.h>
#include <sys/time.h>

typedef struct data_element{
	uint8_t dev;   //I2c Slave Addr
	uint16_t reg;  //Reg Offset
	uint16_t data;  //Reg Value
	uint8_t bits;  //Reg Offset Bit Width
	uint16_t delay;//Per Register Access Delay
} DATA_ELEMENT; 

typedef struct element_info{
	DATA_ELEMENT * pDATA;
	uint32_t ele_size;
} ELE_INFO;

void element_channel_process(void *BaseAddress, uint32_t channel, DATA_ELEMENT * elem, int elem_num);
void serdes_channel_init(void *BaseAddress, uint32_t channel, uint32_t index);

#endif 

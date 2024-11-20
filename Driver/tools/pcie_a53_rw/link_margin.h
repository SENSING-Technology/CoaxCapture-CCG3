#ifndef LINK_MARGIN_H
#define LINK_MARGIN_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <byteswap.h>
#include <string.h>
#include <errno.h>
//#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <termios.h>

#include <sys/types.h>
#include <sys/mman.h>

#define SE_REG01 	0x0001
#define SE_RLMS95 	0x1495
#define SE_RLMSA4 	0x14A4
#define SE_RLMS4 	0x1404
#define SE_RLMS3 	0x1403
#define SE_CNT0 	0x0022
#define SE_CTRL0 	0x0010
#define SE_CTRL3 	0x0013

#define DE_REG01 	0x0001
#define DE_RLMS95 	0x1495
#define DE_RLMSA4 	0x14A4
#define DE_RLMS4 	0x1404
#define DE_RLMS3 	0x1403
#define DE_CNT0 	0x0022
#define DE_CTRL0 	0x0010
#define DE_CTRL3 	0x0013

void link_margin_test(void *BaseAddress, uint32_t channel,uint32_t index);

#endif

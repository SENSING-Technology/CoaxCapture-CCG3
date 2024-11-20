#ifndef ARM_PCIE_PROTOCOL_H
#define ARM_PCIE_PROTOCOL_H

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

uint32_t send_cmd_to_a53_sync(const char send_cmd[]);
uint32_t get_cmd_result();
#endif

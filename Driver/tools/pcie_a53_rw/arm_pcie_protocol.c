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
#include "arm_pcie_protocol.h"
#define RESULT_BUF_LENGTH 4096

extern uint32_t xdma_reg_read(uint32_t addr);
extern uint32_t xdma_reg_write(uint32_t addr, uint32_t value);
char cmd_result[RESULT_BUF_LENGTH];
uint32_t get_cmd_result()
{
    uint32_t result;
    result = strtol(cmd_result, NULL,16);
    return result;
}

uint32_t send_cmd_to_a53_sync(const char send_cmd[])
{
    uint32_t cmd_length = strlen(send_cmd);
    uint32_t result_length = 0;
    uint32_t cursor = 0, buf_cursor = 0;
    uint32_t word;
    uint32_t loop_count = 0;
    uint32_t result = 0;

    // pc set flag register as 0x5a5aa5a5
    xdma_reg_write(0, 0x5a5aa5a5);

    // pc write cmd data.
    // start address at 0x8, 4 bytes once write.
    // write data length at 0x4 after cmd date write completely.
    do
    {
        if (cursor + 4 <= cmd_length)
        {
            word = 0;
            memcpy((void *)&word, (const void *)send_cmd + cursor, 4);
            xdma_reg_write(cursor + 8, word);
        }
        else if (cursor < cmd_length)
        {
            word = 0;
            memcpy((void *)&word, (const void *)send_cmd + cursor, cmd_length - cursor);
            xdma_reg_write(cursor + 8, word);
        }
        else // cursor >= cmd_length
        {
            break;
        }
        cursor += 4;
    } while (1);
    xdma_reg_write(0x4, cmd_length);

    // write correspondence type register 0x30304 as CMD_SHELL(0x3)
    xdma_reg_write(0x30304, 0x3);

    // write interrupt register 0x30300 as 1
    xdma_reg_write(0x30300, 0x01);

    // read a53 interrupt status 1000 times
    // 0:interrupt clear
    // !0:interrupt not clear
    // if a53 malfunction, should clear interrupt manually

    while (loop_count < 1000)
    {
        usleep(100);
        result = xdma_reg_read(0x30300);
        if (result == 0)
        {
            break;
        }
        loop_count += 1;
    }
    if (loop_count == 1000 && (result != 0))
    {
        xdma_reg_write(0x30300, 0x0);
        // str_result = "Interrupt state is error, A53 malfunction ";
        printf("Interrupt state is error, A53 malfunction \n");
        return -1;
    }

    // read cmd execute result at 0x30308
    // 0 OK !0 Error
    result = xdma_reg_read(0x30308);
    if (result != 0)
    {
        printf(" read cmd execute result error, A53 malfunction \n");
        return -1;
    }

    // read flag at 0x8000
    // 0x5a5aa5a5 ok
    word = xdma_reg_read(0x8000);
    if (word != 0x5a5aa5a5)
    {
        printf("read flag error, A53 malfunction \n");
        return -1;
    }
    // read cmd execute result data
    // read log length at 0x8004
    // read log data begin with 0x8008
    result_length = xdma_reg_read(0x8004);

    cursor = (result_length < RESULT_BUF_LENGTH) ? 0 : (result_length - result_length % RESULT_BUF_LENGTH);
    buf_cursor = 0;
    word = 0;
    memset(cmd_result, 0, RESULT_BUF_LENGTH);

    while (cursor < result_length)
    {
        cursor < result_length;
        word = xdma_reg_read(0x8008 + cursor);

        memcpy(cmd_result + buf_cursor, &word, 4);
        cursor += 4;
        buf_cursor += 4;
    }
    result_length = buf_cursor;
    printf("cmd result: \n%s\n", cmd_result);
    return 0;
}

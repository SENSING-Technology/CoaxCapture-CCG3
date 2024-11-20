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

#include "arm_pcie_protocol.h"
#include "link_margin.h"


char link_cmd[256];

uint8_t se_tx_amp_org;
uint8_t de_tx_amp_org;
void mdelay(int ms_count)
{
	usleep(ms_count * 1000);
}

uint8_t iic_reg16_rd8(uint32_t channel, uint8_t dev, uint16_t reg)
{
	uint8_t result;
	uint8_t reg_h, reg_l;
	reg_h = (reg >> 8) & 0xff;
	reg_l = reg & 0xff;
	memset(link_cmd, 0, 256);
	sprintf(link_cmd, "i2ctransfer -f -y %d w2@0x%02x 0x%02x 0x%02x r1\n", channel, dev/2, reg_h, reg_l);
	send_cmd_to_a53_sync(link_cmd);
	result = get_cmd_result();
	return result;
}

void iic_reg16_wr8(uint32_t channel, uint8_t dev, uint16_t reg, uint8_t value)
{
	uint8_t reg_h, reg_l;
	reg_h = (reg >> 8) & 0xff;
	reg_l = reg & 0xff;
	memset(link_cmd, 0, 256);
	sprintf(link_cmd, "i2ctransfer -f -y %d w3@0x%02x 0x%02x 0x%02x 0x%04x\n", channel, dev/2, reg_h, reg_l, value);
	send_cmd_to_a53_sync(link_cmd);
	return;
}



void link_margin_init(void *BaseAddress, uint32_t channel)
{
	uint8_t rd_data;
	
	uint8_t se_rlmsa4;
	uint8_t se_rlms4;
	uint8_t se_rlms3;
	uint8_t se_rlms95;
	uint8_t se_cnt0;
	
	uint8_t de_rlmsa4;
	uint8_t de_rlms4;
	uint8_t de_rlms3;
	uint8_t de_rlms95;
	uint8_t de_cnt0;

	printf ("%s ENTER!\n", __func__);
	//Determine TX rate on Deserializer
	rd_data = iic_reg16_rd8(channel, 0x90, DE_REG01);
	rd_data = (rd_data & 0xc)>>2;
	
	if(rd_data == 0x0) {
		de_tx_amp_org = 25;  //187Mbps
		printf ("Reverse channel 187Mbps\n");
	}
	else if(rd_data == 0x1) {
		de_tx_amp_org = 32;  //375Mbps
		printf ("Reverse channel 375Mbps\n");
	}
	else if(rd_data == 0x2) {
		de_tx_amp_org = 37;  //750Mbps
		printf ("Reverse channel 750Mbps\n");
	}
	else if(rd_data == 0x3) {
		de_tx_amp_org = 37;  //1.5Gbps
		printf ("Reverse channel 1.5Gbps\n");
	}
	mdelay(10);
	iic_reg16_wr8(channel, 0x90, DE_RLMS95, de_tx_amp_org);
	mdelay(10);

	//Determine TX rate on Serializer
	rd_data = iic_reg16_rd8(channel, 0x80, SE_REG01);
	rd_data = (rd_data & 0xc)>>2;
	
	if(rd_data == 0x0) {
		se_tx_amp_org = 41;  //1.5Gbps
		printf ("Forward channel 1.5Gbps\n");
	}
	else if(rd_data == 0x1)  {
		se_tx_amp_org = 41;  //3.0Gbps 	
		printf ("Forward channel 3.0Gbps\n");
	}
	else if(rd_data == 0x2)  {
		se_tx_amp_org = 41;  //6.0Gbps
		printf ("Forward channel 6.0Gbps\n");
	}
	mdelay(10);
	iic_reg16_wr8(channel, 0x80, SE_RLMS95, se_tx_amp_org);
	mdelay(10);
	
	//Turn off periodic  Adapt
	se_rlmsa4 = iic_reg16_rd8(channel, 0x80, SE_RLMSA4);
	mdelay(10);
	de_rlmsa4 = iic_reg16_rd8(channel, 0x90, DE_RLMSA4);
	mdelay(10);
	se_rlmsa4 &= ~0x3f;
	de_rlmsa4 &= ~0x3f;
	iic_reg16_wr8(channel, 0x80, SE_RLMSA4, se_rlmsa4);
	mdelay(10);
	iic_reg16_wr8(channel, 0x90, DE_RLMSA4, de_rlmsa4);
	mdelay(10);
	
	//Turn off EWM Width Monitor
	se_rlms4 = iic_reg16_rd8(channel, 0x80, SE_RLMS4);
	mdelay(10);
	de_rlms4 = iic_reg16_rd8(channel, 0x90, DE_RLMS4);
	mdelay(10);
	se_rlms4 &= ~0x1;
	de_rlms4 &= ~0x1;
	iic_reg16_wr8(channel, 0x80, SE_RLMS4, se_rlms4);
	mdelay(10);
	iic_reg16_wr8(channel, 0x90, DE_RLMS4, de_rlms4);
	mdelay(10);

	//Turn off Global Adapt
	se_rlms3 = iic_reg16_rd8(channel, 0x80, SE_RLMS3);
	mdelay(10);
	de_rlms3 = iic_reg16_rd8(channel, 0x90, DE_RLMS3);
	mdelay(10);
	se_rlms3 &= ~0x80;
	de_rlms3 &= ~0x80;
	iic_reg16_wr8(channel, 0x80, SE_RLMS3, se_rlms3);
	mdelay(10);
	iic_reg16_wr8(channel, 0x90, DE_RLMS3, de_rlms3);	
	mdelay(10);

	//Turn on Manual TX
	se_rlms95 = iic_reg16_rd8(channel, 0x80, SE_RLMS95);
	mdelay(10);
	de_rlms95 = iic_reg16_rd8(channel, 0x90, DE_RLMS95);
	mdelay(10);
	se_rlms95 &= ~0x80;
	de_rlms95 &= ~0x80;
	iic_reg16_wr8(channel, 0x80, SE_RLMS95, se_rlms95);
	mdelay(10);
	iic_reg16_wr8(channel, 0x90, DE_RLMS95, de_rlms95);	
	mdelay(10);
	
	//Clear Errors
	se_cnt0 = iic_reg16_rd8(channel, 0x80, SE_CNT0);
	mdelay(10);
	de_cnt0 = iic_reg16_rd8(channel, 0x90, DE_CNT0);
	mdelay(10);	
}

void forward_link_margin(void *BaseAddress, uint32_t channel)
{
	uint8_t i;
	uint32_t link_margin;
	uint8_t se_tx_amp;
	uint8_t se_rlms95;
	
	uint8_t de_rlmsa4;
	uint8_t de_rlms4;
	uint8_t de_rlms3;
	uint8_t de_cnt0;
	uint8_t de_ctrl3;
	
	printf ("%s TEST:\n", __func__);
	for(i=0; i< 20; i++) {
		de_cnt0  = iic_reg16_rd8(channel, 0x90, DE_CNT0);
		mdelay(10);
		de_ctrl3 = iic_reg16_rd8(channel, 0x90, DE_CTRL3);
		mdelay(10);
		if((de_cnt0 == 0) && (de_ctrl3 & 0x80)) break;
	}

	if((de_cnt0 != 0) || (0 == (de_ctrl3 & 0x80)))
	{
		printf("link error, exit test!\n");
		return;
	}	

	do
	{
		printf ("Link Margin...\n");
		//Decrease RLMS95
		se_rlms95 = iic_reg16_rd8(channel, 0x80, SE_RLMS95);
		mdelay(10);
		se_tx_amp = se_rlms95 & 0x3f;
		se_tx_amp--;	
		iic_reg16_wr8(channel, 0x80, DE_RLMS95, 0x80 | se_tx_amp);
		
		//wait 10ms
		mdelay(10);
		
		//Turn on Global Adapt
		de_rlms3 = iic_reg16_rd8(channel, 0x90, DE_RLMS3);
		mdelay(10);
		de_rlms3 |= 0x80;
		iic_reg16_wr8(channel, 0x90, DE_RLMS3, de_rlms3);
		
		//Wait for 100ms
		mdelay(100);
		
		//Turn off Global Adapt
		de_rlms3 = iic_reg16_rd8(channel, 0x90, DE_RLMS3);
		mdelay(10);
		de_rlms3 &= ~0x80;
		iic_reg16_wr8(channel, 0x90, DE_RLMS3, de_rlms3);
		mdelay(10);
		
		//Check for RXDP_LOCK
		for(i=0; i< 100; i++) {
			de_ctrl3 = iic_reg16_rd8(channel, 0x90, DE_CTRL3);
			mdelay(10);
			if(de_ctrl3 & 0x40) break;
		}
		
		//Clear Errors
		de_cnt0 = iic_reg16_rd8(channel, 0x90, DE_CNT0);
		
		//Delay 2 sec
		mdelay(2000);
		
		//Read Erros
		de_cnt0 = iic_reg16_rd8(channel, 0x90, DE_CNT0);
		mdelay(10);
	}
	while((de_cnt0 == 0) && (se_tx_amp > 10));
	
	//Link Margin
	link_margin = (se_tx_amp_org - se_tx_amp) * 10;
	printf ("Forward Link Margin = %d\n", link_margin);
	
	//Write Serilizer RLMS95 to Org
	iic_reg16_wr8(channel, 0x80, SE_RLMS95, 0x80 | se_tx_amp_org);
	mdelay(10);
	
	//Turn on Global Adapt
	de_rlms3 = iic_reg16_rd8(channel, 0x90, DE_RLMS3);
	mdelay(10);
	de_rlms3 |= 0x80;
	iic_reg16_wr8(channel, 0x90, DE_RLMS3, de_rlms3);
		
	//Wait for 100ms
	mdelay(100);
		
	//Turn off Global Adapt
	de_rlms3 = iic_reg16_rd8(channel, 0x90, DE_RLMS3);
	de_rlms3 &= ~0x80;
	mdelay(10);
	iic_reg16_wr8(channel, 0x90, DE_RLMS3, de_rlms3);
	mdelay(10);
}

void link_margin_complete(void *BaseAddress, uint32_t channel)
{
	uint8_t rd_data;
	
	printf ("%s FINISHED!\n", __func__);	
	//Clear Errors
	iic_reg16_rd8(channel, 0x80, SE_CNT0);
	mdelay(10);
	iic_reg16_rd8(channel, 0x90, DE_CNT0);	
	mdelay(10);
}

void link_margin_test(void *BaseAddress, uint32_t channel, uint32_t index)
{
	mdelay(10);
	memset(link_cmd, 0, 256);
	sprintf(link_cmd, "i2cdetect -r -y %d", channel);
	send_cmd_to_a53_sync(link_cmd);
	link_margin_init(BaseAddress, channel);
	forward_link_margin(BaseAddress, channel);
	link_margin_complete(BaseAddress, channel);
}

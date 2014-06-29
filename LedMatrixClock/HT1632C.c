#include "HT1632C.h"


void HT1632C_Writer(unsigned char Data,unsigned char cnt,unsigned char type)
{
	unsigned char i;
	if(type==CMD)
    {
        for(i=0;i<cnt;i++)
        {
            WR_low;
            if(Data&0x80)
                DATA_high;
            else
                DATA_low;
            Data<<=1;
            WR_high;
        }
    }
	else
    {
        for(i=0;i<cnt;i++)
        {
            WR_low;
            if(Data&0x01)
                DATA_high;
            else
                DATA_low;
            Data>>=1;
            WR_high;
        }
    }
}

void HT1632C_Writer_CMD(unsigned char cmd)
{
	CS_low;
	HT1632C_Writer(0x80,3,CMD);
	HT1632C_Writer(cmd,9,CMD);
	CS_high;
}

void HT1632C_Writer_DATA(unsigned char Addr,unsigned char Data)
{
	CS_low;
	HT1632C_Writer(0xa0,3,CMD);
	HT1632C_Writer(Addr<<1,7,CMD);
	HT1632C_Writer(Data,8,DAT);
	CS_high;
}

void HT1632C_Writer_AllDATA(unsigned char Addr,unsigned char *p,unsigned char cnt)
{
	unsigned char i;
	CS_low;
	HT1632C_Writer(0xa0,3,CMD);
	HT1632C_Writer(Addr<<1,7,CMD);
	for(i=0;i<cnt;i++)
    {
        HT1632C_Writer(*p,8,DAT);
        p++;
    }
	CS_high;
}

void HT1632C_clr(void)
{
	unsigned char i;
	CS_low;
	HT1632C_Writer(0xa0,3,CMD);
	HT1632C_Writer(0x00,7,CMD);
	for(i=0;i<32;i++)
		HT1632C_Writer(0x00,8,DAT);
	CS_high;
}

void HT1632C_Init(void)
{
	CS_high;
	WR_high;
	DATA_high;
	HT1632C_Writer_CMD(SYS_DIS);
	HT1632C_Writer_CMD(COM_OPTION);
	HT1632C_Writer_CMD(RC_MASTER_MODE);
	HT1632C_Writer_CMD(SYS_EN);
	HT1632C_Writer_CMD(PWM_DUTY);
	HT1632C_Writer_CMD(BLINK_OFF);
	HT1632C_Writer_CMD(LED_ON);
	HT1632C_clr();
}

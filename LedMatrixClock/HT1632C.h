//地址映射方式：
//模块电源接口朝左，按键朝右，从右至左定义8*8点阵为0，1，2，3号点阵
//则第i个点阵的地址为0x10*i~（0x10*i+0x0f），即0号点阵的地址为0x00~0x0f,1号地址为0x10~0x1f。。。
//详细地址如下：（以0号点阵为例）
//		COM0	COM1	COM2	COM3	 地址	COM4	COM5	COM6	COM7	 地址
//ROW0	D0		D1		D2		D3		|01H|	D4		D5		D6		D7		|00H|
//ROW1	D0		D1		D2		D3		|03H|	D4		D5		D6		D7		|02H|
//ROW2	D0		D1		D2		D3		|05H|	D4		D5		D6		D7		|04H|
//ROW3	D0		D1		D2		D3		|07H|	D4		D5		D6		D7		|06H|
//ROW4	D0		D1		D2		D3		|09H|	D4		D5		D6		D7		|08H|
//ROW5	D0		D1		D2		D3		|0BH|	D4		D5		D6		D7		|0AH|
//ROW6	D0		D1		D2		D3		|0DH|	D4		D5		D6		D7		|0CH|
//ROW7	D0		D1		D2		D3		|0FH|	D4		D5		D6		D7		|0EH|

#ifndef	_HT1632C_H_
#define	_HT1632C_H_

#include	<avr/io.h>
//指令表
#define SYS_DIS             0x00			   	//关闭系统震荡
#define SYS_EN              0x01			   	//打开系统震荡
#define LED_OFF             0x02			  	//关闭LED显示
#define LED_ON              0x03			   	//打开LED显示
#define BLINK_OFF           0x08		   	//关闭闪烁
#define	BLINK_ON            0X09		   	//打开闪烁
#define	SLAVE_MODE          0X10		   	//从模式
#define	RC_MASTER_MODE      0X18	   	//内部RC时钟
#define	EXT_CLK_MASTER_MODE	0X1C   	//外部时钟
#define COM_OPTION          0X20		   	//8COM，NMOS模式
#define PWM_DUTY            0XA0			//PWM亮度控制

#define	CMD                 1				//1代表发送命令，0为数据
#define	DAT                 0
//引脚定义
#define CS_high		PORTB|=(1<<PB3)
#define CS_low		PORTB&=~(1<<PB3)
#define WR_high		PORTB|=(1<<PB4)
#define WR_low		PORTB&=~(1<<PB4)
#define DATA_high	PORTB|=(1<<PB5)
#define DATA_low	PORTB&=~(1<<PB5)

void HT1632C_Writer(unsigned char Data,unsigned char cnt,unsigned char type);
void HT1632C_Writer_CMD(unsigned char cmd);
void HT1632C_Writer_DATA(unsigned char Addr,unsigned char Data);
void HT1632C_Writer_AllDATA(unsigned char Addr,unsigned char *p,unsigned char cnt);
void HT1632C_clr(void);
void HT1632C_Init(void);
#endif

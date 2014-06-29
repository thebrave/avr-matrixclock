//��ַӳ�䷽ʽ��
//ģ���Դ�ӿڳ��󣬰������ң�����������8*8����Ϊ0��1��2��3�ŵ���
//���i������ĵ�ַΪ0x10*i~��0x10*i+0x0f������0�ŵ���ĵ�ַΪ0x00~0x0f,1�ŵ�ַΪ0x10~0x1f������
//��ϸ��ַ���£�����0�ŵ���Ϊ����
//		COM0	COM1	COM2	COM3	 ��ַ	COM4	COM5	COM6	COM7	 ��ַ
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
//ָ���
#define SYS_DIS             0x00			   	//�ر�ϵͳ��
#define SYS_EN              0x01			   	//��ϵͳ��
#define LED_OFF             0x02			  	//�ر�LED��ʾ
#define LED_ON              0x03			   	//��LED��ʾ
#define BLINK_OFF           0x08		   	//�ر���˸
#define	BLINK_ON            0X09		   	//����˸
#define	SLAVE_MODE          0X10		   	//��ģʽ
#define	RC_MASTER_MODE      0X18	   	//�ڲ�RCʱ��
#define	EXT_CLK_MASTER_MODE	0X1C   	//�ⲿʱ��
#define COM_OPTION          0X20		   	//8COM��NMOSģʽ
#define PWM_DUTY            0XA0			//PWM���ȿ���

#define	CMD                 1				//1���������0Ϊ����
#define	DAT                 0
//���Ŷ���
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

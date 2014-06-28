#ifndef	_DS18B20_H_
#define	_DS18B20_H_
#define DQ_IN 	DDRC &= ~(1 << PC2)  	/*��������*/ 
#define DQ_OUT 	DDRC |= (1 << PC2)  	/*�������*/ 
#define DQ_CLR 	PORTC &= ~(1 << PC2) 	/*�õ͵�ƽ*/ 
#define DQ_SET 	PORTC |= (1 << PC2)  	/*�øߵ�ƽ*/ 
#define DQ_READ	PINC & (1 << PC2)  		/*�����ŵ�ƽ*/ 

#define	Skip_ROM			0xCC		//����ROMƥ��
#define	Convert				0x44		//�¶�ת��
#define	Read_Scratchpad		0xBE
#define	Write_Scratchpad	0x4E
//_delay_loop_1(10) 1M����ʱ10*3us
//_delay_us()	1M�´�10��ʼ�Ż�׼ȷ
unsigned char DS18B20_reset(void) 	//���߸�λ��������������Ƿ����
{
	unsigned char detect;
	DQ_OUT;
	DQ_CLR;
	_delay_us(550);		//����550us>480us
	DQ_IN;
	DQ_SET;				//�ͷ�����
//	DQ_IN;
	_delay_us(60);		//�ȴ���������
	detect=DQ_READ;
	_delay_us(240);
	return (detect);
}
void DS18B20_write_byte(unsigned char DATA)	//дһ���ֽڵ�18B20
{
	unsigned char i;
	for(i=0;i<8;i++)
		{
			DQ_OUT;
			DQ_CLR;			//���ͣ�>1us
		//	_delay_us(1);
			if(DATA&0x01)	DQ_SET;
			_delay_us(60);
			DQ_SET;
			DATA>>=1;
		}
}
unsigned char DS18B20_read_byte(void)
{
	unsigned char i,DATA=0;
	for(i=0;i<8;i++)
		{
			DATA>>=1;
			DQ_OUT;
			DQ_CLR;
		//	_delay_us(1);
			DQ_IN;
			DQ_SET;
//			DQ_IN;
			if(DQ_READ)	DATA|=0X80;
			_delay_us(60);
		}
	return DATA;
}
void adjust_res(unsigned char res) ///res �ֱ���� 0x1f, 0x3f, 0x5f �¶ȶ����ֱ��ʷֱ��Ӧ
                           //              0.5, 0.25, 0.125   
{
	DS18B20_reset();        		//��λ
	DS18B20_write_byte(0xcc);		//����Rom
	DS18B20_write_byte(0x4e);		//д�ݴ���
	DS18B20_write_byte(0x02);		//дTH 
	DS18B20_write_byte(0x01);		//дTL
	DS18B20_write_byte(res);		//д�ṹ�Ĵ���
	DS18B20_reset();				//��λ
	DS18B20_write_byte(0xcc);		//����Rom 
	DS18B20_write_byte(0x48);		//���ݴ�������д��EPRam��
}
/*unsigned int Read_Temperature(void)
{  	
	union
	{
		unsigned char c[2];
		unsigned int x;
	}temp;
	temp.x=0x0000;
	DS18B20_reset();
	DS18B20_write_byte(0xCC); 			//Skip ROM
	DS18B20_write_byte(0xBE); 			//Read Scratch Pad
	temp.c[0]=DS18B20_read_byte();		//Start read data
	temp.c[1]=DS18B20_read_byte();		//read two byte data from device
	DS18B20_reset();		   			//reset
	DS18B20_write_byte(0xCC); 			//Skip ROM
	DS18B20_write_byte(0x44); 			//Start Conversion
	return temp.x;
}*/
#endif

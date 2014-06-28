#ifndef	_DS18B20_H_
#define	_DS18B20_H_
#define DQ_IN 	DDRC &= ~(1 << PC2)  	/*设置输入*/ 
#define DQ_OUT 	DDRC |= (1 << PC2)  	/*设置输出*/ 
#define DQ_CLR 	PORTC &= ~(1 << PC2) 	/*置低电平*/ 
#define DQ_SET 	PORTC |= (1 << PC2)  	/*置高电平*/ 
#define DQ_READ	PINC & (1 << PC2)  		/*读引脚电平*/ 

#define	Skip_ROM			0xCC		//跳过ROM匹配
#define	Convert				0x44		//温度转换
#define	Read_Scratchpad		0xBE
#define	Write_Scratchpad	0x4E
//_delay_loop_1(10) 1M下延时10*3us
//_delay_us()	1M下从10开始才会准确
unsigned char DS18B20_reset(void) 	//总线复位函数，检测器件是否存在
{
	unsigned char detect;
	DQ_OUT;
	DQ_CLR;
	_delay_us(550);		//拉低550us>480us
	DQ_IN;
	DQ_SET;				//释放总线
//	DQ_IN;
	_delay_us(60);		//等待存在脉冲
	detect=DQ_READ;
	_delay_us(240);
	return (detect);
}
void DS18B20_write_byte(unsigned char DATA)	//写一个字节到18B20
{
	unsigned char i;
	for(i=0;i<8;i++)
		{
			DQ_OUT;
			DQ_CLR;			//拉低，>1us
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
void adjust_res(unsigned char res) ///res 分别等于 0x1f, 0x3f, 0x5f 温度读数分辨率分别对应
                           //              0.5, 0.25, 0.125   
{
	DS18B20_reset();        		//复位
	DS18B20_write_byte(0xcc);		//跳过Rom
	DS18B20_write_byte(0x4e);		//写暂存器
	DS18B20_write_byte(0x02);		//写TH 
	DS18B20_write_byte(0x01);		//写TL
	DS18B20_write_byte(res);		//写结构寄存器
	DS18B20_reset();				//复位
	DS18B20_write_byte(0xcc);		//跳过Rom 
	DS18B20_write_byte(0x48);		//把暂存器内容写到EPRam中
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

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "HT1632C.h"

unsigned char font_mini[10][3]= {
    {0b11111,0b10001,0b11111}, // 0
    {0b10010,0b11111,0b10000}, // 1
    {0b11101,0b10101,0b10111}, // 2
    {0b10001,0b10101,0b11111}, // 3
    {0b00111,0b00100,0b11110}, // 4
    {0b10111,0b10101,0b11101}, // 5
    {0b11111,0b10101,0b11101}, // 6
    {0b00001,0b00001,0b11111}, // 7
    {0b11111,0b10101,0b11111}, // 8
    {0b10111,0b10101,0b11111}, // 9
    //{0b,0b,0b},
};

unsigned char rtc_h, rtc_m, rtc_s;
unsigned char n;

#define MATRIX_COL  32
unsigned char framebuf[MATRIX_COL];

void framebuf_clear(void) {
    for(unsigned char i=0; i<MATRIX_COL; i++) {
        framebuf[i] = 0;
    }
}

void framebuf_send(void) {
    unsigned char buf[MATRIX_COL];
    unsigned char mid = 0;
    for (unsigned char i=0; i<MATRIX_COL; ++i) {
        if((0 == (i%8)) && (0 != i)) ++mid;
        unsigned char m = _BV(i%8);
        buf[i] = 0;
        for (unsigned char j=0; j<8; ++j) {
            if(0 != (framebuf[j + (mid*8)] & m)) {
                buf[i] |= _BV(j);
            }
        }
        
    }
    
    HT1632C_Writer_AllDATA(0, buf, MATRIX_COL);
}

void framebuf_setXY(unsigned char x, unsigned char y, unsigned char pixel) {
    if(0 == pixel)
        framebuf[x] &= (unsigned char)0xff - _BV(y);
    else
        framebuf[x] |= _BV(y);
}

void framebuf_printchar(unsigned char v, unsigned char x, unsigned char y) {
    for(unsigned char i=0; i<3; ++i) {
        framebuf[x+i] = font_mini[v][i] << y;
    }
}


SIGNAL(TIMER2_COMP_vect) {
    if(60 == ++rtc_s) {
        rtc_s = 0;
        if(60 == ++rtc_m) {
            rtc_m = 0;
            if(24 == ++rtc_h) {
                rtc_h = 0;
            }
        }
    }
    
    /*framebuf_clear();
    
    for(unsigned char i=0; i<8; i++) {
        framebuf_printchar(i, i*4, i%4);
    }
    framebuf_send();*/
    
    clock_printscreen_main();
    
    n++;
    n = n%MATRIX_COL;
}

void initRTC(void) {
    rtc_s = 0;
    rtc_m = 0;
    rtc_h = 0;
    
    TCCR2 = 0;                  // Timer/counter2 disable
    TCNT2 = 0;                  // Value reset
    OCR2 = 32;                  // We should match compare on 32
    TIMSK |= _BV(OCIE2);              // Match throw signal TIMER2_COMP_vect
    ASSR |= _BV(AS2);          // Timer/Counter2 source is 32k crystal
    TCCR2 = _BV(WGM21) | _BV(CS22) | _BV(CS21) | _BV(CS20);   // CTC mode, start with prescaler value of 1024
    SREG=SREG|0x80;
}

void initMatrix(void) {
    HT1632C_Init();
}

void initIO(void) {
	DDRB|=(1<<PB3)|(1<<PB4)|(1<<PB5);		//配置为输出
	PORTB|=(1<<PB3)|(1<<PB4)|(1<<PB5);		//输出高
	DDRD&=~((1<<PD5)|(1<<PD6)|(1<<PD7));				//输入
	PORTD|=(1<<PD5)|(1<<PD6)|(1<<PD7);				//使能内部上拉
}


unsigned char util_gettensdigit(unsigned char x) {
    return (x - (x % 10)) / 10;
}

#define MAIN_HROFF  2
void clock_printscreen_main() {
    framebuf_clear();
    
    framebuf_printchar(util_gettensdigit(rtc_h), MAIN_HROFF, 1);
    framebuf_printchar(rtc_h % 10, MAIN_HROFF+4, 1);
    
    framebuf_printchar(util_gettensdigit(rtc_m), MAIN_HROFF+4+4+2, 1);
    framebuf_printchar(rtc_m % 10, MAIN_HROFF+4+4+2+4, 1);
    
    framebuf_printchar(util_gettensdigit(rtc_s), MAIN_HROFF+4+4+2+4+4+1, 2);
    framebuf_printchar(rtc_s % 10, MAIN_HROFF+4+4+2+4+4+4+1, 2);
    
    framebuf_setXY(MAIN_HROFF+4+3+1, 2, 1);
    framebuf_setXY(MAIN_HROFF+4+3+1, 4, 1);
    
    framebuf_send();
}

int main(void) {
    _delay_ms(100);
    
    initIO();
    
    initMatrix();

    n=0;
    
    HT1632C_clr();
    
    unsigned char i;
	CS_low;
	HT1632C_Writer(0xa0,3,CMD);
	HT1632C_Writer(0x00,7,CMD);
	for(i=0;i<32;i++)
		HT1632C_Writer(0xff,8,DAT);
	CS_high;
    
    _delay_ms(250);
    
    HT1632C_clr();
    
    _delay_ms(250);
    
    initRTC();
    
    while (1) {
        
    }
    
    return 0; // never reached
}


#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "HT1632C.h"

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
        buf[i] = 0;
        for (unsigned char j=0; j<8; ++j) {
            if(0 != (framebuf[j + (mid*8)] & _BV(j))) {
                buf[i] |= _BV(j);
            }
        }
        
    }
    
    HT1632C_Writer_AllDATA(0, buf, MATRIX_COL);
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
    
    framebuf_clear();
    for(unsigned char i=0; i<8; i++) {
        framebuf[i] = 0xff;
    }
    framebuf_send();
    
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


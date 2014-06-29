#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "clock_fsm.h"
#include "framebuffer.h"

#include "HT1632C.h"

unsigned char rtc_h, rtc_m, rtc_s;
unsigned char n;

extern unsigned char   fsm_need_redraw;

/*
 if (0 == n%2) {
 unsigned char i;
 CS_low;
 HT1632C_Writer(0xa0,3,CMD);
 HT1632C_Writer(0x00,7,CMD);
 for(i=0;i<32;i++)
 HT1632C_Writer(0xff,8,DAT);
 CS_high;
 } else {
 HT1632C_clr();
 }
 
 n++;
 n = n%MATRIX_COL;
 */

unsigned char rtc_enable;

SIGNAL(TIMER2_COMP_vect) {
    if(0 == rtc_enable) {
        return;
    }
    
    if(60 == ++rtc_s) {
        rtc_s = 0;
        if(60 == ++rtc_m) {
            rtc_m = 0;
            if(24 == ++rtc_h) {
                rtc_h = 0;
            }
        }
    }
    
    fsm_need_redraw = 1;
}


#define DOWN		PD5
#define UP			PD6
#define MENU			PD7
#define BTN_MAX_DELAY 0xff
#define BTN_SHORT     50
#define BTN_LONG      200

void tick_button(unsigned char mask, unsigned char *cnt, e_action sht, e_action lng) {
    if(0 == (PIND & mask)) {
        if (*cnt < BTN_MAX_DELAY) {
            ++(*cnt);
        }
    } else {
        if (*cnt > BTN_LONG) {
            fsm_transition(lng);
        } else if (*cnt > BTN_SHORT) {
            fsm_transition(sht);
        }
        *cnt = 0;
    }
}

unsigned char btn_menu_cnt, btn_up_cnt, btn_down_cnt;
SIGNAL(TIMER0_OVF_vect) {
    tick_button(_BV(MENU), &btn_menu_cnt, a_menu_short, a_menu_long);
    tick_button(_BV(UP), &btn_up_cnt, a_plus_short, a_plus_long);
    tick_button(_BV(DOWN), &btn_down_cnt, a_minus_short, a_minus_long);
    
    if(1 == fsm_need_redraw) {
        fsm_need_redraw = 0;
        fsm_display();
    }

    /*
    framebuf_setXY(30, 0, 0 == (PIND & _BV(MENU)) ? 1 : 0);
    framebuf_setXY(30, 1, 0 == (PIND & _BV(UP)) ? 1 : 0);
    framebuf_setXY(30, 2, 0 == (PIND & _BV(DOWN)) ? 1 : 0);
    framebuf_send();*/
}

void initTick(void) {
    btn_menu_cnt = 0;
    btn_up_cnt = 0;
    btn_down_cnt = 0;
    
    TCCR0 = 0;
    TCNT0 = 0;
    TIMSK |= _BV(TOIE0);
    TCCR0 = _BV(CS01);
}

void initRTC(void) {
    rtc_enable = 1;
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
    initTick();
    
    while (1) {
        
    }
    
    return 0; // never reached
}


//
//  clock_fsm.cpp
//  LedMatrixClock
//
//  Created by Jean Berniolles on 28/06/2014.
//  Copyright (c) 2014 Jean Berniolles. All rights reserved.
//

#include "main.h"

#include "clock_fsm.h"
#include "framebuffer.h"
#include "HT1632C.h"

#define BLINK_ON    0xff
#define BLINK_OFF   0

extern unsigned char framebuf[MATRIX_COL];
extern unsigned char rtc_h, rtc_m, rtc_s;
extern unsigned char rtc_enable;
extern unsigned char tick_alarm;

e_state         fsm_state;
unsigned char   fsm_need_redraw;
unsigned char   fsm_2Hz;

void fsm_init(void) {
    fsm_state = s_idle;
    fsm_need_redraw = 1;
    fsm_2Hz = BLINK_OFF;
}

unsigned char util_reverse_blinker(unsigned char x) {
    return ((x == BLINK_ON) ? BLINK_OFF : BLINK_ON);
}

unsigned char util_gettensdigit(unsigned char x) {
    return (x - (x % 10)) / 10;
}

#define MAIN_HROFF  1
#define SM_W        4
#define LM_W        5
void fsm_display() {
    framebuf_clear();
    
    unsigned char i;
    
    switch (fsm_state) {
        case s_idle:
            inv_l = MATRIX_COL + 1;
            inv_h = MATRIX_COL;
            break;
        case s_config_hour:
            inv_l = MAIN_HROFF - 1;
            inv_h = MAIN_HROFF + 2*LM_W - 1;
            break;
        case s_config_min:
            inv_l = MAIN_HROFF + 2*LM_W + 1;
            inv_h = MAIN_HROFF + 4*LM_W + 1;
            break;
            
        default: {
            CS_low;
            HT1632C_Writer(0xa0,3,CMD);
            HT1632C_Writer(0x00,7,CMD);
            for(i=0;i<32;i++)
                HT1632C_Writer(0xff,8,DAT);
            CS_high;
            return;
            break;
        }
    }
    
    if(!(fsm_state == s_config_hour && fsm_2Hz == BLINK_OFF)) {
        framebuf_printbigchar(util_gettensdigit(rtc_h), MAIN_HROFF);
        framebuf_printbigchar(rtc_h % 10, MAIN_HROFF+LM_W);
    }
    
    if(!(fsm_state == s_config_min && fsm_2Hz == BLINK_OFF)) {
        framebuf_printbigchar(util_gettensdigit(rtc_m), MAIN_HROFF+2*LM_W+2);
        framebuf_printbigchar(rtc_m % 10, MAIN_HROFF+3*LM_W+2);
    }
    
    if(fsm_state == s_idle) {
        framebuf_printchar(util_gettensdigit(rtc_s), MAIN_HROFF+4*LM_W+3, 3);
        framebuf_printchar(rtc_s % 10, MAIN_HROFF+4*LM_W+SM_W+3, 3);
    }
    
    if(fsm_2Hz == BLINK_OFF || fsm_state == s_config_hour) {
        framebuf_setXY(MAIN_HROFF+2*LM_W, 2, 1);
        framebuf_setXY(MAIN_HROFF+2*LM_W, 5, 1);
    }
    
    framebuf_send();
}

void fsm_transition(e_action evt) {
    switch (fsm_state) {
        case s_idle:
            switch (evt) {
                case a_menu_long:
                    rtc_enable = 0;
                    fsm_state = s_config_hour;
                    tick_alarm = SYS_TICK_DELAY_500MS;
                    fsm_need_redraw = 1;
                    break;
                    
                case a_rtc_second:
                    tick_alarm = SYS_TICK_DELAY_500MS;
                    fsm_2Hz = 1;
                    fsm_need_redraw = 1;
                    break;
                    
                case a_tick_alarm:
                    fsm_2Hz = 0;
                    fsm_need_redraw = 1;
                    break;
                    
                default:
                    break;
            }
            break;
        case s_config_hour:
            switch (evt) {
                case a_menu_short:
                case a_menu_long:
                    fsm_state = s_config_min;
                    tick_alarm = SYS_TICK_DELAY_500MS;
                    fsm_need_redraw = 1;
                    break;
                    
                case a_plus_short:
                    ++rtc_h;
                    if(rtc_h == 24) {
                        rtc_h = 0;
                    }
                    tick_alarm = SYS_TICK_DELAY_500MS;
                    fsm_2Hz = 1;
                    fsm_need_redraw = 1;
                    break;
                    
                case a_minus_short:
                    if (rtc_h == 0) {
                        rtc_h = 23;
                    } else {
                        --rtc_h;
                    }
                    tick_alarm = SYS_TICK_DELAY_500MS;
                    fsm_2Hz = 1;
                    fsm_need_redraw = 1;
                    break;
                    
                case a_tick_alarm:
                    fsm_2Hz ^= 0xff;
                    tick_alarm = SYS_TICK_DELAY_500MS;
                    fsm_need_redraw = 1;
                    break;
                    
                default:
                    break;
            }
            break;
        case s_config_min:
            switch (evt) {
                case a_menu_short:
                case a_menu_long:
                    rtc_s = 0;
                    fsm_state = s_idle;
                    rtc_enable = 1;
                    fsm_need_redraw = 1;
                    break;
                case a_plus_short:
                    ++rtc_m;
                    if(rtc_m == 60) {
                        rtc_m = 0;
                    }
                    tick_alarm = SYS_TICK_DELAY_500MS;
                    fsm_2Hz = 1;
                    fsm_need_redraw = 1;
                    break;
                case a_minus_short:
                    if (rtc_m == 0) {
                        rtc_m = 59;
                    } else {
                        --rtc_m;
                    }
                    tick_alarm = SYS_TICK_DELAY_500MS;
                    fsm_2Hz = 1;
                    fsm_need_redraw = 1;
                    break;
                    
                case a_tick_alarm:
                    fsm_2Hz ^= 0xff;
                    tick_alarm = SYS_TICK_DELAY_500MS;
                    fsm_need_redraw = 1;
                    break;
                    
                default:
                    break;
            }
            break;
            
        default:
            break;
            
    }
}
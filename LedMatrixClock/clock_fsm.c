//
//  clock_fsm.cpp
//  LedMatrixClock
//
//  Created by Jean Berniolles on 28/06/2014.
//  Copyright (c) 2014 Jean Berniolles. All rights reserved.
//

#include "clock_fsm.h"
#include "framebuffer.h"
#include "HT1632C.h"

extern unsigned char framebuf[MATRIX_COL];
extern unsigned char rtc_h, rtc_m, rtc_s;
extern unsigned char rtc_enable;

e_state         fsm_state;
unsigned char   fsm_need_redraw;

unsigned char util_gettensdigit(unsigned char x) {
    return (x - (x % 10)) / 10;
}

#define MAIN_HROFF  2
#define SM_W        4
void fsm_display() {
    unsigned char inv_l, inv_h;
    framebuf_clear();
    
    unsigned char i;
    
    switch (fsm_state) {
        case s_idle:
            inv_l = MATRIX_COL + 1;
            inv_h = MATRIX_COL;
            break;
        case s_config_hour:
            inv_l = MAIN_HROFF - 1;
            inv_h = MAIN_HROFF + 2*SM_W - 1;
            break;
        case s_config_min:
            inv_l = MAIN_HROFF + 2*SM_W + 1;
            inv_h = MAIN_HROFF + 4*SM_W + 1;
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
    
    
    framebuf_printchar(util_gettensdigit(rtc_h), MAIN_HROFF, 1);
    framebuf_printchar(rtc_h % 10, MAIN_HROFF+SM_W, 1);
    
    framebuf_printchar(util_gettensdigit(rtc_m), MAIN_HROFF+2*SM_W+2, 1);
    framebuf_printchar(rtc_m % 10, MAIN_HROFF+3*SM_W+2, 1);
    
    if(fsm_state == s_idle) {
        framebuf_printchar(util_gettensdigit(rtc_s), MAIN_HROFF+4*SM_W+2+1, 2);
        framebuf_printchar(rtc_s % 10, MAIN_HROFF+5*SM_W+2+1, 2);
    }
    
    framebuf_setXY(MAIN_HROFF+4+3+1, 2, 1);
    framebuf_setXY(MAIN_HROFF+4+3+1, 4, 1);
    
    if(inv_l < inv_h) {
        for (; inv_l <= inv_h; ++inv_l) {
            framebuf[inv_l] ^= 0xff;
        }
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
                    fsm_need_redraw = 1;
                    break;
                case a_plus_short:
                    ++rtc_h;
                    if(rtc_h == 24) {
                        rtc_h = 0;
                    }
                    fsm_need_redraw = 1;
                    break;
                case a_minus_short:
                    if (rtc_h == 0) {
                        rtc_h = 23;
                    } else {
                        --rtc_h;
                    }
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
                    fsm_need_redraw = 1;
                    break;
                case a_minus_short:
                    if (rtc_m == 0) {
                        rtc_m = 59;
                    } else {
                        --rtc_m;
                    }
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
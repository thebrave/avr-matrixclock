//
//  clock_fsm.h
//  LedMatrixClock
//
//  Created by Jean Berniolles on 28/06/2014.
//  Copyright (c) 2014 Jean Berniolles. All rights reserved.
//

#ifndef __clock_fsm__
#define __clock_fsm__

typedef enum {
    s_idle,
    s_config_hour,
    s_config_min,
    s_config_sec,
} e_state;

typedef enum {
    a_menu_short,
    a_menu_long,
    a_plus_short,
    a_plus_long,
    a_minus_short,
    a_minus_long,
    a_tick_alarm,
    a_rtc_second,
} e_action;

unsigned char util_gettensdigit(unsigned char x);

void fsm_init(void);
void fsm_display(void);
void fsm_transition(e_action evt);

#endif /* defined(__clock_fsm__) */

//
//  framebuffer.cpp
//  LedMatrixClock
//
//  Created by Jean Berniolles on 29/06/2014.
//  Copyright (c) 2014 Jean Berniolles. All rights reserved.
//

#include "framebuffer.h"
#include "HT1632C.h"

unsigned char framebuf[MATRIX_COL];

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
//
//  framebuffer.h
//  LedMatrixClock
//
//  Created by Jean Berniolles on 29/06/2014.
//  Copyright (c) 2014 Jean Berniolles. All rights reserved.
//

#ifndef __framebuffer__
#define __framebuffer__

#include <avr/io.h>
#include <avr/interrupt.h>

#define MATRIX_COL  32

void framebuf_clear(void);
void framebuf_send(void);
void framebuf_setXY(unsigned char x, unsigned char y, unsigned char pixel);
void framebuf_printchar(unsigned char v, unsigned char x, unsigned char y);

#endif /* defined(__framebuffer__) */

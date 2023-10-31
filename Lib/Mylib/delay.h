#ifndef DELAY_H
#define DELAY_H

#include "common_defs.h"


void delay_init(void);
void delay_us(uint32_t n);
void delay_ms(uint32_t n);

#endif
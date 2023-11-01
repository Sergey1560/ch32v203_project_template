#ifndef CLOCKS_H
#define CLOCKS_H
#include "ch32v20x.h"

#define PLL_MUL           RCC_PLLMULL18
//APB2 clk
#define RCC_PPRE2_VAL     RCC_PPRE2_DIV2
//APB1 clk
#define RCC_PPRE1_VAL     RCC_PPRE1_DIV2
//AHB clk
#define RCC_HPRE_VAL      RCC_HPRE_DIV1

/* Тактирование шин */
//#define APB1_CLK 72000000UL
#define APB2_CLK 72000000UL


#endif
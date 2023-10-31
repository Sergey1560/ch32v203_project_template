#include "rcc.h"


void rcc_init(void){
    uint32_t tmp;

    RCC->CTLR |= ((uint32_t)RCC_HSEON);
    while((RCC->CTLR & RCC_HSERDY) == 0){__NOP();};

    tmp = RCC->CFGR0;
    tmp &= ~(RCC_PLLSRC | RCC_PLLXTPRE | RCC_PLLMULL | RCC_SW);
    tmp |= (RCC_PPRE2_VAL | RCC_PPRE1_VAL | RCC_HPRE_VAL | PLL_MUL| RCC_PLLSRC);
    RCC->CFGR0 = tmp;

    RCC->CTLR |= RCC_PLLON;
    while((RCC->CTLR & RCC_PLLRDY) == 0){__NOP();}

    RCC->CFGR0 |= (uint32_t)RCC_SW_PLL;
    while ((RCC->CFGR0 & RCC_SWS) != RCC_SWS_PLL){__NOP();}

    SystemCoreClockUpdate();
}
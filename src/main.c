#include "uart.h"
#include "log.h"
/*
LED PA15
*/

int main(void){
    uint32_t tmp;
    uint8_t i = 0;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    Delay_Init();
    
    uart_init();
    DEBUG("SystemClk: %ld", SystemCoreClock);

    RCC->APB2PCENR |= RCC_APB2Periph_GPIOA;
    tmp = GPIOA->CFGHR;
    tmp &= ~GPIO_CFGHR_CNF15;
    tmp |= GPIO_CFGHR_MODE15;
    GPIOA->CFGHR = tmp;

    while(1){
        Delay_Ms(500);
        
        if(i){
            GPIOA->BSHR = GPIO_BSHR_BR15;
            DEBUG("Led off");
            i=0;
        }else{
            GPIOA->BSHR = GPIO_BSHR_BS15;
            DEBUG("Led on");
            i=1;
        }
    }
}

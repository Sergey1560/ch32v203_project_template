#include "common_defs.h"
#include "rcc.h"
/*
LED PA15
*/
void tim2_init(void){
	RCC->APB1PCENR |= RCC_APB1Periph_TIM2;
	
	TIM2->PSC = 36000 - 1; 
	TIM2->ATRLR = 2000;
	TIM2->CTLR1 |= TIM_ARPE;
	TIM2->DMAINTENR |= TIM_UIE;
	
	NVIC_EnableIRQ(TIM2_IRQn);
	TIM2->CTLR1 |= TIM_CEN;
}


void __attribute__((interrupt("machine"))) TIM2_IRQHandler(void){
	static uint8_t i = 0;
	static uint32_t count = 0;

	if(TIM2->INTFR & TIM_UIF){
		TIM2->INTFR &= ~TIM_UIF;
	}

        if(i){
            GPIOA->BSHR = GPIO_BSHR_BR15;
            i=0;
        }else{
            GPIOA->BSHR = GPIO_BSHR_BS15;
            i=1;
        }

		DEBUG("Timer: %ld",count++);
}


int main(void){
    uint32_t tmp;

    rcc_init();

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    delay_init();
    
    uart_init(115200);
    DEBUG("SystemClk: %ld", SystemCoreClock);

    RCC->APB2PCENR |= RCC_APB2Periph_GPIOA;
    tmp = GPIOA->CFGHR;
    tmp &= ~GPIO_CFGHR_CNF15;
    tmp |= GPIO_CFGHR_MODE15;
    GPIOA->CFGHR = tmp;

    tim2_init();

    while(1){
    }
}

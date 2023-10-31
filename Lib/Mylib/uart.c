#include "uart.h"

/*
PA9  USART1 TX Push-pull alternate output
PA10 USART1 RX Floating input or pull-up input
*/
void uart_init(uint32_t baudrate){
    uint32_t tmp;

    RCC->APB2PCENR |= RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA;

    tmp = GPIOA->CFGHR;
    tmp &= ~GPIO_CFGHR_CNF9;
    tmp |= GPIO_CFGHR_CNF9_1;
    tmp |= GPIO_CFGHR_MODE9;
    GPIOA->CFGHR = tmp;

    USART1->CTLR1 = USART_CTLR1_TE;
    USART1->CTLR2 = 0;
    USART1->CTLR3 = 0;
    USART1->BRR = (APB2_CLK)/baudrate;

    USART1->CTLR1 |= USART_CTLR1_UE;
}


void uart_send_char(char data){
    while(!(USART1->STATR & USART_STATR_TXE)){__NOP();};
    USART1->DATAR = data;
}

void uart_send_string(char *s) {
	while(*s) {
	uart_send_char(*s++);
	};
};

void uart_send_bin(uint8_t *data, uint16_t len) {
	while(len--) {
		uart_send_char((char)*data++);
	};
};



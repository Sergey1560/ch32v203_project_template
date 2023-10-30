#include "uart.h"

char buffer[UART_DBG_BUFF_LEN];

void uart_init(void){
    uint32_t tmp;

    //PA9 USART1 TX
    RCC->APB2PCENR |= RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA;

    tmp = GPIOA->CFGHR;
    tmp &= ~GPIO_CFGHR_CNF9;
    tmp |= GPIO_CFGHR_CNF9_1;
    tmp |= GPIO_CFGHR_MODE9;
    GPIOA->CFGHR = tmp;

    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);


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


void debug_to_uart(char *fmt,...){
    int ret_val;
    va_list ParamList;
    char *ptr = (char *)buffer;
    
    va_start(ParamList, fmt);
    ret_val = vsnprintf (buffer, UART_DBG_BUFF_LEN-1, fmt, ParamList);
    va_end(ParamList);

    if(ret_val){
        uart_send_string(ptr);
    }
}

int _write(int fd, char *buf, int size)
{
    uart_send_bin((uint8_t *)buf, size);
    return size;
}

/*********************************************************************
 * @fn      _sbrk
 *
 * @brief   Change the spatial position of data segment.
 *
 * @return  size: Data length
 */
void *_sbrk(ptrdiff_t incr)
{
    extern char _end[];
    extern char _heap_end[];
    static char *curbrk = _end;

    if ((curbrk + incr < _end) || (curbrk + incr > _heap_end))
    return NULL - 1;

    curbrk += incr;
    return curbrk - incr;
}

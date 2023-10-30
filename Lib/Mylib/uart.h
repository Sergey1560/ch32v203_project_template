#ifndef UART_H
#define UART_H

#include "stdio.h"
#include "stdarg.h"
#include "ch32v20x.h"

#define UART_DBG_BUFF_LEN       200

void uart_init(void);
void debug_to_uart(char *fmt,...);
void uart_send_bin(uint8_t *data, uint16_t len);
void uart_send_string(char *s);
void uart_send_char(char data);

#endif
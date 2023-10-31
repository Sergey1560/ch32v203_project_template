#ifndef UART_H
#define UART_H

#include "common_defs.h"


void uart_init(uint32_t baudrate);
void uart_send_bin(uint8_t *data, uint16_t len);
void uart_send_string(char *s);
void uart_send_char(char data);

#endif
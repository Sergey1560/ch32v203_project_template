#include "log.h"

#ifdef DEBUG_TO_UART
char buffer[UART_DBG_BUFF_LEN];

void debug_to_uart(char *fmt,...){
    int ret_val;
    va_list ParamList;
    
    va_start(ParamList, fmt);
    ret_val = vsnprintf (buffer, UART_DBG_BUFF_LEN-1, fmt, ParamList);
    va_end(ParamList);

    if(ret_val > 0){
        uart_send_bin((uint8_t *)buffer, ret_val);
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
#endif
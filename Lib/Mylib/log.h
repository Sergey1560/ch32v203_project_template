#ifndef LOG_H
#define LOG_H
#include "uart.h"

#define DEBUG_TO_UART

#define DEBUG_MSG
//#define INFO_MSG
//#define WARNING_MSG
//#define ERROR_MSG

#ifdef DEBUG_TO_UART
    #include "stdio.h"
    #include "stdarg.h"
    void debug_to_uart(char *fmt,...);

    #define UART_DBG_BUFF_LEN       200
    
    #ifdef DEBUG_MSG
        #define DEBUG(fmt, args...) debug_to_uart((char*)"[D] %s:%-4d :" fmt "\r\n", __func__, __LINE__, ## args)
    #else
        #define DEBUG(fmt, args...)
    #endif
    #ifdef INFO_MSG
        #define INFO(fmt, args...)  debug_to_uart((char*)"[I] %-20s:%-4d :" fmt "\n", __func__, __LINE__, ## args)
    #else
        #define INFO(fmt, args...)
    #endif
    #ifdef WARNING_MSG
        #define WARNING(fmt, args...) debug_to_uart((char*)"[W] %-20s:%-4d :" fmt "\n",  __func__, __LINE__, ## args)
    #else
        #define WARNING(fmt, args...)
    #endif
    #ifdef ERROR_MSG
        #define ERROR(fmt, args...) debug_to_uart((char*)"[E] %-20s:%-4d :" fmt "\n",  __func__, __LINE__,## args)
    #else
        #define ERROR(fmt, args...)
    #endif
#else
    #ifdef DEBUG_MSG
        #define DEBUG(fmt, args...)
    #endif
    #ifdef INFO_MSG
        #define INFO(fmt, args...)
    #endif
    #ifdef WARNING_MSG
        #define WARNING(fmt, args...)
    #endif
    #ifdef ERROR_MSG
        #define ERROR(fmt, args...)
    #endif
#endif

#endif

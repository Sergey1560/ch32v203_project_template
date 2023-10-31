#include "ch32v20x.h"
#include "log.h"
#include "clocks.h"
#include "delay.h"

#define ALGN4 __attribute__ ((aligned (4)))
#define ALGN8 __attribute__ ((aligned (8)))
#define ALGN32 __attribute__ ((aligned (32)))


/* GPIO SPEED */
#define S_LOW 0
#define S_MED 1
#define S_HI  2
#define S_VH  3


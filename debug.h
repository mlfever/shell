#include <stdio.h>

/* for debug output */
#define SHELL_INFO(fm, args...) \
    printf("INFO %s %s %5d: " fm, __FILE__, __func__, __LINE__,  ##args)

#define SHELL_ERR(fm, args...) \
    printf("XX   %s %s %5d: " fm, __FILE__, __func__, __LINE__,  ##args)

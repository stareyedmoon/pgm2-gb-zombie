#include <gb/gb.h>

#ifndef _debug_h_INCLUDE
#define _debug_h_INCLUDE

#ifdef DEBUG

#include <stdio.h>

#define ASSERT(expr, msg) do {\
        if (!(expr)) {\
            puts(msg);\
            while (1) { vsync() }\
        }\
    } while (0)

#define BURN(msg) do {\
        puts(msg)\
        while (1) { vsync(); }\
    } while (0)

#else

#define ASSERT(expr, msg)

#endif

#endif // _debug_h_INCLUDE

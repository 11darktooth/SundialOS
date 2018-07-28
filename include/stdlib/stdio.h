#ifndef __SUNDIALOS_STDLIB_STDIO_H
#define __SUNDIALOS_STDLIB_STDIO_H 

#include <stdlib/types.h>

namespace SundialOS
{
    namespace stdlib
    {
        void printf(const char* msg);
        void printfHex(uint8_t key);
    }
}

#endif

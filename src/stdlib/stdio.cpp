#include <stdlib/stdio.h>

using namespace SundialOS::stdlib;

namespace SundialOS
{
    namespace stdlib
    {

        void printf(const char* msg)
        {
            static uint16_t* VideoMemory = (uint16_t*)0xb8000;
            
            static uint8_t x = 0, y = 0;

            for(int i = 0; msg[i] != '\0'; i++)
            {

                switch(msg[i])
                {
                    case '\n':
                        y++;
                        x = 0;
                        break;
                    case '\t':
                        x += 4;
                        break;
                    case '\b':
                        x--;
                        if(x<0) x = 0;
                        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';
                        break;
                    default:
                        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | msg[i];
                        x++;
                        break;
                }

                if(x >= 80)
                {
                    y++;
                    x = 0;
                }

                if(y >= 25)
                {
                    for(y = 0; y < 25; y++)
                    {
                        for(x = 0; x < 80; x++)
                        {
                            VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';
                        }
                    }
                    x = 0; y = 0;
                }
            }
        }

        void printfHex(uint8_t key)
        {
            char* foo = "00";
            char* hex = "0123456789ABCDEF";
            foo[0] = hex[(key >> 4) & 0x0F];
            foo[1] = hex[key & 0x0F];
            printf(foo);
        }
    }
}

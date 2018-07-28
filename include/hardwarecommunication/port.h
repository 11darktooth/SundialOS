#ifndef __SUNDIALOS_HARDWARECOMMUNICATION_PORT_H
#define __SUNDIALOS_HARDWARECOMMUNICATION_PORT_H

#include <stdlib/stdio.h> 

namespace SundialOS
{
    namespace hardwarecommunication
    {

        class Port
        {
        protected:
            SundialOS::stdlib::uint16_t portNumber;
            Port(SundialOS::stdlib::uint16_t portNumber); // purely virtual base class
            ~Port();
        };

        class Port8Bit : public Port
        {
        public:
            Port8Bit(SundialOS::stdlib::uint16_t portNumber); // purely virtual base class
            ~Port8Bit();
            virtual void Write(SundialOS::stdlib::uint8_t data);
            virtual SundialOS::stdlib::uint8_t Read();
        protected:
            static inline SundialOS::stdlib::uint8_t Read8(SundialOS::stdlib::uint16_t _port)
            {
                SundialOS::stdlib::uint8_t result;
                __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (_port));
                return result;
            }

            static inline void Write8(SundialOS::stdlib::uint16_t _port, SundialOS::stdlib::uint8_t _data)
            {
                __asm__ volatile("outb %0, %1" : : "a" (_data), "Nd" (_port));
            }
        };


        class Port8BitSlow : public Port8Bit
        {
        public:
            Port8BitSlow(SundialOS::stdlib::uint16_t portNumber); // purely virtual base class
            ~Port8BitSlow();
            virtual void Write(SundialOS::stdlib::uint8_t data);

        protected:
            static inline void Write8Slow(SundialOS::stdlib::uint16_t _port, SundialOS::stdlib::uint8_t _data)
            {
                __asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:" : : "a" (_data), "Nd" (_port));
            }
        };

        class Port16Bit : public Port
        {
        public:
            Port16Bit(SundialOS::stdlib::uint16_t portNumber); // purely virtual base class
            ~Port16Bit();
            virtual void Write(SundialOS::stdlib::uint16_t data);
            virtual SundialOS::stdlib::uint16_t Read();

        protected:
            static inline SundialOS::stdlib::uint16_t Read16(SundialOS::stdlib::uint16_t _port)
            {
                SundialOS::stdlib::uint16_t result;
                __asm__ volatile("inw %1, %0" : "=a" (result) : "Nd" (_port));
                return result;
            }

            static inline void Write16(SundialOS::stdlib::uint16_t _port, SundialOS::stdlib::uint16_t _data)
            {
                __asm__ volatile("outw %0, %1" : : "a" (_data), "Nd" (_port));
            }
        };


        class Port32Bit : public Port
        {
        public:
            Port32Bit(SundialOS::stdlib::uint16_t portNumber); // purely virtual base class
            ~Port32Bit();
            virtual void Write(SundialOS::stdlib::uint32_t data);
            virtual SundialOS::stdlib::uint32_t Read();

        protected:
            static inline SundialOS::stdlib::uint32_t Read32(SundialOS::stdlib::uint16_t _port)
            {
                SundialOS::stdlib::uint32_t result;
                __asm__ volatile("inl %1, %0" : "=a" (result) : "Nd" (_port));
                return result;
            }

            static inline void Write32(SundialOS::stdlib::uint16_t _port, SundialOS::stdlib::uint32_t _data)
            {
                __asm__ volatile("outl %0, %1" : : "a" (_data), "Nd" (_port));
            }
        };
    }
}
#endif

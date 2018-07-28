#ifndef __SUNDIALOS_DRIVERS_KEYBOARD_H
#define __SUNDIALOS_DRIVERS_KEYBOARD_H

#include <stdlib/types.h>
#include <hardwarecommunication/interrupts.h>
#include <hardwarecommunication/port.h>
#include <stdlib/stdio.h>
#include <drivers/driver.h>

namespace SundialOS
{
    namespace drivers
    {

        class KeyboardEventHandler
        {
        public:
            KeyboardEventHandler();

            virtual void OnKeyDown(char c);
            virtual void OnKeyUp(char c);
        };

        class KeyboardDriver : public SundialOS::hardwarecommunication::InterruptHandler, public SundialOS::drivers::Driver
        {
            SundialOS::hardwarecommunication::Port8Bit dataport;
            SundialOS::hardwarecommunication::Port8Bit commandport;
            
            KeyboardEventHandler* handler;
        public:
            KeyboardDriver(SundialOS::hardwarecommunication::InterruptManager* manager, KeyboardEventHandler* handler);
            ~KeyboardDriver(); 
            virtual SundialOS::stdlib::uint32_t HandleInterrupt(SundialOS::stdlib::uint32_t esp);
            virtual void Activate();
        };
    }
}
#endif

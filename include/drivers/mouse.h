#ifndef __SUNDIALOS_DRIVERS_MOUSE_H
#define __SUNDIALOS_DRIVERS_MOUSE_H

#include <stdlib/types.h>
#include <hardwarecommunication/interrupts.h>
#include <hardwarecommunication/port.h>
#include <stdlib/stdio.h>
#include <drivers/driver.h>

namespace SundialOS
{
    namespace drivers
    {
        class MouseEventHandler
        {
        public:
            MouseEventHandler();
            
            virtual void OnActivate();
            virtual void OnMouseDown(SundialOS::stdlib::uint8_t button);
            virtual void OnMouseUp(SundialOS::stdlib::uint8_t button);
            virtual void OnMouseMove(SundialOS::stdlib::int8_t x_offset, SundialOS::stdlib::int8_t y_offset);
        };

        class MouseDriver : public SundialOS::hardwarecommunication::InterruptHandler, public SundialOS::drivers::Driver
        {
            SundialOS::hardwarecommunication::Port8Bit dataport;
            SundialOS::hardwarecommunication::Port8Bit commandport;
           
            MouseEventHandler* handler;

            SundialOS::stdlib::uint8_t buffer[3];
            SundialOS::stdlib::uint8_t offset;
            SundialOS::stdlib::uint8_t buttons;

        public:
            MouseDriver(SundialOS::hardwarecommunication::InterruptManager* manager, MouseEventHandler* handler);
            ~MouseDriver(); 
            virtual SundialOS::stdlib::uint32_t HandleInterrupt(SundialOS::stdlib::uint32_t esp);
            virtual void Activate();
        };
    }
}

#endif

#include <drivers/mouse.h>

using namespace SundialOS::stdlib;
using namespace SundialOS::hardwarecommunication;
using namespace SundialOS::drivers;

namespace SundialOS
{
    namespace drivers
    {

        MouseEventHandler::MouseEventHandler()
        {
        }

        void MouseEventHandler::OnActivate()
        {
        }

        void MouseEventHandler::OnMouseUp(uint8_t button)
        {
        }

        void MouseEventHandler::OnMouseDown(uint8_t button)
        {
        }

        void MouseEventHandler::OnMouseMove(int8_t x, int8_t y)
        {
        }

        MouseDriver::MouseDriver(InterruptManager* manager, MouseEventHandler* handler)
            : InterruptHandler(0x2C, manager),
              dataport(0x60),
              commandport(0x64)
        {
            this->handler = handler;
        }

        MouseDriver::~MouseDriver()
        {

        }

        void MouseDriver::Activate()
        {
            offset = 0;
            buttons = 0;
            
            if(handler != 0)
                handler->OnActivate();

            commandport.Write(0xAB); // tells pic to start sending Mouse inputs.
            commandport.Write(0x20); // get current state of pic
            uint8_t status = dataport.Read() | 2;
            commandport.Write(0x60); // set state in pic
            dataport.Write(status);

            commandport.Write(0xD4); 
            dataport.Write(0xF4); // activates the Mouse
            dataport.Read();
        }

        uint32_t MouseDriver::HandleInterrupt(uint32_t esp)
        {
            uint8_t status = commandport.Read();
            if(!(status & 0x20)) // only do work if the 6th bit of the data is 1. 
                return esp;

            buffer[offset] = dataport.Read();
            
            if(handler == 0) // all further work requires a handler 
                return esp;
            
            offset = (offset + 1) % 3;
            
            if(offset == 0) // transmission is through -- yay
            {
                if(buffer[1] != 0 || buffer[2] != 0)
                {
                    handler->OnMouseMove(buffer[1], -buffer[2]);
                }
            
                // states of buttons; 3 buttons (mouse)
                for(uint8_t i = 0; i < 3; i++)
                {
                    // 0x01 << i gives a single byte to work with. 
                    // buffer is the most-recently read-in states; buttons is past state
                    if((buffer[0] & (0x1 << i)) != (buttons & (0x1 << i)))
                    {
                        if(buttons & (0x1 << 1))
                            handler->OnMouseUp(i + 1);
                        else
                            handler->OnMouseDown(i + 1);
                    } 
                }
                buttons = buffer[0];
            }

            return esp;
        }
    }
}

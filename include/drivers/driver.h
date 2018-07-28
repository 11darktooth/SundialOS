#ifndef __SUNDIALOS_DRIVERS_DRIVER_H
#define __SUNDIALOS_DRIVERS_DRIVER_H

#include <stdlib/types.h> 

namespace SundialOS
{
    namespace drivers
    {

        class Driver
        {

        public:
            Driver();
            ~Driver();

            virtual void Activate();
            virtual SundialOS::stdlib::uint32_t Reset();    // good to fix hardware states after initializing due to prior OS cleanup issues. 
            virtual void Deactivate();
        };

        class DriverManager
        {
        private:
            Driver* drivers[256];
            SundialOS::stdlib::uint8_t numDrivers;

        public:
            DriverManager(); 
            void AddDriver(Driver*);
            void ActivateAll();
        };
    }
}
#endif

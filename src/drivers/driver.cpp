#include <drivers/driver.h>

using namespace SundialOS::stdlib;

namespace SundialOS
{
    namespace drivers
    {
        Driver::Driver()
        {
        }

        Driver::~Driver()
        {
        }

        void Driver::Activate()
        {
        }

        uint32_t Driver::Reset()
        {
            return 0;
        }

        void Driver::Deactivate()
        {
        }

        DriverManager::DriverManager()
        {
            numDrivers = 0;
        }

        void DriverManager::AddDriver(Driver* driver)
        {
            drivers[numDrivers] = driver;
            numDrivers++;
        }

        void DriverManager::ActivateAll()
        {
            for(uint8_t i = 0; i < numDrivers; i++)
            {
                drivers[i]->Activate();
            }
        }
    }
}

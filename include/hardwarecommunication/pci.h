#ifndef __SUNDIALOS_HARDWARECOMMUNICATION_PCI_H
#define __SUNDIALOS_HARDWARECOMMUNICATION_PCI_H

#include <hardwarecommunication/port.h>
#include <stdlib/types.h>
#include <hardwarecommunication/interrupts.h>
#include <drivers/driver.h>

namespace SundialOS
{
    namespace hardwarecommunication
    {
        class PeripheralComponentInterconnectDeviceDescriptor
        {
        public:
            SundialOS::stdlib::uint32_t portBase; 
            SundialOS::stdlib::uint32_t interrupt;

            SundialOS::stdlib::uint16_t bus;
            SundialOS::stdlib::uint16_t device;
            SundialOS::stdlib::uint16_t function;

            SundialOS::stdlib::uint16_t vendor_id;
            SundialOS::stdlib::uint16_t device_id;

            SundialOS::stdlib::uint8_t class_id;
            SundialOS::stdlib::uint8_t subclass_id;
            SundialOS::stdlib::uint8_t interface_id;

            SundialOS::stdlib::uint8_t revision;

            PeripheralComponentInterconnectDeviceDescriptor();
            ~PeripheralComponentInterconnectDeviceDescriptor();
        };

        class PeripheralComponentInterconnectController
        {
            SundialOS::hardwarecommunication::Port32Bit dataPort;
            SundialOS::hardwarecommunication::Port32Bit commandPort; 
        public:
            PeripheralComponentInterconnectController();
            ~PeripheralComponentInterconnectController();
            
            SundialOS::stdlib::uint32_t Read
                (SundialOS::stdlib::uint16_t bus, SundialOS::stdlib::uint16_t device,
                 SundialOS::stdlib::uint16_t function, SundialOS::stdlib::uint32_t registeroffset);

            void Write(SundialOS::stdlib::uint16_t bus, SundialOS::stdlib::uint16_t device, 
                    SundialOS::stdlib::uint16_t function, SundialOS::stdlib::uint32_t registeroffset, SundialOS::stdlib::uint32_t value);

            bool DeviceHasFunctions(SundialOS::stdlib::uint16_t bus, SundialOS::stdlib::uint16_t device);

            void SelectDrivers(SundialOS::drivers::DriverManager* driverManager);

            PeripheralComponentInterconnectDeviceDescriptor GetDeviceDescriptor
                (SundialOS::stdlib::uint16_t bus, SundialOS::stdlib::uint16_t device, SundialOS::stdlib::uint16_t function);
        };
    }
}
#endif

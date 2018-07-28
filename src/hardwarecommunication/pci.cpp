#include <hardwarecommunication/pci.h>
#include <stdlib/stdio.h>

using namespace SundialOS::stdlib;
using namespace SundialOS::hardwarecommunication;
using namespace SundialOS::drivers;

PeripheralComponentInterconnectDeviceDescriptor::PeripheralComponentInterconnectDeviceDescriptor()
{
}

PeripheralComponentInterconnectDeviceDescriptor::~PeripheralComponentInterconnectDeviceDescriptor()
{
}

PeripheralComponentInterconnectDeviceDescriptor PeripheralComponentInterconnectController::GetDeviceDescriptor
    (SundialOS::stdlib::uint16_t bus, SundialOS::stdlib::uint16_t device, SundialOS::stdlib::uint16_t function)
{
    PeripheralComponentInterconnectDeviceDescriptor result;
    
    result.bus = bus;
    result.device = device;
    result.function = function;

    result.vendor_id = Read(bus, device, function, 0x00);
    result.device_id = Read(bus, device, function, 0x02);

    result.class_id = Read(bus, device, function, 0x0B);
    result.subclass_id = Read(bus, device, function, 0x0A);
    result.interface_id = Read(bus, device, function, 0x09);

    result.revision = Read(bus, device, function, 0x08);
    result.interrupt = Read(bus, device, function, 0x3C);

    return result;
}

PeripheralComponentInterconnectController::PeripheralComponentInterconnectController()
    : dataPort(0xCFC),
      commandPort(0xCF8)
{

}

PeripheralComponentInterconnectController::~PeripheralComponentInterconnectController()
{
}


uint32_t PeripheralComponentInterconnectController::Read(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset)
{
    uint32_t id =
        0x1 << 31 // sets the first bit explicitly to 1
        | ((bus & 0xFF) << 16)
        | ((device & 0x1F) << 11)
        | ((function & 0x07) << 8)
        | (registeroffset & 0xFC); // 0xFC cuts off the last two digits of the number. registeroffset is 4 byte aligned. 
    // this is primarily due to data being received in 4 byte packages and we want byte 6.

    commandPort.Write(id); // probe for device
    uint32_t result = dataPort.Read();
    return result >> (8 * (registeroffset % 4));
}

void PeripheralComponentInterconnectController::Write
    (uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset, uint32_t value)
{
    uint32_t id =
        0x1 << 31 // sets the first bit explicitly to 1
        | ((bus & 0xFF) << 16)
        | ((device & 0x1F) << 11)
        | ((function & 0x07) << 8)
        | (registeroffset & 0xFC);
    commandPort.Write(id);
    dataPort.Write(value);
}

bool PeripheralComponentInterconnectController::DeviceHasFunctions(uint16_t bus, uint16_t device)
{
    // reading 0x0E on function 0 determines if there're any functions. If so, then we'll need to enumerate. 
    return Read(bus, device, 0, 0x0E) & (1 << 7); // only need the 7th bit.
}

// TODO store this into an array and increase speeds.
void PeripheralComponentInterconnectController::SelectDrivers(DriverManager* driverManager) 
{
   // not a fast method to handle this
   for(uint8_t bus = 0; bus < 8; bus++)
   {
       for(uint8_t device = 0; device < 32; device++)
       {
           uint8_t numFunctions = DeviceHasFunctions(bus, device) ? 8 : 1; // DeviceHasFunctions speeds up function by factor of 8.
           for(uint8_t function = 0; function < numFunctions; function++)
           {
               PeripheralComponentInterconnectDeviceDescriptor descriptor = GetDeviceDescriptor(bus, device, function);
               if(descriptor.vendor_id == 0x0000 || descriptor.vendor_id == 0xFFFF)
                   break; // no more functions
               printf("PCI BUS ");
               printfHex(bus & 0xFF);

               printf(", DEVICE ");
               printfHex(device & 0xFF);

               printf(", FUNCTION ");
               printfHex(function & 0xFF);

               printf(" = VENDOR ");
               printfHex((descriptor.vendor_id & 0xFF) >> 8);
               printfHex(descriptor.vendor_id & 0xFF);

               printf(", DEVICE ");
               printfHex((descriptor.device_id & 0xFF) >> 8);
               printfHex(descriptor.device_id & 0xFF);

               printf("\n");

           }
       }
   }
}

#ifndef __SUNDIALOS_HARDWARECOMMUNICATION_INTERRUPTS_H 
#define __SUNDIALOS_HARDWARECOMMUNICATION_INTERRUPTS_H 

#include <gdt.h>
#include <stdlib/types.h>
#include <hardwarecommunication/port.h>
#include <stdlib/stdio.h>

namespace SundialOS
{
    namespace hardwarecommunication
    {

        class InterruptManager;

        class InterruptHandler
        {
            protected:
                SundialOS::stdlib::uint8_t interruptNumber;
                InterruptManager* interruptManager;

                InterruptHandler(SundialOS::stdlib::uint8_t interruptNumber, InterruptManager* interruptManager);
                ~InterruptHandler();
            public:
                virtual SundialOS::stdlib::uint32_t HandleInterrupt(SundialOS::stdlib::uint32_t esp);
        };

        class InterruptManager
        {
            friend class InterruptHandler;
            protected:
                
                static InterruptManager* ActiveInterruptManager;
                InterruptHandler* handlers[256];

                struct GateDescriptor
                {
                    SundialOS::stdlib::uint16_t handlerAddressLowBits;
                    SundialOS::stdlib::uint16_t gdt_codeSegmentSelector;
                    SundialOS::stdlib::uint8_t reserved;
                    SundialOS::stdlib::uint8_t access;
                    SundialOS::stdlib::uint16_t handlerAddressHighBits;
                } __attribute__((packed));
                
                static GateDescriptor interruptDescriptorTable[256];

                struct InterruptDescriptorTablePointer
                {
                    SundialOS::stdlib::uint16_t size;
                    SundialOS::stdlib::uint32_t base;
                } __attribute__((packed));

                static void SetInterruptDescriptorTableEntry(
                    SundialOS::stdlib::uint8_t interruptNumber,
                    SundialOS::stdlib::uint16_t codeSegmentSelectorOffset,
                    void (*handler)(),
                    SundialOS::stdlib::uint8_t DescriptorPrivilegeLevel,
                    SundialOS::stdlib::uint8_t DescriptorType
                );
               
                static void InterruptIgnore();

                static void HandleInterruptRequest0x00(); // Timer
                static void HandleInterruptRequest0x01(); // Keyboard
                static void HandleInterruptRequest0x0C(); // Mouse
                
                SundialOS::stdlib::uint32_t DoHandleInterrupt(SundialOS::stdlib::uint8_t interruptNumber, SundialOS::stdlib::uint32_t esp);
                static SundialOS::stdlib::uint32_t HandleInterrupt(SundialOS::stdlib::uint8_t interruptNumber, SundialOS::stdlib::uint32_t esp);

                SundialOS::hardwarecommunication::Port8BitSlow picMasterCommand; // two PICs; master and slave; need to communicate with both
                SundialOS::hardwarecommunication::Port8BitSlow picMasterData;

                SundialOS::hardwarecommunication::Port8BitSlow picSlaveCommand;
                SundialOS::hardwarecommunication::Port8BitSlow picSlaveData;

            public:
                InterruptManager(GlobalDescriptorTable* gdt);
                ~InterruptManager();

                void Activate();
                void Deactivate();
        };
    }
}
#endif

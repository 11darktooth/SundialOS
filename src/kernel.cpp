#include <stdlib/stdio.h>
#include <stdlib/types.h>
#include <gdt.h>
#include <drivers/driver.h>
#include <hardwarecommunication/interrupts.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <hardwarecommunication/pci.h>

using namespace SundialOS;
using namespace SundialOS::drivers;
using namespace SundialOS::hardwarecommunication;
using namespace SundialOS::stdlib;

typedef void (*constructor)(); // defined what constructor is 
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;

extern "C" void callConstructors()
{
    for(constructor* i = &start_ctors; i != &end_ctors; i++)
    {
        (*i)();
    }
}

class PrintfKeyboardEventHandler : public ::KeyboardEventHandler
{
    public:
        void OnKeyDown(char c)
        {
            char* foo = " ";
            foo[0] = c;
            printf(foo);
        }
};

class MouseToTerminal : public ::MouseEventHandler
{
    int8_t x, y;
public:
    MouseToTerminal()
    {
    }
    
    virtual void OnActivate()
    {
        uint16_t* VideoMemory = (uint16_t*)0xb8000;
        x = 40;
        y = 12;
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);
    }

    virtual void OnMouseMove(int8_t xoffset, int8_t yoffset)
    {
        static uint16_t* VideoMemory = (uint16_t*)0xb8000;

        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);
       
        x += xoffset;
        if(x >= 80) x = 79;
        if(x < 0) x = 0;
        
        y += yoffset;
        if(y >= 25) y = 24;
        if(y < 0) y = 0;        
        
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);
    }
};

extern "C" void kernelMain(void* multiboot_msgucture, uint32_t magicnumber)
{
    printf("SundialOS!\n");
    printf("Version: 0.0.1\n");
  
    // ========== STAGE 1 ==========
    printf("Initializing GDT and IDT...");
    
    GlobalDescriptorTable gdt; 
    InterruptManager interrupts(&gdt);
    
    printf("done\n");
    // ========== END OF STAGE 1 =========
    
    // ========== STAGE 2 =========
    DriverManager driverManager;

    printf("Initializing keyboard driver...");

    PrintfKeyboardEventHandler kbhandler;
    KeyboardDriver keyboard(&interrupts, &kbhandler);
    driverManager.AddDriver(&keyboard);

    printf("done\n");

    printf("Initializing mouse driver...");
    MouseToTerminal mousehandler;
    MouseDriver mouse(&interrupts, &mousehandler);
    driverManager.AddDriver(&mouse);
    printf("done\n");
   
    printf("Initializing PCI Controller...\n");
    PeripheralComponentInterconnectController PCIController;
    PCIController.SelectDrivers(&driverManager);
    printf("done\n");

    printf("Activating all drivers...");
    driverManager.ActivateAll();
    printf("done\n");
    // ========== END OF STAGE 2 =========
    
    // ========== STAGE 3 =========
    printf("Activating interrupts...");
    interrupts.Activate();
    printf("done\n");
    // ========== END OF STAGE 3 ========== 
    while (1);
}

#ifndef __GDT_H
#define __GDT_H

#include <stdlib/types.h>

namespace SundialOS
{
    class GlobalDescriptorTable
    {
    public:
        class SegmentDescriptor
        {
        private:
            SundialOS::stdlib::uint16_t limit_low;
            SundialOS::stdlib::uint16_t base_low;
            SundialOS::stdlib::uint8_t  base_high;
            SundialOS::stdlib::uint8_t type; 
            SundialOS::stdlib::uint8_t flags_limit_high;
            SundialOS::stdlib::uint8_t base_vhigh;
        public:
            SegmentDescriptor(SundialOS::stdlib::uint32_t base, SundialOS::stdlib::uint32_t limit, SundialOS::stdlib::uint8_t type);
            SundialOS::stdlib::uint32_t Base();
            SundialOS::stdlib::uint32_t Limit();
        } __attribute__((packed)); // prevents compiler from moving this class around.
        
    private:
        SegmentDescriptor nullSegmentSelector;
        SegmentDescriptor unusedSegmentSelector;
        SegmentDescriptor codeSegmentSelector;
        SegmentDescriptor dataSegmentSelector;

    public:
        GlobalDescriptorTable();
        ~GlobalDescriptorTable();

        SundialOS::stdlib::uint16_t CodeSegmentSelector();
        SundialOS::stdlib::uint16_t DataSegmentSelector();
    };
}
#endif

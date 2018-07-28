# _ZN9SundialOS21hardwarecommunication16InterruptManager
.set IRQ_BASE, 0x20

.section .text

.extern _ZN9SundialOS21hardwarecommunication16InterruptManager15HandleInterruptEht

.macro HandleException num
.global _ZN9SundialOS21hardwarecommunication16InterruptManager19HandleException\num\()Ev
_ZN9SundialOS21hardwarecommunication16InterruptManager19HandleException\num\()Ev:
    movb $\num, (interruptnumber)
    jmp int_bottom
.endm

.macro HandleInterruptRequest num
.global _ZN9SundialOS21hardwarecommunication16InterruptManager26HandleInterruptRequest\num\()Ev
_ZN9SundialOS21hardwarecommunication16InterruptManager26HandleInterruptRequest\num\()Ev:
    movb $\num + IRQ_BASE, (interruptnumber)
    jmp int_bottom
.endm

HandleInterruptRequest 0x00 # Hardware Timer
HandleInterruptRequest 0x01 # Keyboard
HandleInterruptRequest 0x0C # Mouse

int_bottom:

    pusha
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs

    pushl %esp
    push (interruptnumber)
    call _ZN9SundialOS21hardwarecommunication16InterruptManager15HandleInterruptEhj

    #movl %eax, %esp # only works since we're returning the ESP in functions
    
    add %esp, 6
    mov %eax, %esp #clean up stack pointer

    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa

.global _ZN9SundialOS21hardwarecommunication16InterruptManager15InterruptIgnoreEv 
_ZN9SundialOS21hardwarecommunication16InterruptManager15InterruptIgnoreEv:
    iret

.data
    interruptnumber: .byte 0

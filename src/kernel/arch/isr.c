#include <common.h>
#include <arch/isr.h>
#include <arch/ports.h>
#include <arch/descriptor_tables.h>

isr_t interrupt_handlers[32];

/* To print the message which defines every exception */
char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",
    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"};

void register_interrupt_handler(uint8_t n, isr_t handler)
{
    interrupt_handlers[n] = handler;
}

// This gets called from our ASM interrupt handler stub.
void isr_handler(registers_t *regs)
{
    u8int int_no = regs->int_no & 0xFF;
    if (interrupt_handlers[int_no] != 0)
    {
        isr_t handler = interrupt_handlers[int_no];
        handler(regs);
    }
    else
    {
        set_terminal_color(VGA_COLOR_RED, VGA_COLOR_LIGHT_GREY);
        terminal_write("\n");
        terminal_write("### UNHANDLED EXCEPTION ###");
        terminal_write("\n");
        terminal_write(exception_messages[int_no]);
        terminal_write(" (");
        terminal_write_hex(int_no);
        terminal_write(")\n");
        reset_terminal_color();
        asm volatile("hlt");
        for (;;)
            ;
    }
}

// This gets called from our ASM interrupt handler stub.
void irq_handler(registers_t *regs)
{
    // Send an EOI (end of interrupt) signal to the PICs.
    // If this interrupt involved the slave.
    if (regs->int_no >= 40)
    {
        // Send reset signal to slave.
        port_byte_out(0xA0, 0x20);
    }

    // Send reset signal to master. (As well as slave, if necessary).
    port_byte_out(0x20, 0x20);

    if (interrupt_handlers[regs->int_no] != 0)
    {
        isr_t handler = interrupt_handlers[regs->int_no];
        handler(regs);
    }
}

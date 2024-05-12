#include "idt.h"

uint16_t get_cs(void);

void idt_set_descriptor(IDT idt, uint8_t vector, void* isr, uint8_t flags) {
    IDT_Entry *descriptor = &idt[vector];
 
    descriptor->isr_low        = (uint64_t)isr & 0xFFFF;
    descriptor->kernel_cs      = get_cs(); // FIXME
    descriptor->ist            = 0;
    descriptor->attributes     = flags;
    descriptor->isr_mid        = ((uint64_t)isr >> 16) & 0xFFFF;
    descriptor->isr_high       = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
    descriptor->reserved       = 0;
}

extern void *isr_stub_table[];

static IDTR idtr;

void idt_init(IDT idt) {
    idtr = (IDTR) {
        .base = (uintptr_t)idt,
        .limit = (uint16_t)sizeof(IDT_Entry) * IDT_MAX_DESCRIPTORS - 1,
    };

    for (uint8_t vector = 0; vector < 32; vector++) {
        idt_set_descriptor(idt, vector, isr_stub_table[vector], 0x8E);
        //vectors[vector] = true;
    }
 
    __asm__ volatile ("lidt %0" : : "m"(idtr)); // load the new IDT
    __asm__ volatile ("sti"); // set the interrupt flag
}

#include "serial.h"
#include "io.h"

#define PORT 0x3f8          // COM1
 
bool serial_init(void) {
   out8(PORT + 1, 0x00);    // Disable all interrupts
   out8(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
   out8(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
   out8(PORT + 1, 0x00);    //                  (hi byte)
   out8(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
   out8(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   out8(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
   out8(PORT + 4, 0x1E);    // Set in loopback mode, test the serial chip
   out8(PORT + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)
 
   // Check if serial is faulty (i.e: not same byte as sent)
   if (in8(PORT + 0) != 0xAE) {
      return false;
   }
 
   // If serial is not faulty set it in normal operation mode
   // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
   out8(PORT + 4, 0x0F);
   return true;
}

int serial_transmit_empty(void) {
   return in8(PORT + 5) & 0x20;
}
 
void serial_write_char(char a) {
   while (!serial_transmit_empty());
 
   out8(PORT, a);
}

void serial_print(const char *text, size_t len) {
    for (size_t i = 0; i < len; i++) {
        serial_write_char(text[i]);
    }
}


#ifndef SERIAL_H
#define SERIAL_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

bool serial_init(void);
int serial_transmit_empty(void);
void serial_write_char(char a);
void serial_print(const char *data, size_t len);

#endif // SERIAL_H

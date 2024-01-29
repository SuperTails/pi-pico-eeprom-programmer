#pragma once

#include <stdint.h>

void eeprom_init();

#define PAGE_SIZE 64

// Writes a page of data to the EEPROM, starting at (page-aligned) address `base`.
void program(const uint8_t data[PAGE_SIZE], uint16_t base);

// Reads `count`-many bytes into `buf`, starting at EEPROM address `base`.
void readback(uint8_t buf[], uint16_t base, uint16_t count);
#include "eeprom.h"

#include <Arduino.h>

const int web_pin = 21;
const int oeb_pin = 20;

/*const int address_pin_map[][2] = {
  {  0, 12 },
  {  1,  7 },
  {  2,  6 },
  {  3,  5 },
  {  4,  4 },
  {  5,  3 },
  {  6,  2 },
  {  7,  1 },
  {  8,  0 },
  { 28,  8 },
  { 27,  9 },
  { 26, 11 },
  { 22, 10 },
}*/

#define ADDR_PIN_COUNT 13

const int address_pin_map[ADDR_PIN_COUNT][2] = {
  {  8,  0 },
  {  7,  1 },
  {  6,  2 },
  {  5,  3 },
  {  4,  4 },
  {  3,  5 },
  {  2,  6 },
  {  1,  7 },
  { 28,  8 },
  { 27,  9 },
  { 22, 10 },
  { 26, 11 },
  {  0, 12 }
};

const int data_pin_map[] = {
  16,
  17,
  18,
  19,
  13,
  12,
  11,
  10
};

void put_addr(uint16_t addr) {
  for (int i = 0; i < ADDR_PIN_COUNT; ++i) {
    digitalWrite(address_pin_map[i][0], (addr >> i) & 0x1);
  }
}

void put_data(uint8_t data) {
  for (int i = 0; i < 8; ++i) {
    digitalWrite(data_pin_map[i], (data >> i) & 0x1);
  }
}

uint8_t get_data() {
  uint8_t result = 0;
  for (int i = 0; i < 8; ++i) {
    result |= digitalRead(data_pin_map[i]) << i;
  }
  return result;
}

enum class DataDir {
  Program,
  Readback,
};

void set_data_dir(DataDir dir) {
  if (dir == DataDir::Program) {
    digitalWrite(oeb_pin, HIGH);
    for (int i = 0; i < 8; ++i) {
      pinMode(data_pin_map[i], OUTPUT);
    }
  } else {
    for (int i = 0; i < 8; ++i) {
      pinMode(data_pin_map[i], INPUT);
    }
    digitalWrite(oeb_pin, LOW);
  }
}

void eeprom_init() {
  for (int i = 0; i < ADDR_PIN_COUNT; ++i) {
    pinMode(address_pin_map[i][0], OUTPUT);
  }

  for (int i = 0; i < 8; ++i) {
    pinMode(data_pin_map[i], INPUT);
  }

  delay(10);

  sio_hw->gpio_out |= (1 << web_pin) | (1 << oeb_pin);
  sio_hw->gpio_oe |= (1 << web_pin) | (1 << oeb_pin);
}

void readback(uint8_t buf[], uint16_t base, uint16_t count) {
  set_data_dir(DataDir::Readback);

  for (uint32_t i = 0; i < count; ++i) {
    put_addr(base + i);
    delayMicroseconds(5);
    uint8_t data = get_data();

    buf[i] = data;
  }
}

void program(const uint8_t data[PAGE_SIZE], uint16_t base) {
  set_data_dir(DataDir::Program);

  put_addr(0x1555);
  put_data(0xAA);

  digitalWrite(web_pin, LOW);
  delayMicroseconds(10);
  digitalWrite(web_pin, HIGH);
  delayMicroseconds(10);

  put_addr(0x0AAA);
  put_data(0x55);

  digitalWrite(web_pin, LOW);
  delayMicroseconds(10);
  digitalWrite(web_pin, HIGH);
  delayMicroseconds(10);

  put_addr(0x1555);
  put_data(0xA0);

  digitalWrite(web_pin, LOW);
  delayMicroseconds(10);
  digitalWrite(web_pin, HIGH);
  delayMicroseconds(10);

  for (int i = 0; i < PAGE_SIZE; ++i) {
    put_addr(base + i);
    put_data(data[i]);

    digitalWrite(web_pin, LOW);
    delayMicroseconds(10);
    digitalWrite(web_pin, HIGH);
    delayMicroseconds(10);
  }
}

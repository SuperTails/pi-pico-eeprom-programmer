#include "eeprom.h"
#include "message.h"
#include "crc.h"

#include <Arduino.h>

void setup() {
  Serial.begin(115200);

  eeprom_init();
}

void loop() {
  Command cmd;
  int cmd_result = get_command(cmd);
  if (cmd_result == 0) {
    switch (cmd.ty) {
    case CommandType::Ping:
      Serial.print("Pong!");
      break;
    case CommandType::Write:
      program(cmd.write_eeprom.data, cmd.write_eeprom.addr);
      break;
    case CommandType::Read: {
      uint8_t buf[64];
      readback(buf, cmd.read_eeprom.addr, cmd.read_eeprom.len);

      Serial.write(buf, cmd.read_eeprom.len);

      uint16_t crc = update_crc(0, buf, cmd.read_eeprom.len);
      Serial.write(reinterpret_cast<uint8_t*>(&crc), 2);
    }

    }
  }

  /*char fmt_buffer[256];

  while (1) {
    uint8_t data[32];
    for (uint16_t i = 0x0000; i < 0x2000; i += 32) {
      readback(data, i, 32);

      size_t fmt_len = snprintf(fmt_buffer, sizeof(fmt_buffer), "%04x: ", (int)i);
      Serial.write(fmt_buffer, fmt_len);

      for (int j = 0; j < 32; ++j) {
        size_t fmt_len = snprintf(fmt_buffer, sizeof(fmt_buffer), "%02x ", (int)data[j]);
        Serial.write(fmt_buffer, fmt_len);
      }

      Serial.println();

      delay(10);
    }
  }*/
}
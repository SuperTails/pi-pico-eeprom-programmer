#include <Arduino.h>

#include "message.h"
#include "crc.h"

int get_command(Command &cmd) {
	int msg_type = Serial.read(); 
	if (msg_type == -1) {
		return ERR_NO_CMD;
	}

	if (msg_type == 0 || msg_type >= static_cast<int>(CommandType::MAX_CMD)) {
		return ERR_BAD_CMD;
	}

	auto cmd_type = static_cast<CommandType>(msg_type);
	cmd.ty = cmd_type;

	// Echo back the message type to tell the host to send the rest of the command
	Serial.write((uint8_t)msg_type);

	switch (cmd_type) {
	case CommandType::Ping:
		break;
	case CommandType::Write: {
		int num_read = Serial.readBytes(reinterpret_cast<uint8_t*>(&cmd.write_eeprom), sizeof(WriteCmd));
		if (num_read != sizeof(WriteCmd)) {
			return ERR_TIMEOUT;
		}
		uint16_t rx_crc = cmd.write_eeprom.checksum;
		uint16_t calc_crc = update_crc(0, reinterpret_cast<uint8_t*>(&cmd.write_eeprom), 2 + EEPROM_PAGE_SIZE);
		if (rx_crc != calc_crc) {
			Serial.write((uint8_t)ERR_BAD_CHECKSUM);
			return ERR_BAD_CHECKSUM;
		}
		if (cmd.write_eeprom.addr & EEPROM_PAGE_MASK) {
			Serial.write((uint8_t)ERR_BAD_ADDR);
			return ERR_BAD_ADDR;
		}

		Serial.write((uint8_t)0);
		break;
	}
	case CommandType::Read: {
		int num_read = Serial.readBytes(reinterpret_cast<uint8_t*>(&cmd.read_eeprom), 2 * sizeof(uint16_t));
		if (num_read != 2 * sizeof(uint16_t)) {
			return ERR_TIMEOUT;
		}
		if (cmd.read_eeprom.len > 64) {
			return ERR_BAD_LEN;
		}
		break;
	}
	case CommandType::PrintInfo:
		break;
	case CommandType::MAX_CMD:
		break;
	}

	return 0;
}
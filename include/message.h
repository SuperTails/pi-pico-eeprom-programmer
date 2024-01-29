#pragma once

#include <stdint.h>

enum class CommandType : uint8_t {
	Ping = 0x1,
	Write = 0x2,
	Read = 0x3,
	PrintInfo = 0x4,
	MAX_CMD
};

#define EEPROM_PAGE_BITS 6
#define EEPROM_PAGE_SIZE (1 << EEPROM_PAGE_BITS)
#define EEPROM_PAGE_MASK (EEPROM_PAGE_SIZE - 1)

struct WriteCmd {
	uint16_t addr;
	uint8_t data[EEPROM_PAGE_SIZE];
	uint16_t checksum;
};

struct ReadCmd {
	uint16_t addr;
	uint16_t len;
};

struct Command {
	CommandType ty;
	union {
		WriteCmd write_eeprom;
		ReadCmd read_eeprom;
	};
};

#define ERR_NO_CMD -1
#define ERR_BAD_CMD -2
#define ERR_BAD_ADDR -3
#define ERR_BAD_LEN -4
#define ERR_BAD_CHECKSUM -5
#define ERR_TIMEOUT -6

// Returns 0 on success and stores the command data in `cmd`.
// Returns ERR_NO_CMD if no command has been sent yet.
int get_command(Command &cmd);
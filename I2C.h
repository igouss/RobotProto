#ifndef I2C_H_
#define I2C_H_

#include <WConstants.h>

namespace naxsoft {
	class I2C {
	public:
		static void sendByte(int deviceAddress, uint8_t dataValue);
		static void updateRegister(int deviceAddress, uint8_t dataAddress, uint8_t dataValue);
		static void writePage(int deviceAddress, unsigned int addresspage, byte* data, byte length);
		static byte readByte(int deviceAddress);
		static int readRegister(int deviceAddress, byte address);

		static int readWord(int deviceAddress);
		static int readReverseWord(int deviceAddress);
		static byte readWho(int deviceAddress);
		static byte eeprom_read_byte(int deviceAddress, unsigned int address);
		static void eeprom_read_buffer(int deviceAddress, unsigned int address,	byte *buffer, int length);
	};
}
#endif /* I2C_H_ */

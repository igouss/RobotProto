#ifndef I2C_H_
#define I2C_H_

#include <WConstants.h>

namespace naxsoft {
	class I2C {
	public:
		void sendByte(int deviceAddress, byte dataValue);
		void updateRegister(int deviceAddress, byte dataAddress, byte dataValue);
		void writePage(int deviceAddress, unsigned int addresspage, byte* data, byte length);
		byte readByte(int deviceAddress);
		int readWord(int deviceAddress);
		int readReverseWord(int deviceAddress);
		byte readWho(int deviceAddress);
		byte eeprom_read_byte(int deviceAddress, unsigned int address);
		void eeprom_read_buffer(int deviceAddress, unsigned int address,
				byte *buffer, int length);
	};
}
#endif /* I2C_H_ */

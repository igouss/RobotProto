#include <WProgram.h>
#include "XBeeATTransport.h"

#define SET_PORTD_PIN(pinNr, x) (PORTD = ((PORTD & (0xFF ^ (1<<(pinNr)))) | ((x) << (pinNr))))
#define GET_PORTD_PIN(pinNr) ((PIND >> (pinNr)) & 0x01)

namespace naxsoft {
void XBeeATTransport::open(void) {
	Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
	m_isOpen = true;

	pinMode(CTS_PIN, OUTPUT);
	digitalWrite(CTS_PIN, 0);

}

void XBeeATTransport::close(void) {
	Serial.end();
	m_isOpen = false;
}

bool XBeeATTransport::isOpen(void) const {
	return m_isOpen;
}

void XBeeATTransport::flush() const {
}

bool XBeeATTransport::peek() const {
	return Serial.available() > 0;
}

uint32_t XBeeATTransport::read(uint8_t* buf, uint32_t len) const {
	uint32_t i = 0;

//	if (Serial.available() >= 32) {
//		digitalWrite(CTS_PIN, 1);
//	} else {
//		digitalWrite(CTS_PIN, 0);
//	}

	while (Serial.available() && i < len) {
		buf[i] = Serial.read();
		i++;
	}
	return i;
}

uint32_t XBeeATTransport::readAll(uint8_t* buf, uint32_t len) const {
	uint32_t have = 0;
	uint32_t get = 0;

	while (have < len) {
		get = read(buf + have, len - have);
		have += get;
	}
	return have;
}

void XBeeATTransport::write(const uint8_t* buf, uint32_t len) const {
	Serial.write(buf, len);
}
} // namespace

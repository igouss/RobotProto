#include <Wire.h>
#include "Compass.h"

// The Arduino two-wire interface uses a 7-bit number for the address,
// and sets the last bit correctly based on reads and writes
#define MAG_ADDRESS 		(0x3C >> 1)

#define CRA_REG_M  			(0x00)
#define CRB_REG_M  			(0x01)
#define MR_REG_M  			(0x02)

#define OUT_X_H_M  			(0x03)
#define OUT_X_L_M  			(0x04)
#define OUT_Y_H_M  			(0x05)
#define OUT_Y_L_M  			(0x06)
#define OUT_Z_H_M  			(0x07)
#define OUT_Z_L_M  			(0x08)

#define SR_REG_M   			(0x09)
#define IRA_REG_M   		(0x0A)
#define IRB_REG_M   		(0x0B)
#define IRC_REG_M   		(0x0C)

namespace naxsoft {

Compass compass; // preinstatiate

void Compass::init() {
	//Enable Magnetometer
	Wire.beginTransmission(MAG_ADDRESS);
	Wire.send(MR_REG_M);
	//0x00 = 0b00000000
	// Continuous conversion mode
	Wire.send(0x00);
	Wire.endTransmission();
}

void Compass::read(compass_data* data) {
	//read magnetometer
	Wire.beginTransmission(MAG_ADDRESS);
	Wire.send(OUT_X_H_M);
	Wire.endTransmission();
	Wire.requestFrom(MAG_ADDRESS, 6);

	while (Wire.available() < 6) {
	}

	uint8_t xhm = Wire.receive();
	uint8_t xlm = Wire.receive();
	uint8_t yhm = Wire.receive();
	uint8_t ylm = Wire.receive();
	uint8_t zhm = Wire.receive();
	uint8_t zlm = Wire.receive();

	data->x = (xhm << 8 | xlm);
	data->y = (yhm << 8 | ylm);
	data->z = (zhm << 8 | zlm);
}

} // namespace

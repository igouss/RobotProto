#include <Wire.h>
#include "Accelerometer.h"

#include "../I2C.h"

// The Arduino two-wire interface uses a 7-bit number for the address,
// and sets the last bit correctly based on reads and writes
#define ACC_ADDRESS 		(0x30 >> 1)
#define CTRL_REG1_A 		(0x20)
#define CTRL_REG2_A 		(0x21)
#define CTRL_REG3_A 		(0x22)
#define CTRL_REG4_A			(0x23)
#define CTRL_REG5_A 		(0x24)
#define HP_FILTER_RESET_A 	(0x25)
#define REFERENCE_A 		(0x26)
#define STATUS_REG_A 		(0x27)

#define OUT_X_L_A  			(0x28)
#define OUT_X_H_A  			(0x29)
#define OUT_Y_L_A  			(0x2A)
#define OUT_Y_H_A  			(0x2B)
#define OUT_Z_L_A  			(0x2C)
#define OUT_Z_H_A  			(0x2D)

#define INT1_CFG_A  		(0x30)
#define INT1_SOURCE_A  		(0x31)
#define INT1_THS_A  		(0x32)
#define INT1_DURATION_A  	(0x33)
#define INT2_CFG_A  		(0x34)
#define INT2_SOURCE_A  		(0x35)
#define INT2_THS_A  		(0x36)
#define INT2_DURATION_A  	(0x37)

namespace naxsoft {

Accelerometer accelerometer; // preinstatiate

void Accelerometer::init() {
	//Enable Accelerometer
	// 0x27 = 0b00100111 => normal power mode, 50 Hz data rate, all axes enabled
	I2C::updateRegister(ACC_ADDRESS, CTRL_REG1_A, 0x27);

	// keep a full scale range ±2 gauss in continuous data update
	// mode and change the little-endian to a big-endian structure
	I2C::updateRegister(ACC_ADDRESS, CTRL_REG4_A, 0x40);
}

void Accelerometer::read(accelerometer_data* data) {
	//read accelerometer
	// assert the MSB of the address to get the accelerometer
	// to do slave-transmit subaddress updating.
	I2C::sendByte(ACC_ADDRESS, OUT_X_L_A | (1 << 7));

	Wire.requestFrom(ACC_ADDRESS, 6);

	while (Wire.available() < 6) {
	}

	uint8_t xha = Wire.receive(); //read OUT_X_H_A (MSB)
	uint8_t xla = Wire.receive(); //read OUT_X_L_A (LSB)
	uint8_t yha = Wire.receive(); //read OUT_Y_H_A (MSB)
	uint8_t yla = Wire.receive(); //read OUT_Y_L_A (LSB)
	uint8_t zha = Wire.receive(); //read OUT_Z_H_A (MSB)
	uint8_t zla = Wire.receive(); //read OUT_Z_L_A (LSB)

	data->x = 0;
	data->y = 0;
	data->z = 0;

	data->x = -(xha << 8 | xla); // nagate because it is mounted upside down
	data->y = -(yha << 8 | yla); // nagate because it is mounted upside down
	data->z = -(zha << 8 | zla); // nagate because it is mounted upside down
}

} // namespace

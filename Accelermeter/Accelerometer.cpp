#include "Accelerometer.h"

#include "../I2C/I2CProtocol.h"
#include "../bitops.h"

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
	// 0x27 = 0b0010 0111 => normal power mode, 50 Hz data rate, all axes enabled
	// 0x27 = 0b0011 1111 => normal power mode, 1000 Hz data rate, all axes enabled
	//          _        | PM2
	//           _       | PM1
	//            _      | PM0
	//             _ _   | DR1 & DR0 ==> 00->50Hz, 01->100Hz, 10->400Hz, 11->1000Hz
	//                ___| Enable x,y,z

	i2cProtocol.write(ACC_ADDRESS, CTRL_REG1_A, 0x3F);

	// keep a full scale range ±2 gauss in continuous data update
	// mode and change the little-endian to a big-endian structure
	// 0x1000 0000
	//   _          | BDU
	//    _         | BLE
	//     __       | FS1 FS0
	//        _0_0  | STsign ST
	// F0 -> output registers are not updates between MSB and LSB reading, MSB first
	//       ±8 gauss
	i2cProtocol.write(ACC_ADDRESS, CTRL_REG4_A, 0xF0);
}

bool Accelerometer::isDataAvailable() {
	i2cProtocol.read(ACC_ADDRESS, STATUS_REG_A, 1);
	uint8_t reg = i2cProtocol.receive();
	return BITSSET(reg, BIT(3)); // ZYXDA
}

void Accelerometer::read(accelerometer_data* data) {
	//	1. Read STATUS_REG
	//	2. If STATUS_REG(3) = 0 then go to 1
	//	3. If STATUS_REG(7) = 1 then some data have been overwritten
	//	4. read XYZ
	while (!isDataAvailable()) {
		delayMicroseconds(10);
	}



	// assert the MSB of the address to get the accelerometer
	// to do slave-transmit subaddress updating.
	//read accelerometer
	i2cProtocol.read(ACC_ADDRESS, OUT_X_L_A | (1 << 7), 6);

	while(i2cProtocol.available() < 6) {

	}

	uint8_t xha = i2cProtocol.receive(); //read OUT_X_H_A (MSB)
	uint8_t xla = i2cProtocol.receive(); //read OUT_X_L_A (LSB)
	uint8_t yha = i2cProtocol.receive(); //read OUT_Y_H_A (MSB)
	uint8_t yla = i2cProtocol.receive(); //read OUT_Y_L_A (LSB)
	uint8_t zha = i2cProtocol.receive(); //read OUT_Z_H_A (MSB)
	uint8_t zla = i2cProtocol.receive(); //read OUT_Z_L_A (LSB)

	data->x = 0;
	data->y = 0;
	data->z = 0;

	// 12 bit sensitivity
	// FS = 00->1  mg/digi
	// FS = 01->2  mg/digi
	// FS = 11->3.9mg/digi
	data->x = (xha << 8 | xla); // nagate because it is mounted upside down
	data->y = (yha << 8 | yla); // nagate because it is mounted upside down
	data->z = (zha << 8 | zla); // nagate because it is mounted upside down
}

} // namespace

#include <math.h>
#include "Compass.h"

#include "../I2C/I2CProtocol.h"
#include "../bitops.h"

/*
 * The strength of the earth's magnetic field is about 0.5 to 0.6 gauss and has a component
 * parallel to the earth's surface that always points toward the magnetic north pole. In the
 * northern hemisphere, this field points down. This angle between the earth’s magnetic field and the
 * horizontal plane is defined as an inclination angle. Another angle between the earth's
 * magnetic north and geographic north is defined as a declination angle in the range of ± 20º
 * depending on the geographic location.
 *
 * A tilt compensated electronic compass system requires a 3-axis magnetic sensor and a 3-
 * axis accelerometer sensor. The accelerometer is used to measure the tilt angles of pitch
 * and roll for tilt compensation. And the magnetic sensor is used to measure the earth’s
 * magnetic field and then to determine the heading angle with respect to the magnetic north.
 * If the heading with respect to the geographic north is required, the declination angle at the
 * current geographic location should be compensated to the magnetic heading.
 *
 * Definitions:
 *
 * Heading is defined as the angle between the Xb axis and the magnetic north on the
 * horizontal plane measured in a clockwise direction when viewing from the top of the device
 * (or aircraft).
 *
 * Pitch is defined as the angle between the Xb axis and the horizontal plane. When rotating
 * the device around the Yb axis with the Xb axis moving upwards, pitch is positive and increasing.
 *
 * Roll is defined as the angle between the Yb axis and the horizontal plane. When rotating the
 * device around the Xb axis with the Yb axis moving downwards, roll is positive and increasing.
 *
 * @see Using LSM303DLH for a tilt compensated electronic compass
 * http://www.st.com/stonline/products/literature/an/17353.pdfs
 */

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

/* Magnetometer Sensor Full Scale */
#define LSM303DLH_H_1_3G                0x20
#define LSM303DLH_H_1_9G                0x40
#define LSM303DLH_H_2_5G                0x60
#define LSM303DLH_H_4_0G                0x80
#define LSM303DLH_H_4_7G                0xA0
#define LSM303DLH_H_5_6G                0xC0
#define LSM303DLH_H_8_1G                0xE0

/* Magnetic Sensor Operating Mode */
#define LSM303DLH_MAG_NORMAL_MODE       0x00
#define LSM303DLH_MAG_POS_BIAS          0x01
#define LSM303DLH_MAG_NEG_BIAS          0x02
#define LSM303DLH_MAG_CC_MODE           0x00
#define LSM303DLH_MAG_SC_MODE           0x01
#define LSM303DLH_MAG_SLEEP_MODE        0x03

/* Magnetometer output data rate  */
#define LSM303DLH_MAG_ODR_75            0x00    /* 0.75Hz output data rate */
#define LSM303DLH_MAG_ODR1_5            0x04    /* 1.5Hz output data rate */
#define LSM303DLH_MAG_ODR3_0            0x08    /* 3Hz output data rate */
#define LSM303DLH_MAG_ODR7_5            0x09    /* 7.5Hz output data rate */
#define LSM303DLH_MAG_ODR15             0x10    /* 15Hz output data rate */
#define LSM303DLH_MAG_ODR30             0x14    /* 30Hz output data rate */
#define LSM303DLH_MAG_ODR75             0x18    /* 75Hz output data rate */




namespace naxsoft {

Compass compass; // preinstatiate

void Compass::init() {
	// change the ODR from 15 Hz to 30 Hz
	i2cProtocol.write(MAG_ADDRESS, CRA_REG_M, LSM303DLH_MAG_ODR30);

	//Enable Magnetometer in Continuous conversion mode
	i2cProtocol.write(MAG_ADDRESS, MR_REG_M, LSM303DLH_MAG_NORMAL_MODE);
}

bool Compass::isDataAvailable() {
	i2cProtocol.read(MAG_ADDRESS, SR_REG_M, 1);
	uint8_t reg = i2cProtocol.receive();
	return BITSSET(reg, BIT(0)); // RDY
}

/**
 * Read magnetometer
 * Resolution 1.5 Gauss
 * Discrete values from -2048 to +2048
 */
void Compass::read(compass_data* data) {
	while (!isDataAvailable()) {
		delayMicroseconds(10);
	}

	i2cProtocol.read(MAG_ADDRESS, OUT_X_H_M, 6);


	while (i2cProtocol.available() < 6) {
	}

	uint8_t xhm = i2cProtocol.receive(); //read OUT_X_H_M (MSB)
	uint8_t xlm = i2cProtocol.receive(); //read OUT_X_L_M (LSB)
	uint8_t yhm = i2cProtocol.receive(); //read OUT_Y_H_M (MSB)
	uint8_t ylm = i2cProtocol.receive(); //read OUT_Y_L_M (LSB)
	uint8_t zhm = i2cProtocol.receive(); //read OUT_Z_H_M (MSB)
	uint8_t zlm = i2cProtocol.receive(); //read OUT_Z_L_M (LSB)


    // offset and scale
	data->x =  (xhm << 8 | xlm);
	data->y =  (yhm << 8 | ylm);
	data->z =  (zhm << 8 | zlm);
}


} // namespace

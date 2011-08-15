#include <Wire.h>
#include <math.h>
#include "Compass.h"

#include "../I2C.h"

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

namespace naxsoft {

Compass compass; // preinstatiate

void Compass::init() {

	// change the ODR from 15 Hz to 30 Hz
	I2C::updateRegister(MAG_ADDRESS, CRA_REG_M, 0x14);

	//Enable Magnetometer in Continuous conversion mode
	I2C::updateRegister(MAG_ADDRESS, MR_REG_M, 0x00);
}

/**
 * Read magnetometer
 * Resolution 1.5 Gauss
 * Discrete values from -2048 to +2048
 */
void Compass::read(compass_data* data) {
	I2C::sendByte(MAG_ADDRESS, OUT_X_H_M);
	Wire.requestFrom(MAG_ADDRESS, 6); // read MR_REG_M

	while (Wire.available() < 6) {
	}

	uint8_t xhm = Wire.receive(); //read OUT_X_H_M (MSB)
	uint8_t xlm = Wire.receive(); //read OUT_X_L_M (LSB)
	uint8_t yhm = Wire.receive(); //read OUT_Y_H_M (MSB)
	uint8_t ylm = Wire.receive(); //read OUT_Y_L_M (LSB)
	uint8_t zhm = Wire.receive(); //read OUT_Z_H_M (MSB)
	uint8_t zlm = Wire.receive(); //read OUT_Z_L_M (LSB)

	data->x = - (xhm << 8 | xlm); // nagate because it is mounted upside down
	data->y = - (yhm << 8 | ylm); // nagate because it is mounted upside down
	data->z = - (zhm << 8 | zlm); // nagate because it is mounted upside down
}

double Compass::getHeading(compass_data* cdata) {
	/*
	Serial.print("getHeading: X = ");
	Serial.print(cdata->x);
	Serial.print("getHeading: Y = ");
	Serial.print(cdata->y);
	*/
	double azimuth = ::atan((double) cdata->y / (double) cdata->x);
	return azimuth;
}

double Compass::getTiltCompensatedHeading(compass_data* cdata, int16_t pitch, int16_t roll) {
	cdata->x = cdata->x * cos(pitch) + cdata->z * sin(pitch);
	cdata->y = cdata->x * sin(roll) * sin(pitch) + cdata->y * cos(roll) - cdata->z * sin(roll) * cos(pitch);
	return getHeading(cdata);
}

} // namespace

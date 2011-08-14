#include <Wire.h>
#include <WConstants.h>

#include "../I2C.h"
#include "Gyroscope.h"


/* l3g4200d gyroscope registers */
#define WHO_AM_I    	0x0F

#define CTRL_REG1       0x20    /* power control reg */
#define CTRL_REG2       0x21    /* power control reg */
#define CTRL_REG3       0x22    /* power control reg */
#define CTRL_REG4       0x23    /* interrupt control reg */
#define CTRL_REG5       0x24    /* interrupt control reg */
#define OUT_TEMP		0x26	/* Temperature data */
#define AXISDATA_REG    0x28

#define STATUS_REG		0x27

#define PM_OFF			0x00
#define PM_NORMAL		0x08
#define ENABLE_ALL_AXES	0x07

#define ODR200_BW12_5	0x00  /* ODR = 200Hz; BW = 12.5Hz */
#define ODR200_BW25		0x10  /* ODR = 200Hz; BW = 25Hz   */
#define ODR200_BW50		0x20  /* ODR = 200Hz; BW = 50Hz   */
#define ODR100_BW12_5	0x40  /* ODR = 100Hz; BW = 12.5Hz */
#define ODR100_BW25		0x50  /* ODR = 100Hz; BW = 25Hz   */
#define ODR400_BW25		0x90  /* ODR = 400Hz; BW = 25Hz   */
#define ODR400_BW50		0xA0  /* ODR = 400Hz; BW = 50Hz   */
#define ODR400_BW110	0xB0  /* ODR = 400Hz; BW = 110Hz  */
#define ODR800_BW50		0xE0  /* ODR = 800Hz; BW = 50Hz   */
#define ODR800_BW100	0xF0  /* ODR = 800Hz; BW = 100Hz  */

#define MIN_ST			175
#define MAX_ST			875



#define L3G4200D_Address 105 //I2C address of the L3G4200D

namespace naxsoft {

Gyroscope gyroscope;              // preinstatiate

int Gyroscope::setupL3G4200D(int scale){
	// Enable x, y, z and turn off power down:
	I2C::updateRegister(L3G4200D_Address, CTRL_REG1, 0b00001111);

	// If you'd like to adjust/use the HPF, you can edit the line below to configure CTRL_REG2:
	I2C::updateRegister(L3G4200D_Address, CTRL_REG2, 0b00000000);

	// Configure CTRL_REG3 to generate data ready interrupt on INT2
	// No interrupts used on INT1, if you'd like to configure INT1
	// or INT2 otherwise, consult the datasheet:
	I2C::updateRegister(L3G4200D_Address, CTRL_REG3, 0b00001000);

	// CTRL_REG4 controls the full-scale range, among other things:
	if(scale == 250){
		I2C::updateRegister(L3G4200D_Address, CTRL_REG4, 0b00000000);
	}else if(scale == 500){
		I2C::updateRegister(L3G4200D_Address, CTRL_REG4, 0b00010000);
	}else{
		I2C::updateRegister(L3G4200D_Address, CTRL_REG4, 0b00110000);
	}

	// CTRL_REG5 controls high-pass filtering of outputs, use it if you'd like:
	I2C::updateRegister(L3G4200D_Address, CTRL_REG5, 0b00000000);
	return 1;
}

void Gyroscope::getGyroValues(gyroscope_data* data){

  byte xMSB = I2C::readRegister(L3G4200D_Address, 0x29);
  byte xLSB = I2C::readRegister(L3G4200D_Address, 0x28);
  data->r = - ((xMSB << 8) | xLSB); // negate because mounted upsite-down

  byte yMSB = I2C::readRegister(L3G4200D_Address, 0x2B);
  byte yLSB = I2C::readRegister(L3G4200D_Address, 0x2A);
  data->p = - ((yMSB << 8) | yLSB); // negate because mounted upsite-down

  byte zMSB = I2C::readRegister(L3G4200D_Address, 0x2D);
  byte zLSB = I2C::readRegister(L3G4200D_Address, 0x2C);
  data->y = - ((zMSB << 8) | zLSB); // negate because mounted upsite-down
}


int16_t Gyroscope::getTemperature() {
	//I2C::sendByte(L3G4200D_Address, OUT_TEMP);
	//Wire.requestFrom(L3G4200D_Address, 1); // read MR_REG_M

	byte zMSB = I2C::readRegister(L3G4200D_Address, OUT_TEMP);

//	while (Wire.available() < 1) {
//	}

//	uint8_t xhm = Wire.receive(); //read OUT_X_H_M (MSB)
//	uint8_t xlm = Wire.receive(); //read OUT_X_L_M (LSB)

//	int16_t temp = (xlm << 8 | xhm);
	return zMSB;
}

} // namespace

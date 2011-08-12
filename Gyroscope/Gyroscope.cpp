#include <Wire.h>
#include <WConstants.h>

#include "../I2C.h"
#include "Gyroscope.h"

#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24

#define L3G4200D_Address 105 //I2C address of the L3G4200D

namespace naxsoft {

Gyroscope gyroscope;              // preinstatiate

int Gyroscope::setupL3G4200D(int scale){
  //From  Jim Lindblom of Sparkfun's code

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

	// CTRL_REG5 controls high-pass filtering of outputs, use it
	// if you'd like:
	I2C::updateRegister(L3G4200D_Address, CTRL_REG5, 0b00000000);
	return 1;
}

void Gyroscope::getGyroValues(gyroscope_data* data){

  byte xMSB = I2C::readRegister(L3G4200D_Address, 0x29);
  byte xLSB = I2C::readRegister(L3G4200D_Address, 0x28);
  data->x = - ((xMSB << 8) | xLSB); // negate because mounted upsite-down

  byte yMSB = I2C::readRegister(L3G4200D_Address, 0x2B);
  byte yLSB = I2C::readRegister(L3G4200D_Address, 0x2A);
  data->y = - ((yMSB << 8) | yLSB); // negate because mounted upsite-down

  byte zMSB = I2C::readRegister(L3G4200D_Address, 0x2D);
  byte zLSB = I2C::readRegister(L3G4200D_Address, 0x2C);
  data->z = - ((zMSB << 8) | zLSB); // negate because mounted upsite-down
}
} // namespace

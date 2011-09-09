#include "Gyroscope.h"

#include "../I2C/I2CProtocol.h"
#include "../bitops.h"

///* l3g4200d gyroscope registers */
//#define WHO_AM_I    	0x0F
//
//#define CTRL_REG1       0x20    /* power control reg */
//#define CTRL_REG2       0x21    /* power control reg */
//#define CTRL_REG3       0x22    /* power control reg */
//#define CTRL_REG4       0x23    /* interrupt control reg */
//#define CTRL_REG5       0x24    /* interrupt control reg */
//#define OUT_TEMP		0x26	/* Temperature data */
//#define STATUS_REG		0x27
//#define AXISDATA_REG    0x28
//
//
//#define PM_OFF			0x00
//#define PM_NORMAL		0x08
//#define ENABLE_ALL_AXES	0x07
//
//#define ODR200_BW12_5	0x00  /* ODR = 200Hz; BW = 12.5Hz */
//#define ODR200_BW25		0x10  /* ODR = 200Hz; BW = 25Hz   */
//#define ODR200_BW50		0x20  /* ODR = 200Hz; BW = 50Hz   */
//#define ODR100_BW12_5	0x40  /* ODR = 100Hz; BW = 12.5Hz */
//#define ODR100_BW25		0x50  /* ODR = 100Hz; BW = 25Hz   */
//#define ODR400_BW25		0x90  /* ODR = 400Hz; BW = 25Hz   */
//#define ODR400_BW50		0xA0  /* ODR = 400Hz; BW = 50Hz   */
//#define ODR400_BW110	0xB0  /* ODR = 400Hz; BW = 110Hz  */
//#define ODR800_BW50		0xE0  /* ODR = 800Hz; BW = 50Hz   */
//#define ODR800_BW100	0xF0  /* ODR = 800Hz; BW = 100Hz  */
//
//#define MIN_ST			175
//#define MAX_ST			875

#define L3G4_Address 105 //I2C address of the L3G4200D

/* TWI least significant bit of the device address */
#define FIRST_L3G4 0x0
#define SECOND_L3G4 0x1

/* Slave address (SAD) of the device on TWI bus */
#define L3G4_TWI_ADDR 0xD0

/* WHO_AM_I register content */
#define L3G4_WHO_AM_I_VAL 0xD3

/* SUB (register address) auto-increment */
#define L3G4_AUTOINC 0x80

/* Device registers */
#define L3G4_WHO_AM_I           0x0F
#define L3G4_CTRL_REG1          0x20
#define L3G4_CTRL_REG2          0x21
#define L3G4_CTRL_REG3          0x22
#define L3G4_CTRL_REG4          0x23
#define L3G4_CTRL_REG5          0x24
#define L3G4_REFERENCE          0x25
#define L3G4_OUT_TEMP           0x26
#define L3G4_STATUS_REG         0x27
#define L3G4_OUT_X_L            0x28
#define L3G4_OUT_X_H            0x29
#define L3G4_OUT_Y_L            0x2A
#define L3G4_OUT_Y_H            0x2B
#define L3G4_OUT_Z_L            0x2C
#define L3G4_OUT_Z_H            0x2D
#define L3G4_FIFO_CTRL_REG      0x2E
#define L3G4_FIFO_SRC_REG       0x2F
#define L3G4_INT1_CFG           0x30
#define L3G4_INT1_SRC           0x31
#define L3G4_INT1_THS_XH        0x32
#define L3G4_INT1_THS_XL        0x33
#define L3G4_INT1_THS_YH        0x34
#define L3G4_INT1_THS_YL        0x35
#define L3G4_INT1_THS_ZH        0x36
#define L3G4_INT1_THS_ZL        0x37
#define L3G4_INT1_DURATION      0x38

/* CTRL_REG1 bits */
#define L3G4_DR1        7
#define L3G4_DR0        6
#define L3G4_BW1        5
#define L3G4_BW0        4
#define L3G4_PD         3
#define L3G4_Zen        2
#define L3G4_Yen        1
#define L3G4_Xen        0

/* CTRL_REG2 bits */
// reserved             7 - Value loaded at boot and must not be changed
// reserved             6 - Value loaded at boot and must not be changed
#define L3G4_HPM1       5
#define L3G4_HPM0       4
#define L3G4_HPCF3      3
#define L3G4_HPCF2      2
#define L3G4_HPCF1      1
#define L3G4_HPCF0      0

/* CTRL_REG3 bits */
#define L3G4_I1_Int1    7
#define L3G4_I1_Boot    6
#define L3G4_H_Lactive  5
#define L3G4_PP_OD      4
#define L3G4_I2_DRDY    3
#define L3G4_I2_WTM     2
#define L3G4_I2_ORun    1
#define L3G4_I2_Empty   0

/* CTRL_REG4 bits */
#define L3G4_BDU        7
#define L3G4_BLE        6
#define L3G4_FS1        5
#define L3G4_FS0        4
// reserved             3
#define L3G4_ST1        2
#define L3G4_ST0        1
#define L3G4_SIM        0

/* CTRL_REG5 bits */
#define L3G4_BOOT       7
#define L3G4_FIFO_EN    6
// reserved             5
#define L3G4_HPen       4
#define L3G4_INT1_Sel1  3
#define L3G4_INT1_Sel0  2
#define L3G4_Out_Sel1   1
#define L3G4_Out_Sel0   0

/* STATUS_REG bits */
#define L3G4_ZYXOR      7
#define L3G4_ZOR        6
#define L3G4_YOR        5
#define L3G4_XOR        4
#define L3G4_ZYXDA      3
#define L3G4_ZDA        2
#define L3G4_YDA        1
#define L3G4_XDA        0

/* FIFO_CTRL_REG bits */
#define L3G4_FM2        7
#define L3G4_FM1        6
#define L3G4_FM0        5
#define L3G4_WTM4       4
#define L3G4_WTM3       3
#define L3G4_WTM2       2
#define L3G4_WTM1       1
#define L3G4_WTM0       0

/* FIFO_SRC_REG bits */
#define L3G4_WTM        7
#define L3G4_OVRN       6
#define L3G4_EMPTY      5
#define L3G4_FSS4       4
#define L3G4_FSS3       3
#define L3G4_FSS2       2
#define L3G4_FSS1       1
#define L3G4_FSS0       0

/* INT1_CFG bits */
#define L3G4_AND_OR     7
#define L3G4_LIR        6
#define L3G4_ZHIE       5
#define L3G4_ZLIE       4
#define L3G4_YHIE       3
#define L3G4_YLIE       2
#define L3G4_XHIE       1
#define L3G4_XLIE       0

/* INT1_SRC bits */
// reserved             7
#define L3G4_INT1_IA    6
#define L3G4_INT1_ZH    5
#define L3G4_INT1_ZL    4
#define L3G4_INT1_YH    3
#define L3G4_INT1_YL    2
#define L3G4_INT1_XH    1
#define L3G4_INT1_XL    0

/* INT1_DURATION bits */
#define L3G4_WAIT       7
#define L3G4_D6         6
#define L3G4_D5         5
#define L3G4_D4         4
#define L3G4_D3         3
#define L3G4_D2         2
#define L3G4_D1         1
#define L3G4_D0         0

namespace naxsoft {

Gyroscope gyroscope; // preinstatiate

int Gyroscope::init(int scale) {
	// If you'd like to adjust/use the HPF, you can edit the line below to configure CTRL_REG2:
	// I2C::updateRegister(L3G4_Address, L3G4_CTRL_REG2, 0b00000000);
	i2cProtocol.write(L3G4_Address, L3G4_CTRL_REG2, 0b00000000);

	// Configure CTRL_REG3 to generate data ready interrupt on INT2
	// No interrupts used on INT1, if you'd like to configure INT1
	// or INT2 otherwise, consult the datasheet:
	// I2C::updateRegister(L3G4_Address, L3G4_CTRL_REG3, 0b00001000);
	i2cProtocol.write(L3G4_Address, L3G4_CTRL_REG3, 0b00001000);

	// CTRL_REG4 controls the full-scale range, among other things:

	// CTRL_REG4
	// 0x0000 0000
	//   _        | BDU
	//    _       | BLE
	//     __     | FS1 FS0
	//        x   | x
	//         __ | ST1
	//           _| ST0
	// If the reading of the angular rate data is particularly slow and cannot be synchronized (or it
	// is not required) with either the XYZDA bit present inside the STATUS_REG or with the RDY
	// signal, it is strongly recommended to set the BDU (block data update) bit in CTRL_REG4 to 1.
	if (scale == 250) {
		i2cProtocol.write(L3G4_Address, L3G4_CTRL_REG4, 0b10000000);
	} else if (scale == 500) {
		i2cProtocol.write(L3G4_Address, L3G4_CTRL_REG4, 0b10010000);
	} else {
		i2cProtocol.write(L3G4_Address, L3G4_CTRL_REG4, 0b10110000);
	}

	// CTRL_REG5 controls high-pass filtering of outputs, use it if you'd like:
	// i2cProtocol.write(L3G4_Address, CTRL_REG5, 0b00000000);

	// Turn on sensor, Enable x, y, z, ODR = 100Hz
	i2cProtocol.write(L3G4_Address, L3G4_CTRL_REG1, 0b00001111);
	return 1;
}

bool Gyroscope::isDataAvailable() {
	i2cProtocol.read(L3G4_Address, L3G4_STATUS_REG, 1);
	uint8_t reg = i2cProtocol.receive();
	return BITSSET(reg, BIT(L3G4_ZYXDA));
}

void Gyroscope::getGyroValues(gyroscope_data* data) {
	//	1. Read STATUS_REG
	//	2. If STATUS_REG(3) = 0 then go to 1
	//	3. If STATUS_REG(7) = 1 then some data have been overwritten
	//	4. read XYZ
	while (!isDataAvailable()) {
		delayMicroseconds(10);
	}

	i2cProtocol.read(L3G4_Address, L3G4_OUT_X_L | L3G4_AUTOINC, 6);

	while (i2cProtocol.available() < 6) {
	}

	uint8_t xLSB = i2cProtocol.receive(); //read OUT_X_L_A (LSB)
	uint8_t xMSB = i2cProtocol.receive(); //read OUT_X_H_A (MSB)
	uint8_t yLSB = i2cProtocol.receive(); //read OUT_Y_L_A (LSB)
	uint8_t yMSB = i2cProtocol.receive(); //read OUT_Y_H_A (MSB)
	uint8_t zLSB = i2cProtocol.receive(); //read OUT_Z_L_A (LSB)
	uint8_t zMSB = i2cProtocol.receive(); //read OUT_Z_H_A (MSB)

	data->r = ((xMSB << 8) | xLSB); // negate because mounted upsite-down
	data->p = ((yMSB << 8) | yLSB); // negate because mounted upsite-down
	data->y = ((zMSB << 8) | zLSB); // negate because mounted upsite-down


	// The scale values (sensitivity) from page 10 are : 8.75e-3,17.5e-3, 70e-3 for 250,500,2000 respectively.
	//	250dps setting: 224/256 to get centi-degrees/sec
	//	500 dps: 45/256 to get (roughly) deci-degrees/sec
	//	2000 dps: 18/256 to get (roughly) deci-degrees/sec
	data->r = (data->r * 18) >> 8;
	data->p = (data->p * 18) >> 8;
	data->y = (data->y * 18) >> 8;
}

/**
 * The L3G4200D is provided with an internal temperature sensor that is suitable for delta
 * temperature measurement. Temperature data are generated with a frequency of 1 Hz and
 * are stored inside the OUT_TEMP register in two’s complement format, with a sensitivity if -1
 * LSB/°C.
 *
 * 0x0000 0000
 * Temp7 Temp6 Temp5 Temp4 Temp3 Temp2 Temp1 Temp0
 */
int8_t Gyroscope::getTemperature() {
	i2cProtocol.read(L3G4_Address, L3G4_OUT_TEMP, 6);
	return i2cProtocol.receive();
}

} // namespace

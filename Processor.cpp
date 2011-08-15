#include <WConstants.h>
#include <HardwareSerial.h>
#include "Tlc5940/Tlc5940.h"
#include "Processor.h"
#include "DcMotor.h"
#include "Protocol.h"

#include "Accelermeter/Accelerometer.h"
#include "Compass/Compass.h"
#include "Gyroscope/Gyroscope.h"

#define DC_MOTOR_SPEED 1
#define TLC 4

#define ACCELEROMETER 5
#define COMPASS 6
#define GYROSCOPE 7

namespace naxsoft {

Processor processor; // preinstatiate

boolean Processor::process(Protocol const* const in, Protocol const* const out) {
	//  Serial.println("Processor::process");
	//	  char name = 0;
	TMessageType messageType;
	//	  int32_t seqid = 0;
	in->readMessageBegin(messageType);
	int8_t rpc = 0;
	in->readI8(rpc);

	bool result = false;

	switch(messageType) {
	case T_CALL:
		result = processSyncFunctionCall(rpc, in, out);
		break;
	case T_ONEWAY:
		result = processASyncFunctionCall(rpc, in, out);
		break;
	default:
		result = false;
		break;
	}

	in->readMessageEnd();
	return result;
//	delay(100); //Just here to slow down the serial to make it more readable


	//  Serial.print("name = ");
	//  Serial.println(name);
	//  Serial.print("messageType = ");
	//  Serial.println(messageType);
	//  Serial.print("seqid = ");
	//  Serial.println(seqid);
}

boolean Processor::processSyncFunctionCall(int8_t rpc, Protocol const* const in, Protocol const* const out) {
//	Serial.print("RPC = ");
//	Serial.println(rpc);

	boolean result = true;
	switch (rpc) {
	case ACCELEROMETER:
		result = this->accelerometer(in, out);
		break;
	case COMPASS:
		result = this->compass(in, out);
		break;
	case GYROSCOPE:
		result = this->gyroscope(in, out);
		break;
	default:
		result = false;
	};
	return result;

}
boolean Processor::processASyncFunctionCall(int8_t rpc, Protocol const* const in, Protocol const* const out) {
//	Serial.print("RPC = ");
//	Serial.println(rpc);

	boolean result = true;
	switch (rpc) {
	case DC_MOTOR_SPEED:
		result = this->processDcMotor(in, out);
		break;
	case TLC:
		result = this->tlc(in, out);
		break;
	default:
		result = false;
	};
	in->readMessageEnd();
	return result;

}

boolean Processor::processDcMotor(Protocol const* const in, Protocol const* const out) {
	(void) out;
	int8_t motor1;
	int8_t motor2;
	int8_t motor3;
	int8_t motor4;
	int16_t speed1;
	int16_t speed2;
	int16_t speed3;
	int16_t speed4;
	int8_t direction1;
	int8_t direction2;
	int8_t direction3;
	int8_t direction4;

	in->readI8(motor1);
	in->readI8(motor2);
	in->readI8(motor3);
	in->readI8(motor4);
	in->readI16(speed1);
	in->readI16(speed2);
	in->readI16(speed3);
	in->readI16(speed4);
	in->readI8(direction1);
	in->readI8(direction2);
	in->readI8(direction3);
	in->readI8(direction4);
	dcMotor.start(motor1, speed1, direction1);
	dcMotor.start(motor2, speed2, direction2);
	dcMotor.start(motor3, speed3, direction3);
	dcMotor.start(motor4, speed4, direction4);
	return true;
}

boolean Processor::tlc(Protocol const* in, Protocol const* const out) {
	(void) out;
	// Serial.println("Processor::tlc");
	int8_t channel;
	int8_t value;

	in->readI8(channel);
	// Serial.print("channel = ");
	// Serial.println(channel);

	in->readI8(value);
	// Serial.print("value = ");
	// Serial.println(value);

	Tlc.set(channel, value);
	return true;
}

boolean Processor::accelerometer(Protocol const* in, Protocol const* const out) {
	(void) in;

	accelerometer_data adata;
	naxsoft::accelerometer.read(&adata);

	out->writeMessageBegin(T_REPLY);
	out->writeI16(adata.x);
	out->writeI16(adata.y);
	out->writeI16(adata.z);
	out->writeMessageEnd();

	/*
	Serial.print("Accelerometer: ");
	Serial.print("X: ");
	Serial.print(adata.x);
	Serial.print(" Y: ");
	Serial.print(adata.y);
	Serial.print(" Z: ");
	Serial.println(adata.z);
	*/

	return true;
}

boolean Processor::compass(Protocol const* in, Protocol const* const out) {
	(void) in;

	compass_data cdata;
	naxsoft::compass.read(&cdata);
	out->writeMessageBegin(T_REPLY);
	out->writeI16(cdata.x);
	out->writeI16(cdata.y);
	out->writeI16(cdata.z);
	out->writeMessageEnd();

	/*
	Serial.print("      Compass: ");
	Serial.print("X: ");
	Serial.print(cdata.x);
	Serial.print(" Y: ");
	Serial.print(cdata.y);
	Serial.print(" Z: ");
	Serial.print(cdata.z);


	Serial.print(" heading: ");
	Serial.print(naxsoft::compass.getHeading(&cdata));
	*/

//	int pitch = gdata.p;
//	int roll = gdata.r;

	/*
	Serial.print(" comp heading: ");
	Serial.print(naxsoft::compass.getTiltCompensatedHeading(&cdata, pitch, roll));
	Serial.println();
	*/
	return true;
}

boolean Processor::gyroscope(Protocol const* in, Protocol const* const out) {
	(void) in;
	gyroscope_data gdata;
	naxsoft::gyroscope.getGyroValues(&gdata); // This will update x, y, and z with new values

	out->writeMessageBegin(T_REPLY);
	out->writeI16(gdata.p);
	out->writeI16(gdata.r);
	out->writeI16(gdata.y);
	out->writeMessageEnd();

	/*
	Serial.print("    Gyroscope:  ");
	Serial.print(" Yaw: ");
	Serial.print(gdata.y);
	Serial.print(" Pitch: ");
	Serial.print(gdata.p);
	Serial.print(" Roll: ");
	Serial.print(gdata.r);
	Serial.print(" Temp: ");
	Serial.print(naxsoft::gyroscope.getTemperature());
	Serial.println();
	*/
	return true;
}
} // namespace

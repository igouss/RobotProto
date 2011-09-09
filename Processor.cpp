#include <WConstants.h>
#include <HardwareSerial.h>
#include <util/crc16.h>

#include "Tlc5940/Tlc5940.h"
#include "Processor.h"
#include "DcMotor.h"

#include "Accelermeter/Accelerometer.h"
#include "Compass/Compass.h"
#include "Gyroscope/Gyroscope.h"

#define DC_MOTOR_SPEED 1
#define TLC 4

#define ACCELEROMETER 5
#define COMPASS 6
#define GYROSCOPE 7

namespace naxsoft {

uint8_t lo8(uint16_t val) {
	return (uint8_t) val;
}

uint8_t hi8(uint16_t val) {
	return (uint8_t) (val >> 8);
}

Processor processor; // preinstatiate

boolean Processor::process(Protocol const* const proto) {
	//  Serial.println("Processor::process");
	//	  char name = 0;
	TMessageType messageType;
	//	  int32_t seqid = 0;
	proto->readMessageBegin(messageType);
	int8_t rpc = 0;
	proto->readI8(rpc);

	bool result = false;

	switch (messageType) {
	case T_CALL:
	case T_ONEWAY:
		result = processFunctionCall(rpc, proto);
		break;
	default:
		result = false;
		break;
	}
	return result;

	//  Serial.print("name = ");
	//  Serial.println(name);
	//  Serial.print("messageType = ");
	//  Serial.println(messageType);
	//  Serial.print("seqid = ");
	//  Serial.println(seqid);
}

boolean Processor::processFunctionCall(int8_t rpc, Protocol const* const proto) {
	//	Serial.print("RPC = ");
	//	Serial.println(rpc);

	boolean result = false;
	switch (rpc) {
	case ACCELEROMETER:
		result = this->accelerometer(proto);
		break;
	case COMPASS:
		result = this->compass(proto);
		break;
	case GYROSCOPE:
		result = this->gyroscope(proto);
		break;
	case DC_MOTOR_SPEED:
		result = this->processDcMotor(proto);
		break;
	case TLC:
		result = this->tlc(proto);
		break;
	default:
		result = false;
	};
	return result;

}

boolean Processor::processDcMotor(Protocol const* const proto) {
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

	proto->readI8(motor1);
	proto->readI8(motor2);
	proto->readI8(motor3);
	proto->readI8(motor4);
	proto->readI16(speed1);
	proto->readI16(speed2);
	proto->readI16(speed3);
	proto->readI16(speed4);
	proto->readI8(direction1);
	proto->readI8(direction2);
	proto->readI8(direction3);
	proto->readI8(direction4);
	proto->readMessageEnd();



	dcMotor.start(motor1, speed1, direction1);
	dcMotor.start(motor2, speed2, direction2);
	dcMotor.start(motor3, speed3, direction3);
	dcMotor.start(motor4, speed4, direction4);
	return true;
}

boolean Processor::tlc(Protocol const* proto) {
	// Serial.println("Processor::tlc");
	int8_t channel;
	int8_t value;

	proto->readI8(channel);
	// Serial.print("channel = ");
	// Serial.println(channel);

	proto->readI8(value);
	// Serial.print("value = ");
	// Serial.println(value);

	proto->readMessageEnd();

	Tlc.set(channel, value);
	return true;
}

boolean Processor::accelerometer(Protocol const* proto) {

	uint16_t localcrc = 0xffff;
	accelerometer_data adata;
	naxsoft::accelerometer.read(&adata);

	proto->readMessageEnd();
	proto->writeMessageBegin(T_REPLY);
	proto->writeI16(adata.x);
	proto->writeI16(adata.y);
	proto->writeI16(adata.z);


	localcrc = ::_crc_ccitt_update(localcrc, hi8(adata.x));
	localcrc = ::_crc_ccitt_update(localcrc, lo8(adata.x));
	localcrc = ::_crc_ccitt_update(localcrc, hi8(adata.y));
	localcrc = ::_crc_ccitt_update(localcrc, lo8(adata.y));
	localcrc = ::_crc_ccitt_update(localcrc, hi8(adata.z));
	localcrc = ::_crc_ccitt_update(localcrc, lo8(adata.z));
	proto->writeI16(localcrc);

	proto->writeMessageEnd();


	return true;
}

boolean Processor::compass(Protocol const* proto) {
	uint16_t localcrc = 0xffff;
	compass_data cdata;
	naxsoft::compass.read(&cdata);

	proto->readMessageEnd();
	proto->writeMessageBegin(T_REPLY);
	proto->writeI16(cdata.x);
	proto->writeI16(cdata.y);
	proto->writeI16(cdata.z);


	localcrc = ::_crc_ccitt_update(localcrc, hi8(cdata.x));
	localcrc = ::_crc_ccitt_update(localcrc, lo8(cdata.x));
	localcrc = ::_crc_ccitt_update(localcrc, hi8(cdata.y));
	localcrc = ::_crc_ccitt_update(localcrc, lo8(cdata.y));
	localcrc = ::_crc_ccitt_update(localcrc, hi8(cdata.z));
	localcrc = ::_crc_ccitt_update(localcrc, lo8(cdata.z));
	proto->writeI16(localcrc);

	proto->writeMessageEnd();

	return true;
}

boolean Processor::gyroscope(Protocol const* proto) {
	uint16_t localcrc = 0xffff;
	gyroscope_data gdata;
	naxsoft::gyroscope.getGyroValues(&gdata); // This will update x, y, and z with new values

	proto->readMessageEnd();
	proto->writeMessageBegin(T_REPLY);
	proto->writeI16(gdata.p);
	proto->writeI16(gdata.r);
	proto->writeI16(gdata.y);

	localcrc = ::_crc_ccitt_update(localcrc, hi8(gdata.p));
	localcrc = ::_crc_ccitt_update(localcrc, lo8(gdata.p));
	localcrc = ::_crc_ccitt_update(localcrc, hi8(gdata.r));
	localcrc = ::_crc_ccitt_update(localcrc, lo8(gdata.r));
	localcrc = ::_crc_ccitt_update(localcrc, hi8(gdata.y));
	localcrc = ::_crc_ccitt_update(localcrc, lo8(gdata.y));
	proto->writeI16(localcrc);


	proto->writeMessageEnd();

	return true;
}
} // namespace

#include "Wire.h"
#include "I2C.h"

namespace naxsoft {
    // I2C functions

    void I2C::sendByte(int deviceAddress, uint8_t dataValue) {
      Wire.beginTransmission(deviceAddress);
      Wire.send(dataValue);
      Wire.endTransmission();
    }

    /**
      * i2c write byte to target device in specified data register address
      */
    void I2C::updateRegister(int deviceAddress, uint8_t dataAddress, uint8_t dataValue) {
      Wire.beginTransmission(deviceAddress);
      Wire.send(dataAddress);
      Wire.send(dataValue);
      Wire.endTransmission();
    }

    // data can be maximum of about 30 bytes, because the Wire library has a buffer of 32 bytes
    void I2C::writePage( int deviceAddress, unsigned int addressPage, byte* data, byte length )
    {
        Wire.beginTransmission(deviceAddress);
        Wire.send((int)(addressPage >> 8)); // MSB
        Wire.send((int)(addressPage & 0xFF)); // LSB
        byte c;
        for ( c = 0; c < length; c++) {
          Wire.send(data[c]);
        }
        Wire.endTransmission();
    }


    byte I2C::readByte(int deviceAddress) {
        Wire.requestFrom(deviceAddress, 1);
        return Wire.receive();
    }

    int I2C::readRegister(int deviceAddress, byte address) {
        Wire.beginTransmission(deviceAddress);
        Wire.send(address); // register to read
        Wire.endTransmission();

        Wire.requestFrom(deviceAddress, 1); // read a byte

        while(!Wire.available()) {
            // waiting
        }

        return Wire.receive();
    }


    int I2C::readWord(int deviceAddress) {
      unsigned char msb, lsb;
      Wire.requestFrom(deviceAddress, 2); // request two bytes
      while(!Wire.available()) {
    	  // wait until data available
      }
      msb = Wire.receive();
      while(!Wire.available()) {
    	  // wait until data available
      }
      lsb = Wire.receive();
      return (((int)msb<<8) | ((int)lsb));
    }

    int I2C::readReverseWord(int deviceAddress) {
      byte lowerByte;
      Wire.requestFrom(deviceAddress, 2);
      lowerByte = Wire.receive();
      return (Wire.receive() << 8) | lowerByte;
    }

    byte I2C::readWho(int deviceAddress) {
      // read the ID of the I2C device
      Wire.beginTransmission(deviceAddress);
      Wire.send(0x00);
      Wire.endTransmission();
      delay(100);
      Wire.requestFrom(deviceAddress, 1);
      return Wire.receive();
    }


    byte I2C::eeprom_read_byte( int deviceAddress, unsigned int address ) {
        byte rdata = 0xFF;
        Wire.beginTransmission(deviceAddress);
        Wire.send((int)(address >> 8)); // MSB
        Wire.send((int)(address & 0xFF)); // LSB
        Wire.endTransmission();
        Wire.requestFrom(deviceAddress,1);
        if (Wire.available()) {
          rdata = Wire.receive();
        }
        return rdata;
    }


      // maybe let's not read more than 30 or 32 bytes at a time!
      void I2C::eeprom_read_buffer( int deviceAddress, unsigned int address, byte *buffer, int length ) {
        Wire.beginTransmission(deviceAddress);
        Wire.send((int)(address >> 8)); // MSB
        Wire.send((int)(address & 0xFF)); // LSB
        Wire.endTransmission();
        Wire.requestFrom(deviceAddress,length);
        int c = 0;
        for ( c = 0; c < length; c++ ) {
          if (Wire.available()) buffer[c] = Wire.receive();
        }
    }
}

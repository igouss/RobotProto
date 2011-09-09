/*
  I2C.h - I2C library
  Copyright (c) 2011 Wayne Truchsess.  All right reserved.
  Rev 1.0 - August 8th, 2011
  Rev 1.1 - August 23rd, 2011 Iouri Goussev (original from http://www.dsscircuits.com/articles/arduino-i2c-master-library.html)
    * Removed warning, renamed to I2CProtocol to remove ambiguity.

  This is a modified version of the Arduino Wire/TWI
  library.  Functions were rewritten to provide more functionality
  and also the use of Repeated Start.  Some I2C devices will not
  function correctly without the use of a Repeated Start.  The
  initial version of this library only supports the Master.


  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "WProgram.h"

#ifndef I2CProtocol_h
#define I2CProtocol_h


#define START           0x08
#define REPEATED_START  0x10
#define MT_SLA_ACK	0x18
#define MT_DATA_ACK     0x28
#define MR_SLA_ACK	0x40
#define MR_DATA_ACK     0x50
#define MR_DATA_NACK    0x58
#define TWI_STATUS      (TWSR & 0xF8)
#define SLA_W(address)  (address << 1)
#define SLA_R(address)  ((address << 1) + 0x01)
#define cbi(sfr, bit)   (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit)   (_SFR_BYTE(sfr) |= _BV(bit))

#define MAX_BUFFER_SIZE 32


#ifndef CPU_FREQ
#define CPU_FREQ 16000000L
#endif



class I2CProtocol
{
  public:
	I2CProtocol();
    void begin();
    void end();
    void setSpeed(boolean);
    void pullup(boolean);
    ///////carry over from Wire library////////
    uint8_t returnStatusWire;
    uint8_t beginTransmission(uint8_t);
    uint8_t beginTransmission(int);
    uint8_t send(uint8_t);
    uint8_t send(int);
    uint8_t endTransmission();
    uint8_t requestFrom(uint8_t, uint8_t);
    uint8_t requestFrom(int, int);
    uint8_t available();
    ///////////////////////////////////////////
    uint8_t write(uint8_t, uint8_t);
    uint8_t write(int, int);
    uint8_t write(uint8_t, uint8_t, uint8_t);
    uint8_t write(int, int, int);
    uint8_t write(uint8_t, uint8_t, char*);
    uint8_t write(uint8_t, uint8_t, uint8_t*, uint8_t);
    uint8_t read(uint8_t, uint8_t);
    uint8_t read(int, int);
    uint8_t read(uint8_t, uint8_t, uint8_t);
    uint8_t read(int, int, int);
    uint8_t read(uint8_t, uint8_t, uint8_t*);
    uint8_t read(uint8_t, uint8_t, uint8_t, uint8_t*);
    uint8_t receive();

  private:
    uint8_t start();
    uint8_t sendAddress(uint8_t);
    uint8_t sendByte(uint8_t);
    uint8_t receiveByte(boolean);
    void stop();
    uint8_t returnStatus;
    uint8_t nack;
    uint8_t data[MAX_BUFFER_SIZE];
    static uint8_t bytesAvailable;
    static uint8_t bufferIndex;
    static uint8_t totalBytes;

};

extern I2CProtocol i2cProtocol;

#endif

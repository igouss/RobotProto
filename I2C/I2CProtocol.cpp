/*
  I2C.cpp - I2C library
  Copyright (c) 2011 Wayne Truchsess.  All right reserved.
  Rev 1.0 - August 8th, 2011

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
#include "I2CProtocol.h"



uint8_t I2CProtocol::bytesAvailable = 0;
uint8_t I2CProtocol::bufferIndex = 0;
uint8_t I2CProtocol::totalBytes = 0;

I2CProtocol::I2CProtocol()
{
}


////////////// Public Methods ////////////////////////////////////////



void I2CProtocol::begin()
{
  #if defined(__AVR_ATmega168__) || defined(__AVR_ATmega8__) || defined(__AVR_ATmega328P__)
    // activate internal pull-ups for twi
    // as per note from atmega8 manual pg167
    sbi(PORTC, 4);
    sbi(PORTC, 5);
  #else
    // activate internal pull-ups for twi
    // as per note from atmega128 manual pg204
    sbi(PORTD, 0);
    sbi(PORTD, 1);
  #endif
  // initialize twi prescaler and bit rate
  cbi(TWSR, TWPS0);
  cbi(TWSR, TWPS1);
  TWBR = ((CPU_FREQ / 100000) - 16) / 2;
  // enable twi module, acks, and twi interrupt
  TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA);
}

void I2CProtocol::end()
{
  TWCR = 0;
}

void I2CProtocol::setSpeed(boolean _fast)
{
  if(!_fast)
  {
    TWBR = ((CPU_FREQ / 100000) - 16) / 2;
  }
  else
  {
    TWBR = ((CPU_FREQ / 400000) - 16) / 2;
  }
}

void I2CProtocol::pullup(boolean activate)
{
  if(activate)
  {
    #if defined(__AVR_ATmega168__) || defined(__AVR_ATmega8__) || defined(__AVR_ATmega328P__)
      // activate internal pull-ups for twi
      // as per note from atmega8 manual pg167
      sbi(PORTC, 4);
      sbi(PORTC, 5);
    #else
      // activate internal pull-ups for twi
      // as per note from atmega128 manual pg204
      sbi(PORTD, 0);
      sbi(PORTD, 1);
    #endif
  }
  else
  {
    #if defined(__AVR_ATmega168__) || defined(__AVR_ATmega8__) || defined(__AVR_ATmega328P__)
      // deactivate internal pull-ups for twi
      // as per note from atmega8 manual pg167
      cbi(PORTC, 4);
      cbi(PORTC, 5);
    #else
      // deactivate internal pull-ups for twi
      // as per note from atmega128 manual pg204
      cbi(PORTD, 0);
      cbi(PORTD, 1);
    #endif
  }
}

/////////////carry over from Wire library ///////////

uint8_t I2CProtocol::beginTransmission(uint8_t address)
{
  returnStatusWire = 0;
  returnStatus = 0;
  returnStatus = start();
  returnStatusWire = returnStatus;
  if(returnStatus){return(returnStatus);}
  returnStatus = sendAddress(SLA_W(address));
  returnStatusWire = returnStatus;
  return(returnStatus);
}

uint8_t I2CProtocol::beginTransmission(int address)
{
  return(beginTransmission((uint8_t) address));
}

uint8_t I2CProtocol::send(uint8_t data)
{
  if(returnStatusWire)
  {
    return(returnStatusWire);
  }
  returnStatus = 0;
  returnStatus = sendByte(data);
  returnStatusWire = returnStatus;
  return(returnStatus);
}

uint8_t I2CProtocol::send(int data)
{
  return(send((uint8_t) data));
}

uint8_t I2CProtocol::endTransmission()
{
  stop();
  return(returnStatusWire);
}

uint8_t I2CProtocol::requestFrom(int address, int numberBytes)
{
  return(requestFrom((uint8_t) address, (uint8_t) numberBytes));
}

uint8_t I2CProtocol::requestFrom(uint8_t address, uint8_t numberBytes)
{
  returnStatus = 0;
  returnStatus = read(address,numberBytes);
  if(!returnStatus)
  {
    return(numberBytes);
  }
  return(0);
}

uint8_t I2CProtocol::available()
{
  return(bytesAvailable);
}

uint8_t I2CProtocol::receive()
{
  bufferIndex = totalBytes - bytesAvailable;
  if(!bytesAvailable)
  {
    bufferIndex = 0;
    return(0);
  }
  bytesAvailable--;
  return(data[bufferIndex]);
}





/////////////////////////////////////////////////////

uint8_t I2CProtocol::write(uint8_t address, uint8_t registerAddress)
{
  returnStatus = 0;
  returnStatus = start();
  if(returnStatus){return(returnStatus);}
  returnStatus = sendAddress(SLA_W(address));
  if(returnStatus){return(returnStatus);}
  returnStatus = sendByte(registerAddress);
  if(returnStatus){return(returnStatus);}
  stop();
  return(returnStatus);
}

uint8_t I2CProtocol::write(int address, int registerAddress)
{
  return(write((uint8_t) address, (uint8_t) registerAddress));
}

uint8_t I2CProtocol::write(uint8_t address, uint8_t registerAddress, uint8_t data)
{
  returnStatus = 0;
  returnStatus = start();
  if(returnStatus){return(returnStatus);}
  returnStatus = sendAddress(SLA_W(address));
  if(returnStatus){return(returnStatus);}
  returnStatus = sendByte(registerAddress);
  if(returnStatus){return(returnStatus);}
  returnStatus = sendByte(data);
  if(returnStatus){return(returnStatus);}
  stop();
  return(returnStatus);
}

uint8_t I2CProtocol::write(int address, int registerAddress, int data)
{
  return(write((uint8_t) address, (uint8_t) registerAddress, (uint8_t) data));
}

uint8_t I2CProtocol::write(uint8_t address, uint8_t registerAddress, char *data)
{
  uint8_t bufferLength = strlen(data);
  returnStatus = 0;
  returnStatus = write(address, registerAddress, (uint8_t*)data, bufferLength);
  return(returnStatus);
}

uint8_t I2CProtocol::write(uint8_t address, uint8_t registerAddress, uint8_t *data, uint8_t numberBytes)
{
  returnStatus = 0;
  returnStatus = start();
  if(returnStatus){return(returnStatus);}
  returnStatus = sendAddress(SLA_W(address));
  if(returnStatus){return(returnStatus);}
  returnStatus = sendByte(registerAddress);
  if(returnStatus){return(returnStatus);}
  for (uint8_t i = 0; i < numberBytes; i++)
  {
    returnStatus = sendByte(data[i]);
    if(returnStatus){return(returnStatus);}
  }
  stop();
  return(returnStatus);
}

uint8_t I2CProtocol::read(int address, int numberBytes)
{
  return(read((uint8_t) address, (uint8_t) numberBytes));
}

uint8_t I2CProtocol::read(uint8_t address, uint8_t numberBytes)
{
  bytesAvailable = 0;
  bufferIndex = 0;
  if(numberBytes == 0){numberBytes++;}
  nack = numberBytes - 1;
  returnStatus = 0;
  returnStatus = start();
  if(returnStatus){return(returnStatus);}
  returnStatus = sendAddress(SLA_R(address));
  if(returnStatus){return(returnStatus);}
  for(uint8_t i = 0; i < numberBytes; i++)
  {
    if( i == nack )
    {
      returnStatus = receiveByte(0);
      if(returnStatus != MR_DATA_NACK){return(returnStatus);}
    }
    else
    {
      returnStatus = receiveByte(1);
      if(returnStatus != MR_DATA_ACK){return(returnStatus);}
    }
    data[i] = TWDR;
    bytesAvailable = i+1;
    totalBytes = i+1;
  }
  stop();
  return(0);
}

uint8_t I2CProtocol::read(int address, int registerAddress, int numberBytes)
{
  return(read((uint8_t) address, (uint8_t) registerAddress, (uint8_t) numberBytes));
}

uint8_t I2CProtocol::read(uint8_t address, uint8_t registerAddress, uint8_t numberBytes)
{
  bytesAvailable = 0;
  bufferIndex = 0;
  if(numberBytes == 0){numberBytes++;}
  nack = numberBytes - 1;
  returnStatus = 0;
  returnStatus = start();
  if(returnStatus){return(returnStatus);}
  returnStatus = sendAddress(SLA_W(address));
  if(returnStatus){return(returnStatus);}
  returnStatus = sendByte(registerAddress);
  if(returnStatus){return(returnStatus);}
  returnStatus = start();
  if(returnStatus){return(returnStatus);}
  returnStatus = sendAddress(SLA_R(address));
  if(returnStatus){return(returnStatus);}
  for(uint8_t i = 0; i < numberBytes; i++)
  {
    if( i == nack )
    {
      returnStatus = receiveByte(0);
      if(returnStatus != MR_DATA_NACK){return(returnStatus);}
    }
    else
    {
      returnStatus = receiveByte(1);
      if(returnStatus != MR_DATA_ACK){return(returnStatus);}
    }
    data[i] = TWDR;
    bytesAvailable = i+1;
    totalBytes = i+1;
  }
  stop();
  return(0);
}

uint8_t I2CProtocol::read(uint8_t address, uint8_t numberBytes, uint8_t *dataBuffer)
{
  bytesAvailable = 0;
  bufferIndex = 0;
  if(numberBytes == 0){numberBytes++;}
  nack = numberBytes - 1;
  returnStatus = 0;
  returnStatus = start();
  if(returnStatus){return(returnStatus);}
  returnStatus = sendAddress(SLA_R(address));
  if(returnStatus){return(returnStatus);}
  for(uint8_t i = 0; i < numberBytes; i++)
  {
    if( i == nack )
    {
      returnStatus = receiveByte(0);
      if(returnStatus != MR_DATA_NACK){return(returnStatus);}
    }
    else
    {
      returnStatus = receiveByte(1);
      if(returnStatus != MR_DATA_ACK){return(returnStatus);}
    }
    dataBuffer[i] = TWDR;
    bytesAvailable = i+1;
    totalBytes = i+1;
  }
  stop();
  return(0);
}

uint8_t I2CProtocol::read(uint8_t address, uint8_t registerAddress, uint8_t numberBytes, uint8_t *dataBuffer)
{
  bytesAvailable = 0;
  bufferIndex = 0;
  if(numberBytes == 0){numberBytes++;}
  nack = numberBytes - 1;
  returnStatus = 0;
  returnStatus = start();
  if(returnStatus){return(returnStatus);}
  returnStatus = sendAddress(SLA_W(address));
  if(returnStatus){return(returnStatus);}
  returnStatus = sendByte(registerAddress);
  if(returnStatus){return(returnStatus);}
  returnStatus = start();
  if(returnStatus){return(returnStatus);}
  returnStatus = sendAddress(SLA_R(address));
  if(returnStatus){return(returnStatus);}
  for(uint8_t i = 0; i < numberBytes; i++)
  {
    if( i == nack )
    {
      returnStatus = receiveByte(0);
      if(returnStatus != MR_DATA_NACK){return(returnStatus);}
    }
    else
    {
      returnStatus = receiveByte(1);
      if(returnStatus != MR_DATA_ACK){return(returnStatus);}
    }
    dataBuffer[i] = TWDR;
    bytesAvailable = i+1;
    totalBytes = i+1;
  }
  stop();
  return(0);
}


/////////////// Private Methods ////////////////////////////////////////


uint8_t I2CProtocol::start()
{
  TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
  while (!(TWCR & (1<<TWINT))) {

  }
  if ((TWI_STATUS == START) || (TWI_STATUS == REPEATED_START))
  {
    return(0);
  }
  return(TWI_STATUS);
}

uint8_t I2CProtocol::sendAddress(uint8_t i2cAddress)
{
  TWDR = i2cAddress;
  TWCR = (1<<TWINT) | (1<<TWEN);
  while (!(TWCR & (1<<TWINT))) {

  }
  if ((TWI_STATUS == MT_SLA_ACK) || (TWI_STATUS == MR_SLA_ACK))
  {
    return(0);
  }
  return(TWI_STATUS);
}

uint8_t I2CProtocol::sendByte(uint8_t i2cData)
{
  TWDR = i2cData;
  TWCR = (1<<TWINT) | (1<<TWEN);
  while (!(TWCR & (1<<TWINT))) {

  }
  if (TWI_STATUS == MT_DATA_ACK)
  {
    return(0);
  }
  return(TWI_STATUS);
}

uint8_t I2CProtocol::receiveByte(boolean ack)
{
  if(ack)
  {
    TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);

  }
  else
  {
    TWCR = (1<<TWINT) | (1<<TWEN);
  }
  while (!(TWCR & (1<<TWINT))) {

  }
  return(TWI_STATUS);
}

void I2CProtocol::stop()
{
  TWCR = (1<<TWINT)|(1<<TWEN)| (1<<TWSTO);
  while ((TWCR & (1<<TWSTO))) {

  }
}



SIGNAL(TWI_vect)
{
  switch(TWI_STATUS){
    case 0x20:
    case 0x30:
    case 0x48:
         TWCR = (1<<TWINT)|(1<<TWEN)| (1<<TWSTO); // send a stop
	 break;
    case 0x38:
    case 0x68:
    case 0x78:
    case 0xB0:
         TWCR = 0; //releases SDA and SCL lines to high impedance
  	 TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA); //reinitialize TWI
	 break;
  }
}


I2CProtocol i2cProtocol = I2CProtocol();


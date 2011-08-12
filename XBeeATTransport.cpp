#include <WProgram.h>
#include "XBeeATTransport.h"

namespace naxsoft {

    //XBeeATTransport::XBeeATTransport() {
    //
    //};

    //XBeeATTransport::~XBeeATTransport() {
    //  close();
    //};


    //void* XBeeATTransport::operator new(size_t size) { return malloc(size); };
    //void XBeeATTransport::operator delete(void* ptr) { free(ptr); };



    void XBeeATTransport::open(void) {
      Serial.begin(9600);	// opens serial port, sets data rate to 9600 bps
      m_isOpen = true;
    }

    void XBeeATTransport::close(void) {
      Serial.end();
      m_isOpen = false;
    }

    bool XBeeATTransport::isOpen(void) const {
          return m_isOpen;
    }

    void XBeeATTransport::flush() const {
      Serial.flush();
    }

    bool XBeeATTransport::peek() const {
       return Serial.available() > 0;
    }


    uint32_t XBeeATTransport::read(uint8_t* buf, uint32_t len) const {
      uint32_t i = 0;
      while(Serial.available() && i < len) {
         buf[i] = Serial.read();
    /*
          Serial.print("i = ");
          Serial.print(i);
          Serial.print(" len = ");
          Serial.print(len);
          Serial.print(" ch = ");
          Serial.println(buf[i]);
    */
         i++;
      }
      return i;
    }

    void XBeeATTransport::write(const uint8_t* buf, uint32_t len) const
    {
      Serial.write(buf, len);
    }

    uint32_t XBeeATTransport::readAll(uint8_t* buf, uint32_t len) const
    {
      uint32_t have = 0;
      uint32_t get = 0;

      while (have < len) {
        get = read(buf+have, len-have);
        have += get;
      }

    //  Serial.print("readAll have: ");
    //  Serial.println(have);
      return have;
    }
} // namespace

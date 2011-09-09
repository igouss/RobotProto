#ifndef PROCESSOR_H_
#define PROCESSOR_H_

#include "Thrift/Protocol.h"


namespace naxsoft {
    class Protocol; // Forward declaration

    class Processor {
      public:
        boolean process(Protocol const* proto);
      private:
        boolean processDcMotor(Protocol  const* proto);
        boolean tlc(Protocol  const* proto);
    	boolean accelerometer(Protocol const* proto);
    	boolean compass(Protocol const* proto);
    	boolean gyroscope(Protocol const* proto);
    	boolean processFunctionCall(int8_t rpc, Protocol const * const  proto);
    };

    extern Processor processor;
}

#endif /* PROCESSOR_H_ */

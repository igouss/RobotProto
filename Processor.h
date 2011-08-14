#ifndef PROCESSOR_H_
#define PROCESSOR_H_

namespace naxsoft {
    class Protocol; // Forward declaration

    class Processor {
      public:
        boolean process(Protocol const* in, Protocol  const* out);
      private:
        boolean processDcMotor(Protocol  const* in, Protocol const* const out);
        boolean tlc(Protocol  const* in, Protocol const* const out);
    	boolean accelerometer(Protocol const* in, Protocol const* const out);
    	boolean compass(Protocol const* in, Protocol const* const out);
    	boolean gyroscope(Protocol const* in, Protocol const* const out);
    	boolean processSyncFunctionCall(int8_t rpc, Protocol const* in, Protocol const* const out);
    	boolean processASyncFunctionCall(int8_t rpc, Protocol const* in, Protocol const* const out);
    };

    extern Processor processor;
}

#endif /* PROCESSOR_H_ */

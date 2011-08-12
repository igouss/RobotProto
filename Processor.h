#ifndef PROCESSOR_H_
#define PROCESSOR_H_

namespace naxsoft {
    class Protocol; // Forward declaration

    class Processor {
      public:
        boolean process(Protocol const* in, Protocol  const* out);
      private:
        boolean processDcMotor(Protocol  const* in);
        boolean tlc(Protocol  const* in);
    	boolean accelerometer(Protocol const* in);
    	boolean compass(Protocol const* in);
    	boolean gyroscope(Protocol const* in);
    };

    extern Processor processor;
}

#endif /* PROCESSOR_H_ */

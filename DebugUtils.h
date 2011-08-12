#ifndef DEBUGUTILS_H_
#define DEBUGUTILS_H_


#ifdef DEBUG
		#include <WProgram.h>

		#define VDEBUG_PRINT(str)    \
			Serial.print(millis());     \
			Serial.print(": ");    \
			Serial.print(__PRETTY_FUNCTION__); \
			Serial.print(' ');      \
			Serial.print(__FILE__);     \
			Serial.print(':');      \
			Serial.print(__LINE__);     \
			Serial.print(' ');      \
			Serial.println(str);

		#define VDEBUG_PRINTDEC(val)    \
			Serial.print(millis());     \
			Serial.print(": ");    \
			Serial.print(__PRETTY_FUNCTION__); \
			Serial.print(' ');      \
			Serial.print(__FILE__);     \
			Serial.print(':');      \
			Serial.print(__LINE__);     \
			Serial.print(' ');      \
			Serial.println(val, DEC);

		#define DEBUG_PRINT(x)     Serial.print (x)
		#define DEBUG_PRINTDEC(x)     Serial.print (x, DEC)
		#define DEBUG_PRINTLN(x)  Serial.println (x)
#else
		#define VDEBUG_PRINT(str)
		#define VDEBUG_PRINTDEC(val)
		#define DEBUG_PRINT(x)
		#define DEBUG_PRINTDEC(x)
		#define DEBUG_PRINTLN(x)
#endif

/*
  blinks an LED
 */
void blink(int whatPin, int howManyTimes, int milliSecs) {
  for (int i = 0; i < howManyTimes; i++) {
    digitalWrite(whatPin, HIGH);
    delay(milliSecs);
    digitalWrite(whatPin, LOW);
    delay(milliSecs);
  }
}


#endif /* DEBUGUTILS_H_ */

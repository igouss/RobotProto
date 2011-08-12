#define STRLEN(s) (sizeof(s) / sizeof(*s) - 1)

#include "WProgram.h"
// build process http://www.arduino.cc/en/Hacking/BuildProcess
#include <stdlib.h>

#include <stdio.h>
#include <inttypes.h>

#include <Wire.h>

//#include "EEPROM/EEPROM.h"
#include "Tlc5940/Tlc5940.h"
#include "Tlc5940/tlc_animations.h"
#include "Tlc5940/tlc_config.h"
#include "Tlc5940/tlc_fades.h"
#include "Tlc5940/tlc_progmem_utils.h"
#include "Tlc5940/tlc_servos.h"
#include "Tlc5940/tlc_shifts.h"

#include "DcMotor.h"
#include "XBeeATTransport.h"
#include "Protocol.h"
#include "Processor.h"
#include "MemoryTest.h"

//#include "LSM303DLH/LSM303DLH.h"

#include "Accelermeter/Accelerometer.h"
#include "Compass/Compass.h"
#include "Gyroscope/Gyroscope.h"

// LSM303DLH compass;

//#include "TimerOne.h"

#define DEBUG 1
#include "DebugUtils.h"

#define ZIGBE_PAN_ID 0xCAFE

#define ZIGBEE_MY_ADDR 0xDEAD
#define ZIGBE_MY_SERIAL_HIGH 0x13A200
#define ZIGBE_MY_SERIAL_LOW 0x40536AB1

#define ZIGBEE_REMOTE_ADDR 0xBEEF
#define ZIGBE_REMOTE_SERIAL_HIGH 0x13A200
#define ZIGBE_REMOTE_SERIAL_LOW 0x404B2098

#define FORWARD_PIN 4
#define REWERSE_PIN 3
#define ENABLE_PIN 2

/*
 * Digital pins (0-13) are accessed using digitalRead(), digitalWrite(), and analogWrite().
 * The analog pins (0-5) are accessed only using analogRead().
 *
 * DigitalRead/DigitalWrite applies to 0-19 (20 I/O lines), PinMode is explicitly required for these calls.
 * AnalogRead applies to 0-5 (Which would be digital 14-19), no PinMode call required.
 * AnalogWrite applies to 3,5,6,9,10,11, (same mapping as the digital pins.) no PinMode call required.
 * Using analogWrite on any of the other digital pins will digitalWrite a 1 if >127 or a 0 if <= 127, and
 * the PinMode is still done for you.
 */

#define motor_direction byte

#define LED_1 13
#define max_pwm 1000 //this controls the period of one complete cycle
#define PMEM_SPEED 0

/*
 static char pwm_counter; //used to count from 0 to max_pwm
 static int motor1Dir = MOTOR_CW;
 static double K = 0.5;
 static double mu_n = 0;
 static double sigma_sqr_n = 0;
 static unsigned int old_pwm = 0;
 static unsigned int pwm = 0;
 */
static const naxsoft::XBeeATTransport transport;
static const naxsoft::Protocol proto(&transport);
/*
Accelerometer accelerometer;
Compass compass;
Gyroscope gyro;
accelerometer_data adata;
compass_data cdata;
gyroscope_data gdata;
*/

extern "C" void __cxa_pure_virtual() {
	cli();
	for (;;) {

	}
}

__extension__ typedef int __guard __attribute__((mode (__DI__)));

void * operator new(size_t size) {
	return malloc(size);
}

void operator delete(void * ptr) {
	free(ptr);
}

void * operator new[](size_t size) {
	return malloc(size);
}

void operator delete[](void * ptr) {
	if (ptr)
		free(ptr);
}

int __cxa_guard_acquire(__guard *g) {
	return !*(char *) (g);
}
void __cxa_guard_release(__guard *g) {
	*(char *) g = 1;
}
void __cxa_guard_abort(__guard *) {
}


void setup() {
	pinMode(13, OUTPUT);
	//  digitalWrite(FORWARD_PIN, HIGH); //Activate the pull up resistor
	pinMode(FORWARD_PIN, OUTPUT);
	pinMode(REWERSE_PIN, OUTPUT);
	pinMode(ENABLE_PIN, OUTPUT);

	pinMode(FORWARD_PWM_PIN, OUTPUT);
	pinMode(REWERSE_PWM_PIN, OUTPUT);
	pinMode(ENABLE_PWM_PIN, OUTPUT);

	Tlc.init();
	Wire.begin();

	Serial.println("starting up gyroscope");
	naxsoft::gyroscope.setupL3G4200D(250); // Configure L3G4200  - 250, 500 or 2000 deg/sec
	Serial.println("starting up accelerometer");
	naxsoft::accelerometer.init();
	Serial.println("starting up compass");
	naxsoft::compass.init();

	// delay(1500); //wait for the sensor to be ready

	//  pinMode(10, OUTPUT);
	//  Timer1.initialize(500000);         // initialize timer1, and set a 1/2 second period
	//  Timer1.pwm(9, 512);                // setup pwm on pin 9, 50% duty cycle
	//  Timer1.attachInterrupt(timer1);  // attaches callback() as a timer overflow interrupt

	//  Wire.begin(); // join the bus as a master.
	//  Wire.onReceive(wireReceiveHandler);
	//  Wire.onRequest(wireRequestHandler);

	/*
	 mode defines when the interrupt should be triggered. Four contstants are predefined as valid values:
	 LOW to trigger the interrupt whenever the pin is low,
	 CHANGE to trigger the interrupt whenever the pin changes value
	 RISING to trigger when the pin goes from low to high,
	 FALLING for when the pin goes from high to low.
	 */
	//  attachInterrupt(0, isr0, CHANGE); // D2
	//  attachInterrupt(1, isr1, CHANGE); // D3

	// http://tronixstuff.wordpress.com/2010/12/07/tutorial-arduino-and-the-aref-pin/
	analogReference(EXTERNAL); // use AREF for reference voltage

	//  blink(LED_1, 3, 500);
	Serial.begin(9600);

	int freeMem = naxsoft::MemoryTest::availableMemory();

	//   transport = new XBeeATTransport();
	//   proto = new Protocol(transport);

	//   proto->writeI8(8);

	Serial.println();
	Serial.println();
	Serial.println("CPU: ATmega168");
	Serial.print("Frequency (MHz): ");
	Serial.println(F_CPU / 1000000.0);
	Serial.print("Free mem (bytes): ");
	Serial.println(freeMem);
	//  byte oldSpeed =  EEPROM.read(PMEM_SPEED);
	naxsoft::DcMotor::motor_move_pwm(MOTOR_COAST, 0);
	//   motor_move_pwm(MOTOR_CW, 128);
}

int main(void) {

	/* Must call init for arduino to work properly */
	init();
	setup();

	/****************************/
	/* Add your setup code here */
	/****************************/

	for (;;) {
		loop();
	} // end for
} // end main


void loop() {
	// analogRead values go from 0 to 1023, analogWrite values from 0 to 128


	// Turn on first len      ...    Turn on second led
	// 04 04 01 80   ...             04 04 02 80
	// --                             | --> protocol::readMessageBegin:  messageType
	//    __                          | --> Processor::process: rpc
	//       --                       | --> Processor::tlc: chan
	//          __                    | --> Processor::tlc: value
	// 04 05
	// --                             | --> protocol::readMessageBegin:  messageType
	//    __                          | --> Processor::accelerometer
	// 04 06
	// --                             | --> protocol::readMessageBegin:  messageType
	//    __                          | --> Processor::compass
	// 04 07
	// --                             | --> protocol::readMessageBegin:  messageType
	//    __                          | --> Processor::gyroscope


//	  compass.read();
//	  delay(50);
//	  delay(50);
//	  delay(50);
//	  Serial.println();
//	  Serial.println();
//	  delay(850); //Just here to slow down the serial to make it more readable

	if (transport.peek() == true) {
		naxsoft::processor.process(&proto, &proto);
	}

	/*
	 if(Serial.available()) {
	 unsigned int x_n_plus = (double) serReadInt();

	 estimate(x_n_plus, K, &mu_n, &sigma_sqr_n);
	 Serial.print("mu = ");
	 Serial.println(mu_n);
	 Serial.print("sigma square= ");
	 Serial.println(sigma_sqr_n);
	 }
	 */

	/*
	 if(Serial.available()) {


	 //         Serial.println("Reading serial");

	 //         old_pwm = pwm;
	 //         transport.readAll( (uint8_t* ) &pwm,  1);
	 pwm = Serial.read();
	 Serial.println(pwm);

	 //pwm = serReadInt();

	 if(pwm > 2) {
	 pwm = map(pwm, 3, 10, 64, 255);
	 }


	 //         EEPROM.write(PMEM_SPEED, pwm);

	 if(pwm == 0) {
	 digitalWrite(LED_1, LOW);
	 motor_move_pwm(MOTOR_COAST, pwm);
	 } else if(pwm == 1) {
	 digitalWrite(LED_1, LOW);
	 if(old_pwm != 0) {
	 motor_move_pwm(motor1Dir, 64);
	 delay(200);
	 }
	 motor_move_pwm(MOTOR_STOP, pwm);
	 } else if(pwm == 2) {
	 if(motor1Dir == MOTOR_CW) {
	 motor1Dir = MOTOR_CCW;
	 } else {
	 motor1Dir = MOTOR_CW;
	 }
	 } else {
	 digitalWrite(LED_1, HIGH);
	 // motor_move_pwm(motor1Dir, pwm);
	 ease_pwm_motor(old_pwm, pwm, motor1Dir);
	 }
	 }
	 */
	Tlc.update();
}

// called when a slave device receives a transmission from a master.
void wireReceiveHandler(int numBytes) {
	(void) numBytes;
	Serial.println("wireReceiveHandler");
}

//  called when a master requests data from this slave device.
void wireRequestHandler() {
	Serial.println("wireRequestHandler");
}

void timer1() {
	//  blink(LED_1, 1, 500);
}

// All variabled midified in ISR must be declared volatile
void isr0() {
	Serial.println("ISR0");
}

// All variabled midified in ISR must be declared volatile
void isr1() {
	Serial.println("ISR1");
}

/*

 static int serReadInt()
 {
 int i, serAva;                           // i is a counter, serAva hold number of serial available
 char inputBytes [7];                 // Array hold input bytes
 char * inputBytesPtr = &inputBytes[0];  // Pointer to the first element of the array

 if (Serial.available()>0)            // Check to see if there are any serial input
 {
 delay(5);                              // Delay for terminal to finish transmitted
 // 5mS work great for 9600 baud (increase this number for slower baud)
 serAva = Serial.available();  // Read number of input bytes
 for (i=0; i<serAva; i++)       // Load input bytes into array
 inputBytes[i] = Serial.read();
 inputBytes[i] =  '\0';             // Put NULL character at the end
 return atoi(inputBytesPtr);    // Call atoi function and return result
 }
 else
 return -1;                           // Return -1 if there is no input
 }
 */
/*
 static void bridge_test(void) {
 digitalWrite(LED_1, HIGH);   // set the LED on
 motor_move(MOTOR_CCW);
 delay(5000);              // wait for a second

 digitalWrite(LED_1, LOW);   // set the LED on
 motor_move(MOTOR_STOP);
 delay(500);              // wait for a second

 digitalWrite(LED_1, HIGH);   // set the LED off
 motor_move(MOTOR_COAST);
 delay(1000);              // wait for a second


 digitalWrite(LED_1, LOW);   // set the LED off
 motor_move(MOTOR_CCW);
 delay(5000);              // wait for a second

 digitalWrite(LED_1, HIGH);   // set the LED off
 motor_move(MOTOR_STOP);
 delay(500);              // wait for a second

 digitalWrite(LED_1, LOW);   // set the LED on
 motor_move(MOTOR_COAST);
 delay(1000);              // wait for a second
 }
 */

/*
 static void manual_pwm(int pwm) {
 pwm_counter++;
 if (pwm_counter > max_pwm) {
 pwm_counter = 0;
 }
 if (pwm_counter < pwm){ //ON cycle
 motor_move(MOTOR_CW); //turn clockwise
 }else{ //OFF cycle
 motor_move(MOTOR_COAST); //motor freewheeling
 }
 }
 */

/*
 static void motor_move(motor_direction dir) {
 switch(dir) {
 case MOTOR_CW:
 digitalWrite(FORWARD_PIN, HIGH);
 digitalWrite(REWERSE_PIN, LOW);
 digitalWrite(ENABLE_PIN, LOW);
 break;
 case MOTOR_CCW:
 digitalWrite(FORWARD_PIN, LOW);
 digitalWrite(REWERSE_PIN, HIGH);
 digitalWrite(ENABLE_PIN, LOW);
 break;
 case MOTOR_STOP:
 digitalWrite(FORWARD_PIN, HIGH);
 digitalWrite(REWERSE_PIN, HIGH);
 digitalWrite(ENABLE_PIN, LOW);
 break;
 case MOTOR_COAST:
 digitalWrite(FORWARD_PWM_PIN, LOW);
 digitalWrite(REWERSE_PWM_PIN, LOW);
 digitalWrite(ENABLE_PIN, LOW);
 break;
 }
 }
 */

/*
 //This function will write a 2 byte integer to the eeprom at the specified address and address + 1
 void EEPROMWriteInt(int p_address, int p_value)
 {
 byte lowByte = ((p_value >> 0) & 0xFF);
 byte highByte = ((p_value >> 8) & 0xFF);

 EEPROM.write(p_address, lowByte);
 EEPROM.write(p_address + 1, highByte);
 }

 //This function will read a 2 byte integer from the eeprom at the specified address and address + 1
 unsigned int EEPROMReadInt(int p_address)
 {
 byte lowByte = EEPROM.read(p_address);
 byte highByte = EEPROM.read(p_address + 1);

 return ((lowByte << 0) & 0xFF) + ((highByte << 8) & 0xFF00);
 }
 */

/*
 static boolean debounce(boolean last, int pin) {
 boolean current = digitalRead(pin);
 if(current != last) {
 delay(5); // ms
 current = digitalRead(pin);
 }
 return current;
 }
 */

/*
 int getPot(int pot) {
 int v;
 v = analogRead(pot);
 v /= 4;
 v = max(v, 90);
 v = min(v, 255);
 return v;
 }
 */

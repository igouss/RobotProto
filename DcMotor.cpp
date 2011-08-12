#include <float.h>

#include <WConstants.h>
#include <HardwareSerial.h>

#include "DcMotor.h"
#include "Estimator.h"

namespace naxsoft {
    #define MOTOR0_FORWARD_PWM_PIN 10 // A4
    #define MOTOR0_REWERSE_PWM_PIN 9 // A3
    #define MOTOR0_ENABLE_PWM_PIN 8 // A2


    DcMotor dcMotor;              // preinstatiate

    void DcMotor::start(uint8_t motor, uint8_t pwm, uint8_t direction) {
    	(void) motor;
    	(void) pwm;
    	(void) direction;
    }

    void DcMotor::stop(uint8_t motor) {
    	(void) motor;
    }

    void DcMotor::coast(uint8_t motor) {
    	(void) motor;
    }

    void DcMotor::ease_pwm_motor(unsigned int start_pwm, unsigned int end_pwm, int dir) {

      if(start_pwm < 64) {
         start_pwm = 64;
      }

      double mu_n = start_pwm;
      double sigma_sqr_n = DBL_MAX;
      for(int i = 0; i < 100; i++) {
            Estimator::estimate(end_pwm, 0.1, &mu_n, &sigma_sqr_n);
            if(((int)mu_n == (int)end_pwm) || ((int)mu_n == (int)(end_pwm - 1)) || ((int)mu_n == (int)(end_pwm + 1))) {
               break;
            }
            motor_move_pwm(dir, mu_n);
            delay(50);
      }
      motor_move_pwm(dir, end_pwm);
    }

    void DcMotor::motor_move_pwm(int dir, unsigned int pwmSignal) {
      switch(dir) {
          case MOTOR_CW:
             Serial.print("MOTOR CW - ");
             Serial.println(pwmSignal);
            analogWrite(FORWARD_PWM_PIN, pwmSignal);
            analogWrite(REWERSE_PWM_PIN, 0);
            analogWrite(ENABLE_PWM_PIN, 0);
            break;
          case MOTOR_CCW:
             Serial.print("MOTOR CCW - ");
             Serial.println(pwmSignal);
            analogWrite(FORWARD_PWM_PIN, 0);
            analogWrite(REWERSE_PWM_PIN, pwmSignal);
            analogWrite(ENABLE_PWM_PIN, 0);
            break;
          case MOTOR_STOP:
             Serial.println("MOTOR STOP ");
            analogWrite(FORWARD_PWM_PIN, 255);
            analogWrite(REWERSE_PWM_PIN, 255);
            analogWrite(ENABLE_PWM_PIN, 0);
            break;
          case MOTOR_COAST:
             Serial.println("MOTOR COAST");
            analogWrite(FORWARD_PWM_PIN, 0);
            analogWrite(REWERSE_PWM_PIN, 0);
            analogWrite(ENABLE_PWM_PIN, 0);
            break;
      }
      return;
    }

}

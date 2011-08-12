#ifndef DCMOTOR_H_
#define DCMOTOR_H_

#include <WConstants.h>

namespace naxsoft {


    #define MOTOR_CW 0
    #define MOTOR_CCW 1
    #define MOTOR_STOP 2
    #define MOTOR_COAST 3
    #define MOTOR_CW_PWM 4
    #define MOTOR_CCW_PWM 5
    #define MOTOR_STOP_PWM 6

    #define FORWARD_PWM_PIN 10 // A4
    #define REWERSE_PWM_PIN 9 // A3
    #define ENABLE_PWM_PIN 8 // A2

    class DcMotor {
        public:
          void start(uint8_t motor, uint8_t pwm, uint8_t direction);
          void stop(uint8_t motor);
          void coast(uint8_t motor);

          static void ease_pwm_motor(unsigned int start_pwm, unsigned int end_pwm, int dir);
          static void motor_move_pwm(int dir, unsigned int pwmSignal);
    };

    extern DcMotor dcMotor;
}

#endif /* DCMOTOR_H_ */

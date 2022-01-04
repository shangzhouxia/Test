#include <stdio.h>
#include "motor_data.h"
#include "motor_proxy.h"

int main(int argc, char const *argv[]) {

    motor_proxy_t* motor1 = motor_create();
    
/*     union motor_data_test data_test;
    data_test.reg = 0xAAAA;

    motor_data_t data_rel;
    data_rel.reg = 0xFFFFFFFF; */

    unsigned int data;

    if (NULL != motor1) {
        motor_configure(motor1, 10, &data);
    }

    motor_enable(motor1);
    motor_set_direction(motor1, FORWARD);

    motor_set_motor_speed(motor1, 45);

    printf("Motor encoded data: 0x%x\n", data);
    printf("Motor speed: %d\n", motor_get_speed(motor1));
    printf("Motor: direction: %d\n", motor_get_direction(motor1));

    return 0;
}
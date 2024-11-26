#ifndef DRIVER_HPP
#define DRIVER_HPP


#include <Arduino.h>
#include <Servo.h>


class Driver {
public:
    Driver(int left_pin, int right_pin, int left_dir_pin, int right_dir_pin, uint8_t servo_arm_pin, uint8_t servo_hand_pin, int motor_speed = 250); 
    ~Driver();
    
    void set_motors(const int velo_left, const int velo_right);
    void stop_motors();
    void arm(const int power);
    void hand(const int power);
    void get_command_wheels();
    void get_command_arm();

private:
    Servo servo_arm;
    Servo servo_hand;
    int pos_servo_arm;
    int pos_servo_hand;

    int pin_motor_left;
    int pin_motor_right;
    int pin_motor_dir_left;
    int pin_motor_dir_right;
    int speed;
    uint8_t pin_servo_arm;
    uint8_t pin_servo_hand;
};


#endif // DRIVER_HPP
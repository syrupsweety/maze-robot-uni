#include "driver.hpp"


Driver::Driver(int left_pin, int right_pin, int left_dir_pin, int right_dir_pin, uint8_t servo_arm_pin, uint8_t servo_hand_pin, int motor_speed) 
    : pin_motor_left(left_pin), pin_motor_right(right_pin), pin_motor_dir_left(left_dir_pin), pin_motor_dir_right(right_dir_pin), speed(motor_speed), pin_servo_arm(servo_arm_pin), pin_servo_hand(servo_hand_pin)
{
    pinMode(pin_motor_left, OUTPUT);
    pinMode(pin_motor_right, OUTPUT);
    pinMode(pin_motor_dir_left, OUTPUT);
    pinMode(pin_motor_dir_right, OUTPUT);

    servo_arm.attach(pin_servo_arm);
    servo_hand.attach(pin_servo_hand);
    pos_servo_arm = 0;
    pos_servo_hand = 0;

    set_motors(0, 0);
    servo_arm.write(0);
    servo_hand.write(150);
}


Driver::~Driver()
{
    set_motors(0, 0);
    servo_arm.write(0);
    servo_hand.write(150);
}


void Driver::set_motors(const int velo_left, const int velo_right)
{
    int motor_dir_left = (velo_left >= 0) ? HIGH : LOW;
    int motor_dir_right = (velo_right >= 0) ? HIGH : LOW;

    digitalWrite(pin_motor_dir_left, motor_dir_left);
    digitalWrite(pin_motor_dir_right, motor_dir_right);

    // Убедись, что скорость ограничена от 0 до 255
    analogWrite(pin_motor_left, constrain(abs(velo_left), 0, 255));
    analogWrite(pin_motor_right, constrain(abs(velo_right), 0, 255));
}


void Driver::stop_motors()
{
    set_motors(0, 0);
}


void Driver::get_command_wheels()
{
    int msg;

    if (Serial.available() > 0) {
        msg = Serial.read() - '0';  // Преобразуем символ в число
        
        /* // Отправляем обратно команду
        Serial.print("Получена команда руки: ");
        Serial.println(msg); */
    } else {
        stop_motors();
        return;
    }

    switch (msg) {
        case 1:
            set_motors(speed, speed);  // Вперед
            break;
        case 2:
            set_motors(-speed, -speed);  // Назад
            break;
        case 3:
            set_motors(speed, -speed);  // Поворот вправо
            break;
        case 4:
            set_motors(-speed, speed);  // Поворот влево
            break;
        default:
            stop_motors();  // Если команда неизвестна, останавливаем моторы
            break;
    }

    delay(10);
}


void Driver::arm(const int power)
{
    pos_servo_arm = constrain(pos_servo_arm + ((power > 0) ? 5 : -5), 0, 45);
    servo_arm.write(pos_servo_arm);
}


void Driver::hand(const int power)
{
    if (power > 0) servo_hand.write(10);
    
    if (power < 0) servo_hand.write(150);
}


void Driver::get_command_arm()
{
    int msg;

    if (Serial.available() > 0) {
        msg = Serial.read() - '0';  // Преобразуем символ в число

        /* // Отправляем обратно команду
        Serial.print("Получена команда руки: ");
        Serial.println(msg); */
    } else {
        return;
    }

    switch (msg) {
        case 5:
            arm(100);  // Вверх
            break;
        case 6:
            arm(-100);  // Вниз
            break;
        case 7:
            hand(100);  // Сжать
            break;
        case 8:
            hand(-100);  // Разжать
            break;
        default:
            break;
    }

    delay(10);
}
#ifndef DRIVER_HPP
#define DRIVER_HPP

#include <Arduino.h>
#include <DHT.h>

// Определение пинов для инфракрасного датчика
#define IR_PIN A0  // Пин для инфракрасного датчика

// Определение пинов для датчика влажности (например, DHT11)
#define DHT_PIN 2
#define DHT_TYPE DHT11  // Или DHT22, в зависимости от датчика

class Driver {
public:
    Driver(int left_pin, int right_pin, int left_dir_pin, int right_dir_pin, int motor_speed = 120); 
    ~Driver();
    
    void set_motors(const int velo_left, const int velo_right);
    void stop_motors();
    void get_command_wheels();
    void check_distance();   // Проверка расстояния с инфракрасным датчиком
    void read_humidity();    // Чтение данных с датчика влажности
    void perform_action(int action);  // Новый метод для выполнения действий
    void check_action_progress();  // Новый метод для проверки прогресса действий

private:
    int pin_motor_left;
    int pin_motor_right;
    int pin_motor_dir_left;
    int pin_motor_dir_right;
    int speed;
    unsigned long action_start_time;  // Время начала действия
    bool action_in_progress;          // Флаг, указывающий на выполнение действия
    int performing_action;            // Текущее выполняемое действие

    DHT dht;          // Объект датчика влажности
};

#endif // DRIVER_HPP
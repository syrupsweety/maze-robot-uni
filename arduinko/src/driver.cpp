#include "driver.hpp"
#include <math.h>  // Для логарифмических операций

// Инициализация инфракрасного датчика и датчика влажности
Driver::Driver(int left_pin, int right_pin, int left_dir_pin, int right_dir_pin, int motor_speed)
    : pin_motor_left(left_pin), pin_motor_right(right_pin), pin_motor_dir_left(left_dir_pin),
      pin_motor_dir_right(right_dir_pin), speed(motor_speed), dht(DHT_PIN, DHT_TYPE),
      action_start_time(0), action_in_progress(false), performing_action(0) {

    pinMode(pin_motor_left, OUTPUT);
    pinMode(pin_motor_right, OUTPUT);
    pinMode(pin_motor_dir_left, OUTPUT);
    pinMode(pin_motor_dir_right, OUTPUT);

    set_motors(0, 0);
    dht.begin();  // Инициализация датчика влажности
}

Driver::~Driver() {
    set_motors(0, 0);
}

void Driver::set_motors(const int velo_left, const int velo_right) {
    int motor_dir_left = (velo_left >= 0) ? HIGH : LOW;
    int motor_dir_right = (velo_right >= 0) ? HIGH : LOW;

    digitalWrite(pin_motor_dir_left, motor_dir_left);
    digitalWrite(pin_motor_dir_right, motor_dir_right);

    analogWrite(pin_motor_left, constrain(abs(velo_left), 0, 255));
    analogWrite(pin_motor_right, constrain(abs(velo_right), 0, 255));
}

void Driver::stop_motors() {
    set_motors(0, 0);
    action_in_progress = false;  // Сбрасываем флаг действия
}

void Driver::check_distance() {
    // Измеряем расстояние с помощью инфракрасного датчика
    int sensorValue = analogRead(IR_PIN);
    // Применяем логарифмическую функцию для преобразования данных
    float distance = pow(10, log10(sensorValue / 1821.2) / -0.65);  // Пример формулы, может отличаться
    
    Serial.print("Считанное значение: ");
    Serial.print(sensorValue);
    Serial.print(" Преобразованное расстояние: ");
    Serial.println(distance);
    
    if (distance > 0 && distance < 40) {  // Если объект ближе 20 см
        stop_motors();  // Останавливаем моторы
        Serial.println("Объект слишком близко! Моторы остановлены.");
    }
}

void Driver::read_humidity() {
    float humidity = dht.readHumidity();  // Чтение влажности
    if (isnan(humidity)) {
        Serial.println("Ошибка чтения с датчика влажности.");
    } else {
        Serial.print("Влажность: ");
        Serial.print(humidity);
        Serial.println(" %");
    }
}

void Driver::get_command_wheels() {
    int msg;

    if (Serial.available() > 0) {
        msg = Serial.read() - '0';  // Преобразуем символ в число
        
        // Отправляем обратно команду
        Serial.print("Получена команда: ");
        Serial.println(msg); 

        switch (msg) {
            case 1:
                set_motors(speed, speed);  // Вперед
                break;
            case 2:
                set_motors(-speed, -speed);  // Назад
                break;
            case 3:
                set_motors(speed*0.9, -speed*0.9);  // Поворот вправо
                break;
            case 4:
                set_motors(-speed*0.9, speed*0.9);  // Поворот влево
                break;
            case 5:
                read_humidity();  // Отображаем данные с датчика влажности
                break;
            case 6:
                perform_action(6);  // Начать выполнение действия 6
                break;
            case 7:
                perform_action(7);  // Начать выполнение действия 7
                break;
            default:
                stop_motors();  // Если команда неизвестна, останавливаем моторы
                break;
        }
    }
}

void Driver::perform_action(int action) {
    if (action_in_progress) {
        return;  // Если действие уже выполняется, выходим
    }

    action_start_time = millis();  // Запоминаем время начала действия
    action_in_progress = true;   // Устанавливаем флаг действия
    performing_action = action;  // Запоминаем текущее действие

    switch (action) {
        case 6:
            // Действие 6: двигаться вперед на 50, поворот налево в течение 2 секунд, снова вперед на 50, остановиться
            set_motors(speed, speed);  // Двигаемся вперед
            break;
        case 7:
            // Действие 7: двигаться назад на 20, остановиться
            set_motors(-speed, -speed);  // Двигаемся назад
            break;
    }
}

void Driver::check_action_progress() {
    if (!action_in_progress) {
        return;  // Если действие не выполняется, выходим
    }

    unsigned long current_time = millis();
    unsigned long elapsed_time = current_time - action_start_time;

    switch (performing_action) {
        case 6:
            read_humidity();
            if (elapsed_time < 1000) {
                // Двигаемся вперед в течение 1 секунды
                set_motors(speed, speed);;
            } else if (elapsed_time < 3500) {
                // Поворот налево в течение 2 секунд
                set_motors(-speed, speed);;
            } else if (elapsed_time < 4500) {
                // Двигаемся вперед в течение 1 секунды
                set_motors(-speed, -speed);;
            } else {
                // Останавливаем моторы
                stop_motors();
            }
            break;
        case 7:
            if (elapsed_time < 1000) {
                // Двигаемся назад в течение 2 секунд
                set_motors(-speed, -speed);;
            } else {
                // Останавливаем моторы
                stop_motors();
            }
            break;
    }
}


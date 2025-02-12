#include "driver.hpp"

Driver Wheels(6, 5, 7, 4);  // Инициализация с пинами для моторов

void setup() {
    Serial.begin(9600);
    delay(100);
}

void loop() {
    Wheels.check_distance();  // Проверка расстояния независимо от команды
    Wheels.check_action_progress();  // Проверка прогресса действия

    if (Serial.available() > 0) {
        Wheels.get_command_wheels();
    }
    delay(40);
}
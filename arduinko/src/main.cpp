#include "driver.hpp"


Driver Wheels(6, 5, 7, 4, 10, 9);  // Инициализация с пинами для моторов


void setup() {
    Serial.begin(9600);
    delay(100);
}


void loop() {
    if (Serial.available() > 0) {
        Wheels.get_command_arm();
        Wheels.get_command_wheels();
    }

    delay(40);
}
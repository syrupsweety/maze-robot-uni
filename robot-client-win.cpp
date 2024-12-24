#include <iostream>
#include <cstring>
#include <unistd.h> // Для close() и usleep()
#include <arpa/inet.h> // Для sockaddr_in и inet_addr
#include <fcntl.h> // Для fcntl()
#include <termios.h> // Для терминала

#define SERVER_PORT 33033  // Порт для передачи данных

// Функция для проверки нажатия клавиши
int getch() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

int main() {
    char server_ip[16];
    std::cout << "Введите IP адрес сервера: ";
    std::cin >> server_ip;

    // Создание сокета
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        std::cerr << "Ошибка создания сокета" << std::endl;
        return -1;
    }

    // Настройка адреса сервера
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(server_ip);

    std::cout << "Нажимайте клавиши для отправки команд..." << std::endl;

    // Основной цикл обработки клавиш
    while (true) {
        int key = getch();  // Считываем клавишу

        char command = 0;  // Команда, которая будет отправлена
        switch (key) {
            case 'w': command = '1'; break;
            case 's': command = '2'; break;
            case 'd': command = '3'; break;
            case 'a': command = '4'; break;
            case '8': command = '5'; break;  // Вверх
            case '2': command = '6'; break;  // Вниз
            case '4': command = '7'; break;  // Влево
            case '6': command = '8'; break;  // Вправо
            case 'q':  // Выход из программы
                close(sock);
                return 0;
        }

        if (command != 0) {
            // Отправляем команду на сервер
            sendto(sock, &command, sizeof(command), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
        }
    }

    // Закрываем сокет и освобождаем ресурсы
    close(sock);

    return 0;
}

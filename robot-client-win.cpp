#include <iostream>
#include <cstring>
#include <unistd.h> // Для close()
#include <arpa/inet.h> // Для sockaddr_in и inet_addr
#include <fcntl.h> // Для fcntl()
#include <termios.h> // Для терминала

#define SERVER_PORT 33033  // Порт для передачи данных

// Функция для проверки нажатия клавиши
bool kbhit() {
    struct termios oldt, newt;
    int oldf;
    char ch;
    bool oldf2;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    oldf2 = false;
    if (read(STDIN_FILENO, &ch, 1) == 1) {
        oldf2 = true;
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    return oldf2;
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

    // Основной цикл обработки клавиш
    while (true) {
        if (kbhit()) {  // Проверяем, была ли нажата клавиша
            char key = getchar();  // Считываем клавишу

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
            }

            if (command != 0) {
                // Отправляем команду на сервер
                sendto(sock, &command, sizeof(command), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
            }
        }
        usleep(100000); // Задержка в 100 мс для предотвращения излишней загрузки процессора
    }

    // Закрываем сокет и освобождаем ресурсы
    close(sock);

    return 0;
}


// // #include <winsock2.h>
// // #include <ws2tcpip.h>
// #include <termios.h>
// #include <unistd.h>
// #include <stdio.h>
// #include <iostream>
// #include <netinet/in.h>
// #include <linux/ioctl.h>
// // #include <conio.h>  // Для работы с функцией _kbhit() и _getch()z
// // #include <curses.h>
//
//
// #pragma comment(lib, "ws2_32.lib")  // Подключение библиотеки Winsock
//
//
// // #define SERVER_IP "192.168.1.46"  // IP адрес Raspberry  указываем вручную в переменной server_ip
// #define SERVER_PORT 33033  // Порт для передачи данных
//
//
// int main() {
//     char server_ip[16];
//     std::cin>>server_ip;
//     // Инициализация Winsock
//     WSADATA wsaData;
//     if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
//         std::cerr << "Ошибка инициализации Winsock" << std::endl;
//         return -1;
//     }
//
//     // Создание сокета
//     SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
//     if (sock == INVALID_SOCKET) {
//         std::cerr << "Ошибка создания сокета" << std::endl;
//         WSACleanup();
//         return -1;
//     }
//
//     // Настройка адреса сервера
//     sockaddr_in serverAddr;
//     serverAddr.sin_family = AF_INET;
//     serverAddr.sin_port = htons(SERVER_PORT);
//     serverAddr.sin_addr.s_addr = in_addr(server_ip);
//
//     // Основной цикл обработки клавиш
//     while (true) {
//         if (_kbhit()) {  // Проверяем, была ли нажата клавиша
//             char key = _getch();  // Считываем клавишу
//
//             char command = 0;  // Команда, которая будет отправлена
//             switch (key) {
//                 case 'w': command = '1'; break;
//                 case 's': command = '2'; break;
//                 case 'd': command = '3'; break;
//                 case 'a': command = '4'; break;
//                 case 72: command = '5'; break;  // Вверх
//                 case 80: command = '6'; break;  // Вниз
//                 case 75: command = '7'; break;  // Влево
//                 case 77: command = '8'; break;  // Вправо
//             }
//
//             if (command != 0) {
//                 // Отправляем команду на сервер
//                 sendto(sock, &command, sizeof(command), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
//             }
//         }
//     }
//
//     // Закрываем сокет и освобождаем ресурсы
//     closesocket(sock);
//     WSACleanup();
//     return 0;
// }

#include "Communicate.h"
#include <openssl/md5.h>
#include <random>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <sstream>

// ... (другие include) ...

// Функция для разбиения строки по разделителю
std::vector<std::string> split(const std::string& str, char delim) {
    std::vector<std::string> parts;
    std::stringstream ss(str);
    std::string part;
    while (std::getline(ss, part, delim)) {
        parts.push_back(part);
    }
    return parts;
}

std::string Communicate::md5(std::string input_str) {
    unsigned char result[MD5_DIGEST_LENGTH];
    MD5((unsigned char*)input_str.c_str(), input_str.length(), result);
    std::stringstream ss;
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(result[i]);
    }
    return ss.str();
}

std::string Communicate::generate_salt() {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(0, 255);

    std::stringstream ss;
    for (int i = 0; i < 16; i++) { // Генерируем 16 байт соли
        ss << std::hex << std::setfill('0') << std::setw(2) << distribution(generator);
    }
    return ss.str();
}

int Communicate::connection(int port, std::map<std::string, std::string> database, Logger* l1) {
    // Создание сокета
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        l1->writelog("Error creating socket");
        return 1;
    }

    // Настройка адреса сервера
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Связывание сокета с адресом
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        l1->writelog("Error binding socket");
        close(server_socket);
        return 1;
    }

    // Прослушивание подключений
    if (listen(server_socket, 5) == -1) {
        l1->writelog("Error listening for connections");
        close(server_socket);
        return 1;
    }

    l1->writelog("Server started and listening on port " + std::to_string(port));

    while (true) {
        // Принимаем данные от клиента
        char buffer[1024];
        int client_socket = accept(server_socket, nullptr, nullptr);
        if (client_socket == -1) {
            // ... (обработка ошибки приема) ...
            l1->writelog("Error accepting connection");
            continue;
        }

        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            // ... (обработка разрыва соединения) ...
            l1->writelog("Connection closed by client");
            close(client_socket);
            continue;
        }

        std::string request = std::string(buffer, bytes_received);

        // Разбираем запрос
        std::vector<std::string> parts = split(request, ':');
        if (parts.size() != 3) {
            // ... (обработка некорректного запроса) ...
            l1->writelog("Invalid request format");
            close(client_socket);
            continue;
        }

        std::string username = parts[1];
        std::string hashed_password = parts[2];

        // Проверяем, есть ли пользователь в базе данных
        if (database.find(username) == database.end()) {
            send(client_socket, "auth_failed", strlen("auth_failed"), 0);
            l1->writelog("Authentication failed: User not found");
            close(client_socket);
            continue;
        }

        // Сравниваем хэшированный пароль
        std::string stored_password = database[username];
        if (hashed_password == stored_password) {
            send(client_socket, "auth_success", strlen("auth_success"), 0);
            l1->writelog("Authentication successful: " + username);
            // ... (обработка запросов от авторизованного клиента) ...
        } else {
            send(client_socket, "auth_failed", strlen("auth_failed"), 0);
            l1->writelog("Authentication failed: Incorrect password");
            close(client_socket);
            continue;
        }
    }

    // ... (код отключения соединения) ...
    close(server_socket);
    return 0; 
}

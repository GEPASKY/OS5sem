#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fstream>
#include <cstring>

const int BUFFER_SIZE = 256;

int main() {
    int pipe_client[2], pipe_server[2];
    pid_t pid;

    // Создаем каналы
    if (pipe(pipe_client) == -1 || pipe(pipe_server) == -1) {
        perror("pipe");
        return 1;
    }

    // Создаем процесс
    pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid > 0) {
        // Родительский процесс (клиент)
        close(pipe_client[0]); // Закрываем конец чтения
        close(pipe_server[1]); // Закрываем конец записи

        std::string filename;
        std::cout << "Введите имя файла: ";
        std::getline(std::cin, filename);

        // Записываем имя файла в канал
        write(pipe_client[1], filename.c_str(), filename.size() + 1);
        close(pipe_client[1]); // Закрываем конец записи

        char buffer[BUFFER_SIZE];
        read(pipe_server[0], buffer, BUFFER_SIZE);
        std::cout << "Сервер сообщил: " << buffer << std::endl;
        close(pipe_server[0]); // Закрываем конец чтения
        
        wait(NULL); // Ждем завершения дочернего процесса

    } else {
        // Дочерний процесс (сервер)
        close(pipe_client[1]); // Закрываем конец записи
        close(pipe_server[0]); // Закрываем конец чтения

        char buffer[BUFFER_SIZE];
        read(pipe_client[0], buffer, BUFFER_SIZE);
        
        // Проверяем существование файла
        std::ifstream file(buffer);
        if (file) {
            // Читаем содержимое файла и отсылаем обратно клиенту
            file.read(buffer, BUFFER_SIZE);
            write(pipe_server[1], buffer, file.gcount() + 1);
        } else {
            // Отправляем сообщение об ошибке
            const char* error_msg = "Файл не существует";
            write(pipe_server[1], error_msg, strlen(error_msg) + 1);
        }

        close(pipe_client[0]); // Закрываем конец чтения
        close(pipe_server[1]); // Закрываем конец записи
    }

    return 0;
}

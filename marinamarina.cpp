#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fstream>
#include <cstring>

const int BUFFER_SIZE = 256;

int main() {
    int pipe_fd[2];
    pid_t pid;

    // Создаем канал
    if (pipe(pipe_fd) == -1) {
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
        close(pipe_fd[0]); // Закрываем конец чтения

        std::string filename;
        std::cout << "Введите имя файла: ";
        std::getline(std::cin, filename);

        // Записываем имя файла в канал
        write(pipe_fd[1], filename.c_str(), filename.size() + 1);
        
        // Читаем ответ сервера
        char buffer[BUFFER_SIZE];
        read(pipe_fd[0], buffer, BUFFER_SIZE);
        std::cout << "Сервер сообщил: " << buffer << std::endl;

        close(pipe_fd[1]); // Закрываем конец записи
        wait(NULL); // Ждем завершения дочернего процесса

    } else {
        // Дочерний процесс (сервер)
        close(pipe_fd[1]); // Закрываем конец записи

        char buffer[BUFFER_SIZE];
        read(pipe_fd[1], buffer, BUFFER_SIZE);
        
        // Проверяем существование файла
        std::ifstream file(buffer);
        if (file) {
            // Читаем содержимое файла и отправляем обратно клиенту
            file.read(buffer, BUFFER_SIZE);
            write(pipe_fd[0], buffer, file.gcount());
        } else {
            // Отправляем сообщение об ошибке
            const char* error_msg = "Файл не существует";
            write(pipe_fd[0], error_msg, strlen(error_msg) + 1);
        }

        close(pipe_fd[0]); // Закрываем конец чтения
    }

    return 0;
}

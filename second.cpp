/*Вариант 6

Напишите программу выводящую некоторое сообщение через равные промежутки времени
в течение определенного интервала времени. Использовать функции alarm и pause.*/
 /*8 вар*/


#include <stdio.h>  // Для printf
#include <csignal>   // Для signal()
#include <unistd.h>  // Для alarm(), pause()

// Глобальная переменная для отслеживания оставшегося времени
unsigned int remaining_time;

// Обработчик сигнала SIGALRM
void signal_handler(int sig) {
    if (remaining_time > 0) {
        printf("Сообщение: Программа работает. Осталось %i секунд.\n", remaining_time);
        remaining_time--;
        alarm(1); // Снова устанавливаем таймер на 1 секунду
    } else {
        printf("Программа завершена.");
    }
}

int main() {
    unsigned int interval = 1;     // Интервал между сообщениями (в секундах)
    unsigned int total_time = 15; // Общее время работы программы (в секундах)

    remaining_time = total_time;

    // Устанавливаем обработчик сигнала SIGALRM
    signal(SIGALRM, signal_handler);

    // Запускаем первый таймер
    alarm(interval);

    // Ждем завершения программы
    while (remaining_time > 0) {
        pause(); // Ожидаем сигнал SIGALRM
        printf("Alarm\n");
    }

    return 0;
}

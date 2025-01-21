#include <windows.h>
#include <iostream>

class MutexEvent {
private:
    HANDLE eventHandle; // Дескриптор для события

public:
    // Конструктор создает событие в сигнальном состоянии (разблокировано)
    MutexEvent() {
        eventHandle = CreateEvent(
                NULL,  // Дескриптор защиты, NULL — по умолчанию
                TRUE,  // Сброс вручную, установим TRUE
                TRUE,  // Начальное состояние — сигнальное
                NULL   // Без имени
        );
        if (!eventHandle) {
            throw std::runtime_error("Не удалось создать событие.");
        }
    }

    // Деструктор закрывает дескриптор события
    ~MutexEvent() {
        CloseHandle(eventHandle);
    }

    // Захват "мьютекса" (вход в критическую секцию)
    void lock() {
        // Ожидаем, пока событие не станет сигнальным (пока мьютекс не будет свободен)
        WaitForSingleObject(eventHandle, INFINITE);

        // Переводим событие в несигнальное состояние, блокируя мьютекс
        ResetEvent(eventHandle);
    }

    // Освобождение "мьютекса" (выход из критической секции)
    void unlock() {
        // Переводим событие в сигнальное состояние, разблокируя мьютекс
        SetEvent(eventHandle);
    }
};

DWORD WINAPI threadFunction(LPVOID lpParam) {
    MutexEvent* mutex = reinterpret_cast<MutexEvent*>(lpParam);
    int threadId = GetCurrentThreadId();

    std::cout << "Поток " << threadId << " ожидает мьютекс.\n";
    mutex->lock();
    std::cout << "Поток " << threadId << " вошел в критическую секцию.\n";

    // Эмулируем работу в критической секции
    Sleep(1000);

    std::cout << "Поток " << threadId << " выходит из критической секции.\n";
    mutex->unlock();

    return 0;
}

int main() {
    MutexEvent mutex;
    const int numThreads = 5;
    HANDLE threads[numThreads];

    // Создаем потоки, каждый поток будет пытаться войти в критическую секцию
    for (int i = 0; i < numThreads; ++i) {
        threads[i] = CreateThread(
                NULL, 0, threadFunction, &mutex, 0, NULL
        );
        if (threads[i] == NULL) {
            std::cerr << "Не удалось создать поток " << i << std::endl;
            return 1;
        }
    }

    // Ожидаем завершения всех потоков
    WaitForMultipleObjects(numThreads, threads, TRUE, INFINITE);

    // Закрываем дескрипторы потоков
    for (int i = 0; i < numThreads; ++i) {
        CloseHandle(threads[i]);
    }

    return 0;
}

#include <windows.h>
#include <cstdio>
#include <stdexcept>

class MutexEvent {
private:
    HANDLE eventHandle;     // Дескриптор для события
    int lockCount;          // Счётчик захватов мьютекса текущим потоком
    DWORD ownerThreadId;    // Идентификатор потока, захватившего мьютекс

public:
    // Конструктор создает событие в сигнальном состоянии (разблокировано)
    MutexEvent(LPSECURITY_ATTRIBUTES sec, BOOL initial, LPCSTR name): // передаем такие же параметры как и обычному мьютексу
            lockCount(0),
            ownerThreadId(0)
    {
        if (initial)
        {
            lockCount = 1;
            ownerThreadId = GetCurrentThreadId();
        }
        eventHandle = CreateEvent(
                sec,
                FALSE,
                !initial,
                name
        );
        if (!eventHandle) {
            throw std::runtime_error("Не удалось создать событие.");
        }
    }

    // Деструктор закрывает дескриптор события
    ~MutexEvent() {
        CloseHandle(eventHandle);
    }

    // Захват "мьютекса"
    bool lock() {
        DWORD currentThreadId = GetCurrentThreadId();

        // Если мьютекс уже захвачен текущим потоком, просто увеличиваем счётчик
        if (ownerThreadId == currentThreadId) {
            lockCount++;
            return true;
        }

        // Ожидаем, пока событие не станет сигнальным
        DWORD waitResult = WaitForSingleObject(eventHandle, INFINITE);

        if (waitResult == WAIT_OBJECT_0) {
            // Устанавливаем текущий поток владельцем и увеличиваем счётчик
            ownerThreadId = currentThreadId;
            lockCount = 1; // Первый захват
            // ResetEvent(eventHandle); // Переводим событие в несигнальное состояние
            return true;
        } else {
            return false;
        }
    }

    // Освобождение "мьютекса"
    bool unlock() {
        // Проверяем, что текущий поток является владельцем
        if (ownerThreadId != GetCurrentThreadId()) {
            return false; // Другой поток не может освободить мьютекс
        }

        // Уменьшаем счётчик захватов
        lockCount--;

        // Если счётчик достиг нуля, полностью освобождаем мьютекс
        if (lockCount == 0) {
            ownerThreadId = 0; // Сбрасываем владельца
            SetEvent(eventHandle); // Устанавливаем событие в сигнальное состояние
        }

        return true;
    }
};

DWORD WINAPI threadFunction(LPVOID lpParam) {
    MutexEvent* mutex = reinterpret_cast<MutexEvent*>(lpParam); // lpParam - указатель на мьютекс
    // получаем мьютекс как параметр, не создаем
    int threadId = GetCurrentThreadId();

    printf("Поток %d ожидает мьютекс.\n", threadId);
    if (mutex->lock()) {
        printf("Поток %d захватил мьютекс.\n", threadId);

        // Эмулируем работу
        Sleep(1000);

        printf("Поток %d освобождает мьютекс.\n", threadId);
        if (mutex->unlock()) {
            printf("Поток %d успешно освободил мьютекс.\n", threadId);
        } else {
            printf("Поток %d не смог освободить мьютекс.\n", threadId);
        }
    } else {
        printf("Поток %d не смог захватить мьютекс.\n", threadId);
    }

    return 0;
}

int main() {
    MutexEvent mutex(NULL, FALSE, NULL);
    const int numThreads = 5;
    HANDLE threads[numThreads];

    SetConsoleOutputCP(CP_UTF8); // Устанавливаем UTF-8 для корректного вывода русского текста

    // Создаем потоки, каждый поток будет пытаться захватить мьютекс
    for (int i = 0; i < numThreads; ++i) {
        threads[i] = CreateThread(
                NULL, 0, threadFunction, &mutex, 0, NULL
        );
        if (threads[i] == NULL) {
            printf("Не удалось создать поток %d\n", i);
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

#include <iostream>
#include <Windows.h>
#include <vector>
#include <fstream>

/* 7. Даны файл прямого доступа и файл последовательного доступа одинакового размера,
содержащие одинаковую информацию числового типа. Переписать эти файлы в ОП 10-20 раз с
использованием алгоритма чтения "от конца файла". Характеристика: размер файла
(диапазон изменения 100-200 Кбайт).*/

// Функция для генерации файла с числовыми данными заданного размера
void GenerateFile(const wchar_t* file1, const wchar_t* file2, int fileSize) {
    HANDLE file3 = CreateFileW(file1, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE file4 = CreateFileW(file2, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file3 == INVALID_HANDLE_VALUE || file4 == INVALID_HANDLE_VALUE) {
        std::wcerr << L"Failed to create file: " << file1 << std::endl
                   << L"Failed to create file: " << file2 << std::endl;
        return;
    }

    DWORD bytesWritten;
    int value;

    // Записываем случайные числа в файл
    for (int i = 0; i < fileSize / sizeof(int); ++i) {
        value = rand() % 1000;  // Генерация случайного числа
        if (!WriteFile(file3, &value, sizeof(value), &bytesWritten, NULL) || bytesWritten != sizeof(int)
            || !WriteFile(file4, &value, sizeof(value), &bytesWritten, NULL)) {
            std::wcerr << L"Error writing to file: " << file1 << std::endl
                       << file2 << std::endl;
            break;
        }
    }

    LARGE_INTEGER fileSizeLarge, file1SizeLarge;
    GetFileSizeEx(file3, &fileSizeLarge);
    GetFileSizeEx(file4, &file1SizeLarge);
    std::wcout << L"Created file1: " << file1 << L", Size: " << fileSizeLarge.QuadPart << L" bytes" << std::endl
               << L"Created file2: " << file2 << L", Size: " << file1SizeLarge.QuadPart << L"bytes" << std::endl;

    CloseHandle(file3);
    CloseHandle(file4);
}

// Функция для чтения файла "от конца к началу" и возврата данных
std::vector<int> ReadFileBackward(const wchar_t* fileName, int fileSize) {
    HANDLE file = CreateFileW(fileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (file == INVALID_HANDLE_VALUE) {
        std::wcerr << L"Failed to open file: " << fileName << std::endl;
        return {};
    }

    DWORD bytesRead;
    int value;
    std::vector<int> data;

    // Проверка кратности размера файла размеру int
    if (fileSize % sizeof(int) != 0) {
        std::wcerr << L"File size is not aligned with int size, potential data corruption: " << fileName << std::endl;
        CloseHandle(file);
        return {};
    }

    // Чтение файла от конца к началу
    for (int i = fileSize - sizeof(int); i >= 0; i -= sizeof(int)) {
        SetFilePointer(file, i, NULL, FILE_BEGIN);
        if (!ReadFile(file, &value, sizeof(value), &bytesRead, NULL) || bytesRead != sizeof(int)) {
            std::wcerr << L"Error reading from file: " << fileName << std::endl;
            break;
        }

        data.push_back(value);
    }

    CloseHandle(file);
    return data;
}

int main() {
    setlocale(LC_ALL, "Russian");

    const int minFileSize = 100 * 1024; // 100 Кбайт
    const int maxFileSize = 200 * 1024; // 200 Кбайт
    const int fileSize = (rand() % ((maxFileSize - minFileSize) / sizeof(int)) + minFileSize / sizeof(int)) * sizeof(int);

    const wchar_t* directFileName = L"direct_access_file.bin";
    const wchar_t* sequentialFileName = L"sequential_access_file.bin";

    // Генерация двух файлов одинакового размера
    GenerateFile(directFileName, sequentialFileName, fileSize);

    // Операция переписывания данных из файлов 10-20 раз
    for (int iteration = 1; iteration < 20; ++iteration) {
        std::wcout << L"Iteration " << iteration << L": Reading direct access file..." << std::endl;
        std::vector<int> directData = ReadFileBackward(directFileName, fileSize);

        std::wcout << L"Iteration " << iteration << L": Reading sequential access file..." << std::endl;
        std::vector<int> sequentialData = ReadFileBackward(sequentialFileName, fileSize);
    }

    std::ifstream fin1("direct_access_file.bin", std::ios::binary);
    std::ifstream fin2("sequential_access_file.bin", std::ios::binary);
    if (fin1 && fin2)
    {
        char ch1, ch2;
        bool result = true;
        while (fin1.get(ch1) && fin2.get(ch2))
        {
            if (ch1 != ch2)
            {
                result = false;
                break;
            }
        }
        if (result)
            std::cout << "Equal" << std::endl;
        else
            std::cout << "Unequal" << std::endl;
    }
    else
        std::cout << "Error opening file!" << std::endl;
}

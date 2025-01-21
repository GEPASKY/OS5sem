#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>

const int FILE_SIZE = 1024 * 200; // Размер файла 200 Кбайт (можно изменить до 100-200 Кбайт)

void createFiles(const std::string& directAccessFile, const std::string& sequentialAccessFile) {
    std::ofstream directFile(directAccessFile, std::ios::binary);
    std::ofstream sequentialFile(sequentialAccessFile, std::ios::binary);

    if (!directFile || !sequentialFile) {
        std::cerr << "Ошибка при открытии файлов для записи." << std::endl;
        return;
    }

    // Генерация случайных чисел и запись в файлы
    for (int i = 0; i < FILE_SIZE / sizeof(int); ++i) {
        int num = rand() % 1000; // Генерация случайного числа
        directFile.write(reinterpret_cast<char*>(&num), sizeof(int));
        sequentialFile.write(reinterpret_cast<char*>(&num), sizeof(int));
    }

    directFile.close();
    sequentialFile.close();
}

std::vector<int> readFileReverse(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    std::vector<int> data;

    if (!file) {
        std::cerr << "Ошибка при открытии файла для чтения." << std::endl;
        return data;
    }

    // Определяем размер файла
    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    
    // Читаем файл от конца
    for (std::streampos offset = sizeof(int); offset <= fileSize; offset += sizeof(int)) {
        file.seekg(-offset, std::ios::end);
        int num;
        file.read(reinterpret_cast<char*>(&num), sizeof(int));
        data.push_back(num);
    }

    file.close();
    return data;
}

void compareFiles(const std::string& file1, const std::string& file2) {
    std::ifstream fin1(file1, std::ios::binary);
    std::ifstream fin2(file2, std::ios::binary);
    
    if (!fin1 || !fin2) {
        std::cerr << "Ошибка при открытии файлов для сравнения." << std::endl;
        return;
    }

    char ch1, ch2;
    bool result = true;
    
    while (fin1.get(ch1) && fin2.get(ch2)) {
        if (ch1 != ch2) {
            result = false;
            break;
        }
    }

    if (result) {
        std::cout << "Файлы идентичны." << std::endl;
    } else {
        std::cout << "Файлы отличаются." << std::endl;
    }
}

int main() {
    srand(static_cast<unsigned>(time(0))); // Инициализация генератора случайных чисел

    const std::string directAccessFile = "direct_access.dat";
    const std::string sequentialAccessFile = "sequential_access.dat";

    // Создаем файлы
    createFiles(directAccessFile, sequentialAccessFile);

    // Чтение данных из файлов и запись в ОП
    for (int i = 0; i < 20; ++i) { // Повторяем до 20 раз
        std::vector<int> directData = readFileReverse(directAccessFile);
        std::vector<int> sequentialData = readFileReverse(sequentialAccessFile);

        // Для демонстрации выводим первые 10 считанных чисел
        std::cout << "Итерация " << (i + 1) << ":\n";
        std::cout << "Direct Access Data (первые 10 чисел): ";
        for (size_t j = 0; j < 10 && j < directData.size(); ++j) {
            std::cout << directData[j] << " ";
        }
        std::cout << "\nSequential Access Data (первые 10 чисел): ";
        for (size_t j = 0; j < 10 && j < sequentialData.size(); ++j) {
            std::cout << sequentialData[j] << " ";
        }
        std::cout << "\n\n";
    }

    // Сравнение файлов
    compareFiles(directAccessFile, sequentialAccessFile);

    return 0;
}

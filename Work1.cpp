#include <iostream>  
#include <cmath>  
#include <sstream>
#include <string>
#include <vector>
//#include <cctype>
//#include <limits>

using namespace std;

// Структура для хранения координат точки  
struct Point {
    double x;
    double y;
};

// Функция для вычисления расстояния между двумя точками  
double distance(const Point& a, const Point& b) {
    return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}

// Функция для вычисления координат центра описанной окружности и радиуса  
void circumcircle(
    const Point& A,
    const Point& B,
    const Point& C,
    Point& center,
    double& radius)
{
    double D = 2 * (A.x * (B.y - C.y) + B.x * (C.y - A.y) + C.x * (A.y - B.y));

    // Проверяем, не вырожден ли треугольник  
    if (D == 0) {
        throw runtime_error("Треугольник вырожден (вершины совпадают или лежат на одной прямой).");
    }

    center.x = ((A.x * A.x + A.y * A.y) * (B.y - C.y) +
        (B.x * B.x + B.y * B.y) * (C.y - A.y) +
        (C.x * C.x + C.y * C.y) * (A.y - B.y)) / D;

    center.y = ((A.x * A.x + A.y * A.y) * (C.x - B.x) +
        (B.x * B.x + B.y * B.y) * (A.x - C.x) +
        (C.x * C.x + C.y * C.y) * (B.x - A.x)) / D;

    radius = distance(center, A); // Радиус равен расстоянию от центра до одной из вершин  
}

bool isDouble(const std::string& word) {
    try {
        size_t pos;
        stod(word, &pos);  // Пробуем преобразовать строку в double
        return pos == word.size();  // Проверяем, что вся строка является числом
    }
    catch (const std::invalid_argument&) { return false; }
    catch (const std::out_of_range&) { return false; }
}

bool parseTwoDouble(double& num1, double& num2) {
    char input[1024];  // Задаем буфер для ввода
    cin.getline(input, 1024);

    istringstream stream(input);  // Оборачиваем строку в поток для обработки
    vector<double> numbers;
    double temp;
    string word;
    bool count = true;
    while (stream >> word && count) {   // Извлекаем до двух вещественных чисел
        if (isDouble(word)) {
            temp = stod(word);
        }else {
            cerr << "Ошибка: ввод содержит недопустимые символы.\n";
            return false;
        }
        numbers.push_back(temp);
        if (numbers.size() == 2) { count = false; }
    }

    if (numbers.size() < 2) {
        std::cerr << "Ошибка: введено менее двух чисел.\n";
        return false;
    }
    num1 = numbers[0];
    num2 = numbers[1];
    return true;
}

// Функция которая проверяет введенное значение 1 или 2 для повторного запуска программы или ее завершения 
bool parseDouble(int& num) {
    char input[1024];  // Задаем буфер для ввода
    cin.getline(input, 1024);
    istringstream stream(input);  // Оборачиваем строку в поток для обработки
    string word;
    stream >> word;

    if (isDouble(word)) {
        num = stod(word);
    }
    else {
        cerr << "Введите 1 или 2.\n";
        return false;
    }
    if (num < 0 || num > 2) {
        cerr << "Введите 1 или 2.\n"; 
        return false; 
    }
    return true;
}

int isRepeat() {
    bool tmp = true;
    int num;
    while (tmp) {
        if (parseDouble(num) == true) {
            tmp = false;
        }
    }
    return num;
}

// Функция которая проверяет корректность введенных координат 
Point isCoord() {
    Point coord;
    bool tmp = true;
    while (tmp) {
        if (parseTwoDouble(coord.x, coord.y) == true) {
            tmp = false;
        } 
    }
    return coord;
}

int main() {
    bool repeat = true;
    while (repeat) {

        setlocale(LC_ALL, "ru");

        Point A, B, C, center;
        double radius;

        // Ввод координат вершин треугольника  
        cout << "Введите координаты вершины A (x y): ";
        A = isCoord();

        cout << "Координаты А: " << A.x << "," << A.y << endl;

        cout << "Введите координаты вершины B (x y): ";
        B = isCoord();

        cout << "\nКоординаты B: " << B.x << "," << B.y << endl;

        cout << "Введите координаты вершины C (x y): ";
        C = isCoord();

        cout << "\nКоординаты C: " << C.x << "," << C.y << endl;

        try {
            circumcircle(A, B, C, center, radius);
            cout << "Координаты центра описанной окружности: (" << center.x << ", " << center.y << ")\n";
            cout << "Радиус описанной окружности: " << radius << endl;

        }
        catch (const runtime_error& e) {
            cout << "Ошибка: " << e.what() << endl;
        }

        // Запрос на повторный запуск программы 
        cout << "Желаете начать заново?(Если да - 1, если нет - 2): ";
        int num = isRepeat();
        if (num == 2) {
            repeat = false;
        }
    }
    return 0;
}
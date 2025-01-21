
#include <iostream>
#include <cstdlib>
#include <cstring>

const char* my_getenv(const char* name) {
    
    extern char** environ;
    
    for (char** env = environ; *env != nullptr; ++env) {
        const char* env_var = *env;

        if (strncmp(env_var, name, strlen(name)) == 0 && env_var[strlen(name)] == '=') {
            return env_var + strlen(name) + 1; // Пропускаем имя и '='
        }
    }
    
    return nullptr;
}

int main() {
    const char* var_name = "USER"; 
    const char* value = my_getenv(var_name);
    const char* var_name1 = "HOME"; 
    const char* value1 = my_getenv(var_name1);

    if (value) {
        std::cout << var_name << ": " << value << std::endl;
    } else {
        std::cout << var_name << " не найдена." << std::endl;
    }
    if (value1) {
        std::cout << var_name1 << ": " << value1 << std::endl;
    } else {
        std::cout << var_name1 << " не найдена." << std::endl;
    }

    return 0;
}


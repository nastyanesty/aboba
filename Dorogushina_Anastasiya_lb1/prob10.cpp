#include <iostream>
#include <fstream>
#include <string>

// открываем файл
int main(int argc, char* argv[]) {
    // проверка на аргумент
    if (argc <= 1) {
        std::cerr << "Нет файла в аргументе" << std::endl;
        return 1;
    }
    const std::string filename = argv[1];
    std::ifstream file(filename);

    // открылся ли файл
    if (!file.is_open()) {
        std::cerr << "Ошибка открытия файла '" << filename << "'" << std::endl;
        return 1;
    }

    // вывод, если открылся
    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << '\n';
    }
    file.close();
    return 0;
}
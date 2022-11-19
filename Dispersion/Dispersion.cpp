// Дисперсия
// Вам даётся файл с 3 миллионами строк и 10 столбцами. Написать программу, которая находит математическое ожидание и дисперсию величины в каждой строке.
// Протестировать на файле с не менее 4 строками, для которых известны значения математического ожидания и дисперсии в строках.
// Для распараллеливания использовать OpenMP, использовать весь ресурс параллелизма (параллелить, где можно) и оптимизации компилятора.
// Построить график зависимости ускорения по сравнению с 1 потоком и сравнить с линейным ускорением.

#include <iostream>
#include <fstream>
#include <string>

const int STRINGS = 3000000;
const int COLUMNS = 10;

int main() {
    std::string line;

    unsigned int** input_array = new unsigned int* [STRINGS]; // strings
    for (int count = 0; count < STRINGS; count++) {
        input_array[count] = new unsigned int[COLUMNS]; // cols
    }
    unsigned long int* dispersion_array = new unsigned long int [STRINGS]; // strings
    unsigned int* math_array = new unsigned int[STRINGS]; // strings

    std::ifstream in("minmax.txt");

    if (in.is_open()) {
        int i = 0;
        while (getline(in, line)) {
           sscanf(line.c_str(), "%d %d %d %d %d %d %d %d %d %d", &input_array[i][0], &input_array[i][1], &input_array[i][2], &input_array[i][3], &input_array[i][4], &input_array[i][5], &input_array[i][6], &input_array[i][7], &input_array[i][8], &input_array[i][9]);
           i++;
        }
    }
    else {
        std::cout << "File not found" << std::endl;
    }
    in.close();

    for (int i = 0; i < STRINGS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {

        }
    }

    for (int count = 0; count < COLUMNS; count++) {
        delete[] input_array[count];
    }
    delete[] dispersion_array;
    delete[] math_array;
    std::cout << "Finish" << std::endl;
}

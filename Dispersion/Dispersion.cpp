// Дисперсия
// Вам даётся файл с 3 миллионами строк и 10 столбцами. Написать программу, которая находит математическое ожидание и дисперсию величины в каждой строке.
// Протестировать на файле с не менее 4 строками, для которых известны значения математического ожидания и дисперсии в строках.
// Для распараллеливания использовать OpenMP, использовать весь ресурс параллелизма (параллелить, где можно) и оптимизации компилятора.
// Построить график зависимости ускорения по сравнению с 1 потоком и сравнить с линейным ускорением.

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <math.h>
#include <chrono>
#include <omp.h>

const int STRINGS = 3000000;
const int COLUMNS = 10;

void PrintCSV(float math_array[], double dispersion_array[])
{
    std::ofstream out("output.csv");
    out << std::setprecision(20) << "Mathematical expectation;" << "Dispersion;" << "\n\n";

    for (int i = 0; i < STRINGS; i++)
    {
        out << math_array[i] << ";" << dispersion_array[i] << "\n";
    }

    out.close();
}

int main() {
    // Variables
    std::string line;
    unsigned int** input_array = new unsigned int* [STRINGS]; // strings
    for (int count = 0; count < STRINGS; count++) {
        input_array[count] = new unsigned int[COLUMNS]; // cols
    }
    float* math_array = new float[STRINGS]; // strings
    double* dispersion_array = new double[STRINGS]; // strings

    // File reading
    std::ifstream in("minmax.txt");
    auto begin = std::chrono::high_resolution_clock::now();
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
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = end - begin;
    std::cout << "File reading just finished for: " << (float)duration.count() << " sec\n\n";

    // Сalculation
    begin = std::chrono::high_resolution_clock::now();
    omp_set_num_threads(12);
#pragma omp parallel
    {
        #pragma omp for
        for (int i = 0; i < STRINGS; i++)
        {
            unsigned long long int sum0 = 0;
            double sum1 = 0;
            for (int j = 0; j < COLUMNS; j++)
            {
                sum0 += input_array[i][j];
                sum1 += pow(input_array[i][j], 2);
            }
            math_array[i] = (float)(sum0 / COLUMNS);
            dispersion_array[i] = sum1 / COLUMNS;
        }
    }
    end = std::chrono::high_resolution_clock::now();
    duration = end - begin;
    std::cout << "Finish at: " << (float)duration.count() << " sec\n\n";

    // Putting in file
    begin = std::chrono::high_resolution_clock::now();
    PrintCSV(math_array, dispersion_array);
    end = std::chrono::high_resolution_clock::now();
    duration = end - begin;
    std::cout << "Putting in file just finished for: " << (float)duration.count() << " sec\n\n";

    // Cleaning memory
    begin = std::chrono::high_resolution_clock::now();
    for (int count = 0; count < COLUMNS; count++) {
        delete[] input_array[count];
    }
    delete[] dispersion_array;
    delete[] math_array;
    end = std::chrono::high_resolution_clock::now();
    duration = end - begin;
    std::cout << "Cleaning memory just finished for: " << (float)duration.count() << " sec\n\n";

}

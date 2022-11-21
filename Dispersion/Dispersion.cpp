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
#include <vector>

const int STRINGS = 3000000;
const int COLUMNS = 10;

void PrintCSV(float* math_array, long double* dispersion_array)
{
    FILE *f = fopen("output.csv", "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    fprintf(f, "Mathematical expectation;Dispersion;\n\n");
    for (int i = 0; i < STRINGS; i++)
    {
        fprintf(f, "%.9g;%.25g\n", math_array[i], dispersion_array[i]);
    }
    
    fclose(f);
}

void PrintTimes(std::vector<float>& times)
{
    std::ofstream out("times.csv");

    for (int i = 0; i < times.size(); i+=2)
    {
        out << times[i] << ";" << times[i+1] << "\n";
    }

    out.close();
}

void Read(std::vector<std::string>& lines)
{
    std::string line;
    std::ifstream in("minmax.txt");

    if (in.is_open()) {
        while (getline(in, line)) {
            lines.push_back(line);
        }
    }
    else {
        std::cout << "File not found" << std::endl;
        system("pause");
    }
    in.close();
}

void Parse(std::vector<std::string>& lines, unsigned int** input_array) {
        #pragma omp parallel for num_threads(12)
        for (int i = 0; i < STRINGS; i++)
        {
            sscanf(lines[i].c_str(), "%d %d %d %d %d %d %d %d %d %d", &input_array[i][0], &input_array[i][1], &input_array[i][2], &input_array[i][3], &input_array[i][4], &input_array[i][5], &input_array[i][6], &input_array[i][7], &input_array[i][8], &input_array[i][9]);
        }
}

void Calculation(unsigned int** input_array, float* math_array, long double* dispersion_array, int num_threads) {
    omp_set_num_threads(num_threads);
#pragma omp parallel
    {
#pragma omp for
        for (int i = 0; i < STRINGS; i++)
        {
            unsigned long long int sum0 = 0;
            unsigned long long int sum1 = 0;
            for (int j = 0; j < COLUMNS; j++)
            {
                sum0 += input_array[i][j];
            }
            math_array[i] = (float)(sum0 / COLUMNS);
            for (int k = 0; k < COLUMNS; k++)
            {
                sum1 += pow(input_array[i][k] - math_array[i], 2);
            }
            dispersion_array[i] = sum1;
        }
    }
}

void Calculation_interface(unsigned int** input_array, float* math_array, long double* dispersion_array, std::vector<float>& times, int num_threads = 1) {
    auto begin = std::chrono::high_resolution_clock::now();
    Calculation(input_array, math_array, dispersion_array, num_threads);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = end - begin;
    times.push_back((float)num_threads);
    times.push_back((float)duration.count());
    std::cout << "Finish at: " << (float)duration.count() << " sec\n\n";
}

int main() {
    // Variables
    std::vector<std::string> lines;
    std::vector<float> times;
    unsigned int** input_array = new unsigned int* [STRINGS]; // strings
    for (int count = 0; count < STRINGS; count++) {
        input_array[count] = new unsigned int[COLUMNS]; // cols
    }
    float* math_array = new float[STRINGS]; // strings
    long double* dispersion_array = new long double[STRINGS]; // strings

    auto total_begin = std::chrono::high_resolution_clock::now();

    // File reading
    auto begin = std::chrono::high_resolution_clock::now();
    Read(lines);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = end - begin;
    std::cout << "File reading just finished for: " << (float)duration.count() << " sec\n\n";
   
    // File parsing
    begin = std::chrono::high_resolution_clock::now();
    Parse(lines, input_array);
    end = std::chrono::high_resolution_clock::now();
    duration = end - begin;
    std::cout << "File parsing just finished for: " << (float)duration.count() << " sec\n\n";

    // Сalculation
    //Calculation_interface(input_array, math_array, dispersion_array, times);
    int num_threads = 12;
    /*Calculation_interface(input_array, math_array, dispersion_array, times, num_threads);
    num_threads += 2;
    Calculation_interface(input_array, math_array, dispersion_array, times, num_threads);
    num_threads += 2;
    Calculation_interface(input_array, math_array, dispersion_array, times, num_threads);
    num_threads += 2;
    Calculation_interface(input_array, math_array, dispersion_array, times, num_threads);
    num_threads += 2;
    Calculation_interface(input_array, math_array, dispersion_array, times, num_threads);
    num_threads += 2;*/
    Calculation_interface(input_array, math_array, dispersion_array, times, num_threads);

    // Putting in file
    begin = std::chrono::high_resolution_clock::now();
    PrintCSV(math_array, dispersion_array);
    PrintTimes(times);
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

    // Total runtime
    auto total_end = std::chrono::high_resolution_clock::now();
    duration = total_end - total_begin;
    std::cout << "Total runtime: " << (float)duration.count() << " sec\n\n";
}


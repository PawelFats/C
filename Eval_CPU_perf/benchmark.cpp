#include <iostream>
#include <cmath>
#include <ctime>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <sys/time.h>
#include <random>

using namespace std;

template<typename T>
T factorial(T n) {
    if (n == 0 || n == 1) return 1;
    return n * factorial(n - 1);
}

template<typename T>
T sum_of_squares(T n) {
    T sum = 0;
    for (T i = 1; i <= n; ++i) {
        sum += i * i;
    }
    return sum;
}

template<typename T>
T compute_log(T n) {
    return log(n);
}


// Функция для измерения времени с помощью gettimeofday (с точностью до микросекунд)
double get_time(){
    // Структура представляющая календарное время
    struct timeval time; 
    gettimeofday(&time, NULL);
    return time.tv_sec + (time.tv_usec / 1000000.0);
}

// Запуск бенчмарка
template<typename T> 
void run_benchmark(int trials, const string& task_name, T (*task)(int), const string& type){
    // Вектор для хранения времени выполнения каждого теста
    vector<double> times; 
    double start, end;

    // Генерация случайных данных
    srand(static_cast<unsigned int>(time(0)));
    int n = rand() % 10000 + 1; // Генерация случайного числа от 1 до 10000
    
    for (int i = 0; i < trials; ++i){ 
        start = get_time();
        task(n); // запуск теста
        end = get_time();
        double time_taken = end - start;
        times.push_back(time_taken);
    }

    // Вычисление среднего времени
    double sum = 0;
    for (double t : times){
        // Сумма времени выполнения всех тестов
        sum += t; 
    }
    // Среднее время выполнения
    double avg_time = sum / trials;  

    // Вычисление дисперсии для того, чтобы найти отклонение 
    double variance = 0;
    for (double t : times){
        // Сумма квадратов отклонения каждого времени от среднего
        variance += (t - avg_time) * (t - avg_time); 
    }
    // Дисперсия
    variance /= trials; 
    // Старндратное отклоенние 
    double stddev = sqrt(variance); 

    // Простая производительность как размер задачи, делённый на время
    // int task_perf = n / avg_time; 
    double task_perf = trials / sum;

    // Сохранение результатов в файл
    ofstream file;
    file.open("benchmark_results.csv", ios::app);
    file << "AMD Ryzen 7 5800H, " 
        << task_name //Тип теста 
        << ", " 
        << type      //Тип данных 
        << ", "
        << "None, "  //Ключ оптимизации 
        << "insCount, " //Оценка числа инструкций при выполнении типовой задачи
        << "GETTIMEOFDAY, " //Функция обращения к таймеру
        << "time, " //время выполнения отдельного испытания
        << "LNum, " //номер испытания типовой задачи
        << trials    //Кол-во испытаний ??
        << ", " 
        << avg_time  //Среднее время выполнение теста
        << ", " 
        << stddev    // Старндратное отклоенние
        << ", " 
        << (stddev / avg_time) * 100 //Насколько сильно варьируются времена выполнения задачи по сравнению с их средним значением
        << "%, " 
        << task_perf << "\n"; // Производительность
    file.close();
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <number_of_trials>\n";
        return 1;
    }

    int trials = stoi(argv[1]);
    if (trials < 10) {
        trials = 10;
        cout << "The total number of tests is at least 10. Number_of_trials changed to 10!";
    }

    // Запуск с конкретным типом данных (например, int)
    run_benchmark<int>(trials, "Factorial", factorial<int>, "INT");
    run_benchmark<int>(trials, "Sum_of_squares", sum_of_squares<int>, "INT");
    run_benchmark<int>(trials, "Compute_log", compute_log<int>, "INT");

    cout << "Benchmark completed. Results saved to benchmark_results.csv\n";
    return 0;
}


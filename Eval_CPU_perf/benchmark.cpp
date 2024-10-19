#include <iostream>
#include <cmath>
#include <ctime>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <sys/time.h>

using namespace std;

// Функции для выполнения различных задач
double benchmark_task_1_double(int n) {
    double result = 0;
    for (int i = 0; i < n; ++i) {
        result += sin(i) * cos(i);
    }
    return result;
}

float benchmark_task_1_float(int n) {
    float result = 0;
    for (int i = 0; i < n; ++i) {
        result += sin(i) * cos(i);
    }
    return result;
}

int benchmark_task_1_int(int n) {
    int result = 0;
    for (int i = 0; i < n; ++i) {
        result += i * (i + 1);
    }
    return result;
}

// Функция для измерения времени с помощью gettimeofday
double get_time() {
    struct timeval time;
    gettimeofday(&time, NULL);
    return time.tv_sec + (time.tv_usec / 1000000.0);
}

// Запуск бенчмарка
template<typename T>
void run_benchmark(int trials, int n, const string& task_name, T (*task)(int), const string& type, const string& opt_flag) {
    vector<double> times;
    double start, end;

    for (int i = 0; i < trials; ++i) {
        start = get_time();
        task(n);
        end = get_time();
        double time_taken = end - start;
        times.push_back(time_taken);
    }

    // Вычисление среднего времени
    double sum = 0;
    for (double t : times) {
        sum += t;
    }

    double avg_time = sum / trials;
    double variance = 0;
    for (double t : times) {
        variance += (t - avg_time) * (t - avg_time);
    }
    variance /= trials;
    double stddev = sqrt(variance);

    // Сохранение результатов в файл
    ofstream file;
    file.open("benchmark_results.csv", ios::app);
    file << "ProcessorModel, " << task_name << ", " << type << ", " << opt_flag << ", " << n << ", GETTIMEOFDAY, " << trials
         << ", " << avg_time << ", " << stddev << ", " << (stddev / avg_time) * 100 << "%\n";
    file.close();
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " <number_of_trials> <task_size> <optimization_flag>\n";
        return 1;
    }

    int trials = stoi(argv[1]);
    int n = stoi(argv[2]);
    string opt_flag = argv[3]; // Оптимизационные флаги (например, O1, O2, O3)

    // Запуск с разными типами данных
    run_benchmark(trials, n, "Task1_sin_cos", benchmark_task_1_double, "DOUBLE", opt_flag);
    run_benchmark(trials, n, "Task1_sin_cos", benchmark_task_1_float, "FLOAT", opt_flag);
    run_benchmark(trials, n, "Task1_sin_cos", benchmark_task_1_int, "INT", opt_flag);

    cout << "Benchmark completed. Results saved to benchmark_results.csv\n";
    return 0;
}

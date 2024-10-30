#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <sys/time.h>

using namespace std;

// Функция для создания матрицы и заполнения случайными числами
template<typename T>
vector<vector<T>> generate_matrix(int size) {
    vector<vector<T>> matrix(size, vector<T>(size));
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            matrix[i][j] = rand() % 100;
    return matrix;
}

// Сложение двух матриц
template<typename T>
vector<vector<T>> add_matrices(const vector<vector<T>>& A, const vector<vector<T>>& B) {
    int size = A.size();
    vector<vector<T>> C(size, vector<T>(size));
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            C[i][j] = A[i][j] + B[i][j];
    return C;
}

// Умножение двух матриц
template<typename T>
vector<vector<T>> multiply_matrices(const vector<vector<T>>& A, const vector<vector<T>>& B) {
    int size = A.size();
    vector<vector<T>> C(size, vector<T>(size, 0));
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            for (int k = 0; k < size; ++k)
                C[i][j] += A[i][k] * B[k][j];
    return C;
}

// Функция для измерения времени с помощью gettimeofday
double get_time() {
    struct timeval time; 
    gettimeofday(&time, NULL);
    return time.tv_sec + (time.tv_usec / 1000000.0);
}

// Бенчмарк
template<typename T>
void run_benchmark(int trials, const string& task_name, vector<vector<T>> (*task)(const vector<vector<T>>&, const vector<vector<T>>&), int matrix_size, const string& type) {
    vector<double> times;
    double start, end;

    // Генерация случайных матриц
    vector<vector<T>> A = generate_matrix<T>(matrix_size);
    vector<vector<T>> B = generate_matrix<T>(matrix_size);

    for (int i = 0; i < trials; ++i) { 
        start = get_time();
        task(A, B); // запуск теста
        end = get_time();
        times.push_back(end - start);
    }

    // Вычисление ср. времени
    double sum = 0;
    for (double t : times) sum += t;
    double avg_time = sum / trials;

    // Вычисление стандартного отклонения
    double variance = 0;
    for (double t : times) variance += (t - avg_time) * (t - avg_time);
    variance /= trials;
    double stddev = sqrt(variance);

    // Производительность задачи
    double task_perf = trials / sum;

    // Сохранение результатов в файл
    ofstream file("benchmark_results.csv", ios::app);
    file << "AMD Ryzen 7 5800H, "
         << task_name << ", "
         << type << ", "
         << "-O0, "
         << "insCount, "
         << "GETTIMEOFDAY, "
         << matrix_size << ", "
         << trials << ", "
         << avg_time << ", "
         << stddev << ", "
         << (stddev / avg_time) * 100 << "%, "
         << task_perf << "\n";
    file.close();
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cerr << "Использование: " << argv[0] << " <количество_испытаний> <размер_матрицы>\n";
        return 1;
    }

    int trials = stoi(argv[1]);
    if (trials < 10) {
        trials = 10;
        cout << "Общее количество испытаний должно быть не менее 10. Количество испытаний изменено на 10!\n";
    }

    int matrix_size = stoi(argv[2]);
    if (matrix_size < 2) {
        matrix_size = 2;
        cout << "Размер матрицы должен быть не менее 2. Размер матрицы изменен на 2!\n";
    }

    // Запуск бенчмарков для операций с матрицами
    run_benchmark<int>(trials, "Matrix_Addition", add_matrices<int>, matrix_size, "INT");
    run_benchmark<int>(trials, "Matrix_Multiplication", multiply_matrices<int>, matrix_size, "INT");

    run_benchmark<float>(trials, "Matrix_Addition", add_matrices<float>, matrix_size, "FLOAT");
    run_benchmark<float>(trials, "Matrix_Multiplication", multiply_matrices<float>, matrix_size, "FLOAT");

    run_benchmark<double>(trials, "Matrix_Addition", add_matrices<double>, matrix_size, "DOUBLE");
    run_benchmark<double>(trials, "Matrix_Multiplication", multiply_matrices<double>, matrix_size, "DOUBLE");

    cout << "Benchmark completed. Results saved to benchmark_results.csv\n";
    return 0;
}

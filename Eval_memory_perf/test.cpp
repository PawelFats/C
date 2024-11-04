#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <chrono>
#include <unistd.h>
#include <cmath>
#include <algorithm>

using namespace std;

// Используем типы памяти: RAM, HDD, SSD, Flash
enum MemoryType { RAM, HDD, SSD, Flash};

struct TestResult {
    string memoryType;      //Тип пямяти
    size_t blockSize;       //Размер блока данных для записи и чтения на каждом испытании
    string elementType;     //Тип элементов используемых для заполнения массива данных
    size_t bufferSize;      //Размер буфера, т.е. порции данных для выполнения одно операции записи или чтения
    int launchNum;          //Порядковый номер испытания
    string timer;           //Название функции обращения к таймеру (для измерения времени)
    double writeTime;       //Время выполнения отдельного испытания с номером LaunchNum [секунды]
    double avgWriteTime;    //Среднее время записи из LaunchNum испытаний [секунды]
    double writeBandwidth;  //Пропускная способность памяти (BLOCK_SIZE/AverageWriteTime) * 10**6[Mb/s]
    double absErrorWrite;   //Абсолютная погрешность измерения времени записи или СКО [секунды]
    double relErrorWrite;   //Относительная погрешность измерения времени [%]
    double readTime;        //Время выполнения отдельного испытания LaunchNum [секунды]
    double avgReadTime;     //Среднее время записи из LaunchNum испытаний [секунды]
    double readBandwidth;   //Пропускная способность памяти (BLOCK_SIZE/AverageReadTime) * 10**6[Mb/s]
    double absErrorRead;    //Абсолютная погрешность измерения времени чтения или СКО [секунды]
    double relErrorRead;    //Относительная погрешность измерения времени [%]
};

size_t parseBlockSize(const char* arg) {
    string argStr = arg;
    size_t baseSize = 0;

    if (argStr.find("Kb") != string::npos) {
        baseSize = atoi(argStr.c_str()) * 1024;
    } else if (argStr.find("Mb") != string::npos) {
        baseSize = atoi(argStr.c_str()) * 1024 * 1024;
    } else {
        baseSize = atoi(argStr.c_str());
    }
    return baseSize; // возвращаем размер в байтах
}


// Функция для измерения времени
timespec diff(timespec start, timespec end) {
    timespec temp;
    if ((end.tv_nsec - start.tv_nsec) < 0) {
        temp.tv_sec = end.tv_sec - start.tv_sec - 1;
        temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec - start.tv_sec;
        temp.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    return temp;
}

// Функция для записи в файл (HDD, SSD, Flash)
double writeToFile(const string& filePath, const vector<char>& data) {
    timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    ofstream outFile(filePath, ios::binary);
    if (!outFile) {
        cerr << "Ошибка открытия файла для записи!" << endl;
        exit(1);
    }

    outFile.write(data.data(), data.size());
    outFile.close();

    clock_gettime(CLOCK_MONOTONIC, &end);
    timespec duration = diff(start, end);
    return duration.tv_sec + (duration.tv_nsec / 1000000000.0);
}

// Функция для чтения из файла (HDD, SSD, Flash)
double readFromFile(const string& filePath, vector<char>& buffer) {
    timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    ifstream inFile(filePath, ios::binary);
    if (!inFile) {
        cerr << "Ошибка открытия файла для чтения!" << endl;
        exit(1);
    }

    inFile.read(buffer.data(), buffer.size());
    inFile.close();

    clock_gettime(CLOCK_MONOTONIC, &end);
    timespec duration = diff(start, end);
    return duration.tv_sec + (duration.tv_nsec / 1000000000.0);
}

// Функция для записи в RAM
double writeToRAM(char* dest, const char* src, size_t dataSize) {
    timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Записываем случайные данные в массив
    for (size_t i = 0; i < dataSize; ++i) {
        dest[i] = src[i];
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    // Вычисляем время выполнения
    timespec duration;
    duration.tv_sec = end.tv_sec - start.tv_sec;
    duration.tv_nsec = end.tv_nsec - start.tv_nsec;
    if (duration.tv_nsec < 0) {
        duration.tv_sec -= 1;
        duration.tv_nsec += 1000000000;
    }

    return duration.tv_sec + (duration.tv_nsec / 1000000000.0);
}

// Функция для чтения с RAM
double readFromRAM(char* dest, size_t dataSize) {
    timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (size_t i = 0; i < dataSize; ++i) {
        volatile char tmp = dest[i]; // Читаем данные
        (void)tmp;
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    timespec duration = diff(start, end);
    return duration.tv_sec + (duration.tv_nsec / 1000000000.0);
}

void saveResultToCSV(const string& filePath, const TestResult& result) {
    ofstream outFile(filePath, ios::app);
    outFile << result.memoryType << ";"
            << result.blockSize << ";"
            << result.elementType << ";"
            << result.bufferSize << ";"
            << result.launchNum << ";"
            << result.timer << ";"
            << result.writeTime << ";"
            << result.avgWriteTime << ";"
            << result.writeBandwidth << ";"
            << result.absErrorWrite << ";"
            << result.relErrorWrite << ";"
            << result.readTime << ";"
            << result.avgReadTime << ";"
            << result.readBandwidth << ";"
            << result.absErrorRead << ";"
            << result.relErrorRead << endl;
    outFile.close();
}

// Функция запуска тестов
TestResult runTest(MemoryType memType, size_t blockSize, int numTests, const string& filePath = "") {
    vector<char> buffer(blockSize);
    char* data = new char[blockSize];
    TestResult result;
    result.memoryType = (memType == RAM) ? "RAM" : "SSD";
    result.blockSize = blockSize;
    result.elementType = "char"; // Тип элементов - char
    result.bufferSize = blockSize;
    result.timer = "clock_gettime";
    
    double totalWriteTime = 0, totalReadTime = 0;

    // Генерируем заранее один блок случайных данных размером blockSize
    vector<char> randomData(blockSize);
    for (size_t i = 0; i < blockSize; ++i) {
        randomData[i] = rand() % 256;
    }

    for (int i = 0; i < numTests; ++i) {
        double writeTime = 0, readTime = 0;

        if (memType == RAM) {
            // Копируем заранее подготовленные случайные данные в buffer
            //std::copy(randomData.begin(), randomData.end(), buffer.begin());

            // Записываем данные в память и считаем время
            writeTime = writeToRAM(buffer.data(), randomData.data(), blockSize);
            readTime = readFromRAM(buffer.data(), blockSize);
        } else {
            // Для SSD/HDD/Flash
            std::copy(randomData.begin(), randomData.end(), buffer.begin());

            writeTime = writeToFile(filePath, buffer);
            readTime = readFromFile(filePath, buffer);
        }
 
        totalWriteTime += writeTime;
        totalReadTime += readTime;

        result.writeTime = writeTime;
        result.readTime = readTime;
        result.launchNum = i + 1;

        result.avgWriteTime = totalWriteTime / result.launchNum;
        result.avgReadTime = totalReadTime / result.launchNum;
        result.writeBandwidth = (blockSize / result.avgWriteTime) * 1e-6;
        result.readBandwidth = (blockSize / result.avgReadTime) * 1e-6;

        // Погрешности
        result.absErrorWrite = sqrt(((writeTime - result.avgWriteTime) * (writeTime - result.avgWriteTime)) / result.launchNum);
        result.relErrorWrite = (result.absErrorWrite /  result.avgWriteTime) * 100;
        result.absErrorRead = sqrt(((readTime - result.avgReadTime) * (readTime - result.avgReadTime)) / result.launchNum);
        result.relErrorRead = (result.absErrorRead /  result.avgReadTime) * 100;

        saveResultToCSV("results.csv", result);
    }

    delete[] data; // Не забываем освободить память
    return result;
}


int main(int argc, char* argv[]) {
    if (argc < 7) {
        cerr << "Usage: ./memory_test --memory-type RAM|HDD|SSD|flash --block-size SIZE --launch-count COUNT" << endl;
        return 1;
    }

    // Значенеия по умолчанию
    MemoryType memType = RAM;
    size_t blockSize = 1024;
    int numTests = 10;
    string filePath = "test_file.dat";

    // Пример вызова программы: ./memory_test --memory-type RAM --block-size 1024 --launch-count 10
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--memory-type") == 0) {
            if (strcmp(argv[i + 1], "RAM") == 0) {
                memType = RAM;
            } else if (strcmp(argv[i + 1], "HDD") == 0 || strcmp(argv[i + 1], "SSD") == 0 || strcmp(argv[i + 1], "flash") == 0) {
                memType = SSD;
                filePath = "test_file_" + string(argv[i + 1]) + ".dat"; // Файл для записи на диск
            }
        } else if (strcmp(argv[i], "--block-size") == 0) {
            blockSize = parseBlockSize(argv[i + 1]);
        } else if (strcmp(argv[i], "--launch-count") == 0) {
            numTests = atoi(argv[i + 1]);
        }
    }

    TestResult result = runTest(memType, blockSize, numTests, filePath);

    cout << "Тест завершен. Результаты сохранены в 'results.csv'" << endl;
    return 0;
}

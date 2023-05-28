#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <bitset>
#include <iomanip>


using namespace std;

enum CellStatus {
    FREE,     // Свободная ячейка
    BUSY,     // Занятая ячейка
    DELETED   // Удаленная ячейка
};

struct CustomHashTableEntry {
    string customKey;          // Ключ элемента
    CellStatus customStatus;   // Состояние ячейки

    CustomHashTableEntry() : customStatus(FREE) {}
};

class CustomHashTable {
private:
    vector<CustomHashTableEntry> customTable;   // Хеш-таблица
    int customSize;                       // Размер таблицы
    int customCount;                      // Текущее количество элементов

    int customHashFunction(const string& key) {
        string concatenatedKey;
        for (char c : key) {
            concatenatedKey += bitset<8>(static_cast<unsigned char>(c)).to_string();
        }

        // Преобразование конкатенированной строки в натуральное число
        unsigned long long hash = stoull(concatenatedKey, nullptr, 2);

        return hash % customSize;
    }


    int customLinearProbing(int index, int attempt) {
        return (index + attempt) % customSize;
    }

    int customFindInsertionIndex(const string& key) {
        int index = customHashFunction(key);
        int attempt = 0;
        int insertionIndex = -1; // Индекс для вставки дубликата

        while (customTable[index].customStatus == BUSY) {
            if (customTable[index].customKey == key) {
                // Найден дубликат ключа, сохраняем индекс для вставки
                insertionIndex = index;
            }

            attempt++;
            index = customLinearProbing(index, attempt);
        }

        if (insertionIndex != -1 && customTable[insertionIndex].customStatus == DELETED) {
            // Если был найден дубликат и есть удаленная ячейка, используем ее для вставки
            return insertionIndex;
        }

        return index;
    }



public:
    // Конструктор хеш-таблицы
    CustomHashTable(int tableSize) : customSize(tableSize), customCount(0) {
        customTable.resize(customSize);
    }

    // Вставка элемента по ключу k
    void customInsert(const string& key) {
        if (customCount >= customSize) {
            cout << "Ошибка: Хеш-таблица заполнена." << endl;
            return;
        }

        int index = customFindInsertionIndex(key);

        customTable[index].customKey = key;
        customTable[index].customStatus = BUSY;
        customCount++;

        cout << "Элемент '" << key << "' был успешно вставлен." << endl;
    }

    //Поиск дубликата ключа
    bool customFindDuplicate(const string& key) {
        int index = customHashFunction(key);
        int probes = 0;
        int duplicateCount = 0;  // Счетчик дубликатов

        while (customTable[index].customStatus != FREE && probes < customSize) {
            if (customTable[index].customStatus == BUSY && customTable[index].customKey == key) {
                duplicateCount++;
                if (duplicateCount >= 3) {
                    return true;  // Найдено две и более копий ключа
                }
            }
            index = customLinearProbing(index, probes);
            probes++;
        }
        return false;  // Найдена только одна или ни одной копии ключа
    }

    // Поиск элемента по ключу k
    bool customSearch(const string& key) {
        int index = customHashFunction(key);
        int probes = 0;
        while (customTable[index].customStatus != FREE && probes < customSize) {
            if (customTable[index].customStatus == BUSY && customTable[index].customKey == key) {
                return true;
            }
            index = customLinearProbing(index, probes);
            probes++;
        }
        return false;
    }

    // Удаление элемента по ключу k
    void customRemove(const string& key) {
        int index = customHashFunction(key);
        int probes = 0;
        while (customTable[index].customStatus != FREE && probes < customSize) {
            if (customTable[index].customStatus == BUSY && customTable[index].customKey == key) {
                customTable[index].customStatus = DELETED;
                customCount--;

                cout << "Элемент \"" << key << "\" был удален." << endl;

                return;
            }
            index = (index + 1) % customSize;
            probes++;
        }

        cout << "Ошибка: Элемент \"" << key << "\" не найден." << endl;
    }

    // Текущее количество элементов в таблице
    int customGetCurrentSize() {
        return customCount;
    }

    // Текущий размер таблицы
    int customGetCurrentCount() {
        return customSize;
    }

    // Проверка на пустоту таблицы
    bool customIsEmpty() {
        return customCount == 0;
    }

    // Вывод содержимого таблицы на экран
    void customDisplay() {
        cout << "Содержимое хеш-таблицы:" << endl;
        cout << "=======================" << endl;
        for (int i = 0; i < customSize; i++) {
            cout << "INDEX " << i << ": ";
            if (customTable[i].customStatus == BUSY) {
                cout << customTable[i].customKey;
            }
            else if (customTable[i].customStatus == DELETED) {
                cout << "[DELETED]";
            }
            else {
                cout << "[FREE]";
            }
            cout << endl;
        }
    }

    void customEvaluateComplexity() {
        vector<double> loadFactors = { 0.5, 0.7, 0.9 };

        for (double loadFactor : loadFactors) {

            int tableSize = static_cast<int>(customSize * loadFactor);
            CustomHashTable evaluationTable(tableSize);
            evaluationTable.customClear();

            // Вставка элементов
            auto startTime = chrono::high_resolution_clock::now();
            for (int i = 0; i < tableSize; i++) {
                string key = "Key" + to_string(i);
                evaluationTable.customInsert(key);
            }
            auto endTime = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::microseconds>(endTime - startTime);
            cout << "Оценка трудоемкости вставки для коэффициента заполнения " << loadFactor << ": " << duration.count() << " микросекунд." << endl;

            // Поиск элементов
            startTime = chrono::high_resolution_clock::now();
            for (int i = 0; i < tableSize; i++) {
                string key = "Key" + to_string(i);
                evaluationTable.customSearch(key);
            }
            endTime = chrono::high_resolution_clock::now();
            duration = chrono::duration_cast<chrono::microseconds>(endTime - startTime);
            cout << "Оценка трудоемкости поиска для коэффициента заполнения " << loadFactor << ": " << duration.count() << " микросекунд." << endl;

            // Удаление элементов
            startTime = chrono::high_resolution_clock::now();
            for (int i = 0; i < tableSize; i++) {
                string key = "Key" + to_string(i);
                evaluationTable.customRemove(key);
            }
            endTime = chrono::high_resolution_clock::now();
            duration = chrono::duration_cast<chrono::microseconds>(endTime - startTime);
            cout << "Оценка трудоемкости удаления для коэффициента заполнения " << loadFactor << ": " << duration.count() << " микросекунд." << endl;

            cout << "-------------------------" << endl;
        }
    }





    // Очистка таблицы
    void customClear() {
        cout << "Очистка таблицы...." << endl;
        for (int i = 0; i < customSize; i++) {
            customTable[i].customStatus = FREE;
        }
        customCount = 0;
        cout << "Таблица была очищена.\n" << endl;
    }


    void printMenu() { // Вывод меню управления хеш-таблицей
        cout << "===============Меню===============" << endl;
        cout << "1| Вставить элемент" << endl;
        cout << "----------------------------------" << endl;
        cout << "2| Поиск элемента" << endl;
        cout << "----------------------------------" << endl;
        cout << "3| Удалить элемент" << endl;
        cout << "----------------------------------" << endl;
        cout << "4| Отобразить содержимое таблицы" << endl;
        cout << "----------------------------------" << endl;
        cout << "5| Проверить пустоту таблицы" << endl;
        cout << "----------------------------------" << endl;
        cout << "6| Очистить таблицу" << endl;
        cout << "----------------------------------" << endl;
        cout << "7| Провести оценку трудоемкости" << endl;
        cout << "----------------------------------" << endl;
        cout << "8| Поиск дубликата" << endl;
        cout << "----------------------------------" << endl;
        cout << "9| Выход" << endl;
        cout << "==================================" << endl;
    }
};



int main() {
    setlocale(LC_ALL, "Russian");
    CustomHashTable customHashTable(12); // Создание экземпляра пользовательской хеш-таблицы с размером 20

    int choice;
    string keyValue;


    do {
        customHashTable.printMenu();
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Введите значение элемента: ";
            cin >> keyValue;
            customHashTable.customInsert(keyValue); // Вставка элемента в хеш-таблицу
            break;
        case 2:
            cout << "Введите значение элемента: ";
            cin >> keyValue;
            if (customHashTable.customSearch(keyValue)) { // Поиск элемента в хеш-таблице
                cout << "Элемент '" << keyValue << "' найден в хеш-таблице." << endl;
            }
            else {
                cout << "Элемент '" << keyValue << "' не найден в хеш-таблице." << endl;
            }
            break;
        case 3:
            cout << "Введите значение элемента: ";
            cin >> keyValue;
            customHashTable.customRemove(keyValue); // Удаление элемента из хеш-таблицы
            break;
        case 4:
            customHashTable.customDisplay(); // Отображение содержимого хеш-таблицы
            break;
        case 5:
            if (customHashTable.customIsEmpty()) { // Проверка на пустоту хеш-таблицы
                cout << "Хеш-таблица пуста." << endl;
            }
            else {
                cout << "Хеш-таблица не пуста." << endl;
            }
            break;
        case 6:
            customHashTable.customClear(); // Очистка хеш-таблицы
            break;
        case 7:
            customHashTable.customEvaluateComplexity(); // Оценка трудоемкости
            break;
        case 8:
            cout << "Введите значение элемента: ";
            cin >> keyValue;
            if (customHashTable.customFindDuplicate(keyValue)) { // Поиск дубликата в хеш-таблице
                cout << "Найден дубликат элемента '" << keyValue << "'." << endl;
            }
            else {
                cout << "Дубликат элемента '" << keyValue << "' не найден." << endl;
            }
            break;
        case 9:
            cout << "Программа завершена." << endl;
            break;
        default:
            cout << "Ошибка: Неверный номер операции." << endl;
        }

        cout << endl;

    } while (choice != 9);

    return 0;
}

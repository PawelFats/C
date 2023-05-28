#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <random>

using namespace std;

// Класс кучи
class MinHeap {
private:
    vector<int> heap;

    // Вспомогательные методы
    int parent(int i) { return (i - 1) / 2; } // Возвращает индекс родителя
    int leftChild(int i) { return 2 * i + 1; } // Возвращает индекс левого потомка
    int rightChild(int i) { return 2 * i + 2; } // Возвращает индекс правого потомка

    // Просеивание вниз (приведение кучи к правильному виду)
    void siftDown(int i) {
        int minIndex = i;
        int left = leftChild(i);
        int right = rightChild(i);

        // Сравнение с левым потомком
        if (left < heap.size() && heap[left] < heap[minIndex])
            minIndex = left;

        // Сравнение с правым потомком
        if (right < heap.size() && heap[right] < heap[minIndex])
            minIndex = right;

        // Если индекс изменился, меняем элементы местами и вызываем siftDown для новой позиции
        if (i != minIndex) {
            swap(heap[i], heap[minIndex]);
            siftDown(minIndex);
        }
    }

    // Просеивание вверх (приведение кучи к правильному виду)
    void siftUp(int i) {
        while (i > 0 && heap[parent(i)] > heap[i]) {
            swap(heap[i], heap[parent(i)]);
            i = parent(i);
        }
    }

public:
    // Конструктор
    MinHeap() {}

    // Конструктор с параметром - превращает неупорядоченный массив в кучу
    MinHeap(vector<int>& array) {
        heap = array;

        // Начинаем просеивание с самого последнего родителя
        for (int i = heap.size() / 2 - 1; i >= 0; --i) {
            siftDown(i);
        }
    }

    // Добавление элемента в кучу
    void insert(int value) {
        heap.push_back(value);
        int i = heap.size() - 1;
        siftUp(i);
    }

    // Исключение минимального элемента из кучи
    void extractMin() {
        if (heap.empty()) {
            cout << "Куча пуста." << endl;
            return;
        }

        int min = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        siftDown(0);

        cout << "Минимальный элемент " << min << " удален из кучи." << endl;
    }

    // Изменение значения элемента по индексу
    void changeValue(int index, int newValue) {
        if (index < 0 || index >= heap.size()) {
            cout << "Неверный индекс." << endl;
            return;
        }

        int oldValue = heap[index];
        heap[index] = newValue;

        if (newValue < oldValue)
            siftUp(index);
        else
            siftDown(index);

        cout << "Значение элемента с индексом " << index << " изменено." << endl;
    }

    // Проверка, является ли дерево кучей
    bool isHeap() {
        for (int i = 0; i < heap.size(); ++i) {
            int left = leftChild(i);
            int right = rightChild(i);

            if (left < heap.size() && heap[i] > heap[left])
                return false;

            if (right < heap.size() && heap[i] > heap[right])
                return false;
        }

        return true;
    }

    // Находит максимальный элемент в куче
    int findMax() const {
        if (isEmpty())
            throw out_of_range("Куча пуста.");

        int max = heap[0];
        for (int i = 1; i < heap.size(); ++i) {
            if (heap[i] > max)
                max = heap[i];
        }

        return max;
    }

    // Удаляет максимальный элемент из кучи
    void deleteMax() {
        if (isEmpty())
            throw out_of_range("Куча пуста.");

        int max = findMax();
        for (int i = 0; i < heap.size(); ++i) {
            if (heap[i] == max) {
                heap[i] = heap.back();
                heap.pop_back();
                siftDown(i);
                cout << "Удален максимальный элемент: " << max << endl;
                return;
            }
        }
    }

    // Проверка дерева на пустоту
    bool isEmpty() const {
        return heap.empty();
    }

    // Вывод структуры дерева на экран (обход в прямом порядке)
    void printPreOrder(int index) {
        if (index >= heap.size())
            return;

        cout << heap[index] << " "; // Печать текущего элемента
        printPreOrder(rightChild(index)); // Рекурсивный вызов для правого потомка
        printPreOrder(leftChild(index)); // Рекурсивный вызов для левого потомка
    }

    // Вывод структуры дерева на экран (обход в прямом порядке)
    void print() {
        for (int i = 0; i < heap.size(); ++i) {
            cout << heap[i] << " ";
        }
        cout << endl;
    }

    // Вспомогательная функция для печати структуры дерева с ветвями
    void printTree(int index, string prefix, bool isLastChild) const {
        cout << prefix;
        cout << (isLastChild ? "|__ " : "|-- ");
        cout << heap[index] << endl;

        string childPrefix = prefix + (isLastChild ? "    " : "|   ");

        int leftChild = 2 * index + 1;
        int rightChild = 2 * index + 2;

        if (leftChild < heap.size())
            printTree(leftChild, childPrefix, rightChild >= heap.size());

        if (rightChild < heap.size())
            printTree(rightChild, childPrefix, false);

    }

    // Размер дерева (количество элементов)
    int size() const {
        return heap.size();
    }

    // Очистка дерева
    void clear() {
        heap.clear();
        cout << "Дерево очищено." << endl;
    }

    // Функция для генерации случайных чисел в заданном диапазоне
        vector<int> generateRandomNumbers(int n, int minValue, int maxValue) {
        vector<int> numbers;
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(minValue, maxValue);

        for (int i = 0; i < n; ++i) {
            numbers.push_back(dis(gen));
        }

        return numbers;
    }

    // Функция для создания вырожденной структуры дерева (худший случай)
    vector<int> createDegenerateStructure(int n) {
        vector<int> structure;
        for (int i = n; i > 0; --i) {
            structure.push_back(i);
        }
        return structure;
    }

    // Тестирование трудоёмкости операций вставки, удаления и поиска для средней структуры дерева
    void testAverageCaseComplexity(int n) {
        vector<int> numbers = generateRandomNumbers(n, 1, 1000);
        MinHeap heap(numbers);

        // Тестирование вставки
        auto start = chrono::steady_clock::now();
        heap.insert(999);
        auto end = chrono::steady_clock::now();
        auto diff = end - start;
        cout << "Средняя трудоемкость операции вставки: " << chrono::duration<double, milli>(diff).count() << " мс" << endl;

        // Тестирование поиска максимального элемента
        start = chrono::steady_clock::now();
        heap.findMax();
        end = chrono::steady_clock::now();
        diff = end - start;
        cout << "Средняя трудоемкость операции поиска максимального элемента: " << chrono::duration<double, milli>(diff).count() << " мс" << endl;

        // Тестирование удаления
        start = chrono::steady_clock::now();
        heap.extractMin();
        end = chrono::steady_clock::now();
        diff = end - start;
        cout << "Средняя трудоемкость операции удаления: " << chrono::duration<double, milli>(diff).count() << " мс" << endl;
    }

    // Тестирование трудоёмкости операций вставки, удаления и поиска для вырожденной структуры дерева (худший случай)
    void testWorstCaseComplexity(int n) {
        vector<int> structure = createDegenerateStructure(n);
        MinHeap heap(structure);

        // Тестирование вставки
        auto start = chrono::steady_clock::now();
        heap.insert(999);
        auto end = chrono::steady_clock::now();
        auto diff = end - start;
        cout << "Трудоемкость операции вставки в вырожденную структуру: " << chrono::duration<double, milli>(diff).count() << " мс" << endl;

        // Тестирование поиска максимального элемента
        start = chrono::steady_clock::now();
        heap.findMax();
        end = chrono::steady_clock::now();
        diff = end - start;
        cout << "Трудоемкость операции поиска максимального элемента в вырожденной структуре: " << chrono::duration<double, milli>(diff).count() << " мс" << endl;
    
        // Тестирование удаления
        start = chrono::steady_clock::now();
        heap.extractMin();
        end = chrono::steady_clock::now();
        diff = end - start;
        cout << "Трудоемкость операции удаления в вырожденной структуре: " << chrono::duration<double, milli>(diff).count() << " мс" << endl;
    }


};

int main() {
    setlocale(LC_ALL, "Russian");
    MinHeap minHeap;
    vector<int> array;
    int choice, value, index;

    do {
        cout << "--------------------- Меню ---------------------";
        cout << "\n1. Создать кучу из неупорядоченного массива";
        cout << "\n2. Добавить элемент в кучу";
        cout << "\n3. Исключить минимальный элемент из кучи";
        cout << "\n4. Изменить значение элемента по индексу";
        cout << "\n5. Проверить, является ли дерево кучей";
        cout << "\n6. Вывести структуру дерева(читаемый вид)";
        cout << "\n7. Проверить дерево на пустоту";
        cout << "\n8. Узнать размер дерева";
        cout << "\n9. Очистить дерево";
        cout << "\n10. Структура дерева (прямой порядок)";
        cout << "\n11. Найти максимальный элемент";
        cout << "\n12. Удалить максимальный элемент";
        cout << "\n13. Сравнительное тестирование трудоемкости операций (средний случай)";
        cout << "\n14. Сравнительное тестирование трудоемкости операций (вырожденный случай)";
        cout << "\n0. Выйти";
        cout << "\n";
        cout << "\nВыберите операцию: ";
        cin >> choice;

        switch (choice) {
        case 1:
            int n;
            cout << "Введите размер массива: ";
            cin >> n;

            cout << "Введите элементы массива: ";
            for (int i = 0; i < n; ++i) {
                cin >> value;
                array.push_back(value);
            }

            minHeap = MinHeap(array);
            cout << "Куча создана." << endl;
            break;

        case 2:
            cout << "Введите значение элемента: ";
            cin >> value;
            minHeap.insert(value);
            cout << "Элемент добавлен в кучу." << endl;
            break;

        case 3:
            minHeap.extractMin();
            break;

        case 4:
            cout << "Введите индекс элемента: ";
            cin >> index;
            cout << "Введите новое значение элемента: ";
            cin >> value;
            minHeap.changeValue(index, value);
            break;

        case 5:
            if (minHeap.isHeap())
                cout << "Дерево является кучей." << endl;
            else
                cout << "Дерево не является кучей." << endl;
            break;

        case 6:
            if (minHeap.isEmpty()) {
                cout << "Дерево пусто." << endl;
            }
            else {
                cout << "Структура дерева:" << endl;
                minHeap.printTree(0, "", true);
            }
            break;

        case 7:
            if (minHeap.isEmpty())
                cout << "Дерево пусто." << endl;
            else
                cout << "Дерево не пусто." << endl;
            break;

        case 8:
            cout << "Размер дерева: " << minHeap.size() << endl;
            break;

        case 9:
            minHeap.clear();
            break;

        case 10:
            if (minHeap.isEmpty())
                cout << "Дерево пусто." << endl;
            else
                cout << "Структура дерева (прямой порядок): ";
                minHeap.printPreOrder(0);
                cout << endl;
            break;

        case 11:
            try {
                int max = minHeap.findMax();
                cout << "Максимальный элемент: " << max << endl;
            }
            catch (const out_of_range& e) {
                cout << e.what() << endl;
            }
            break;

        case 12:
            try {
                minHeap.deleteMax();
            }
            catch (const out_of_range& e) {
                cout << e.what() << endl;
            }
            break;
        case 13:
            cout << "Введите количество элементов для тестирования: ";
            cin >> n;
            minHeap.testAverageCaseComplexity(n);
            break;
        case 14:
            cout << "Введите количество элементов для тестирования: ";
            cin >> n;
            minHeap.testWorstCaseComplexity(n);
            break;
        
        case 0:
            cout << "Программа завершена." << endl;
            break;

        default:
            cout << "Неверный выбор." << endl;
            break;
        }

        cout << endl;

    } while (choice != 0);

    return 0;
}

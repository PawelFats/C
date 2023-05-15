#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>


typedef struct Node {
    int value;
    struct Node* next;
} Node;


void rus_lan(), push(Node**, int), push_back(Node*, int), print_reverse(Node*), insert(Node*, int, int), delete_list(Node**);
void from_array(Node**, int*, int), print_list(Node*), editing(Node**, int, int), bubbleSort(Node*), comb_sort(Node*);
int pop(Node**), pop_back(Node**), delete_nth(Node**, int), length(Node*);
Node* get_nth(Node*, int);
Node* get_last(Node*);


void push(Node** head, int data) {     //(вставка в начало списка) так как указатель head изменяется, то необходимо передавать указатель на указатель
    Node* tmp = malloc(sizeof(struct Node)); //выделяем память для новой ячейки (тип указателя (struct node))
    tmp->value = data; //кладем в новую ячейку памяти(на которую указывает указатель) данные
    tmp->next = (*head); // Присваиваем указателю tmp адрес предыдущего узла
    (*head) = tmp; // Присваиваем указателю head адрес нового узла
}  //После выхода из функции переменная tmp будет уничтожена

int pop(Node** head) { //удаление первого элемента(передаем адрес адреса)
    Node* prev = NULL;
    int val;
    if (head == NULL) exit(-1);
    prev = (*head); //сохраняем адрес первого узла
    val = prev->value; //сохраняем данные первого узла
    (*head) = (*head)->next; //перекидываем указатель head на следующий узел(тем самым удалив первый узел из списка)
    free(prev);// высвобождаем память
    return val;
}

Node* get_nth(Node* head, int n) {//получение указателя на n-ый элемент (так как возвращается указа
    int counter = 0;//  тель типа Node то поэтому у нас фу-ия имеет тип node*
    while (counter < n && head) { //пока не дошли до n и пока head != null(чтобы нельзя было пойти дальше нашего списка)
        head = head->next;
        counter++;
    }
    return head;
}

Node* get_last(Node* head) {//получение указателя на последний элемент
    if (head == NULL) return NULL;
    while (head->next) head = head->next;//пока не дошли до последнего элемента, который будет ссылать на null
    return head;
}

void push_back(Node* head, int value) { //добавляет новый элемент в конец списка
    Node* last = get_last(head);
    Node* tmp = malloc(sizeof(struct Node));
    tmp->value = value;
    tmp->next = NULL;
    last->next = tmp;//так как мы меняем конкретно элемент списка тоесть указательс, нам не надо передавать node** head
}

void print_reverse(Node* head) { //печать списка с хвоста
    if (head == NULL) {
        return;
    }
    print_reverse(head->next);
    printf("%d ", head->value);
}

int pop_back(Node** head) { //удаление последнего элемента
    int val;
    Node* pFwd = NULL; //текущий узел
    Node* pBwd = NULL; //предыдущий узел 
    if (!head) exit(-1); //получили NULL
    pFwd = *head;
    while (pFwd->next) { //пока невстретили NULL
        pBwd = pFwd;
        pFwd = pFwd->next;
    }
    val = pFwd->value;
    if (pBwd == NULL) { //если в списке один элемент
        free(*head);
        *head = NULL;
    }
    else {
        free(pFwd->next);
        pBwd->next = NULL;
    }
    return val;
}

void insert(Node* head, int n, int val) {//вставляет на n-е место новое значение
    int i = 0;
    Node* tmp = (Node*)malloc(sizeof(Node));
    tmp->value = val;
    //Находим нужный элемент. Если вышли за пределы списка, то выходим из цикла,
    //ошибка выбрасываться не будет, произойдёт вставка в конец
    while (i < n && head->next) {
        head = head->next;
        i++;
    }
    //Если это не последний элемент 
    if (head->next) {
        tmp->next = head->next; //делаем так, чтобы новый элемент ссылался на следующий после n-го
    }
    else {
        tmp->next = NULL;
    }
    head->next = tmp; //Перекидываем указатель next n-го элемента на вновь созданный узел
}

int delete_nth(Node** head, int n) { //Функция удаления n-го элемента
    int val;
    if (n == 0) {
        return pop(head); //удаляем первый элемент
    }
    else {
        Node* prev = get_nth(*head, n - 1); //указатель на n-1 элемент
        Node* elm = prev->next; //указатель на n элемент(который будем удалять)
        val = elm->value; //даные из n элемента
        prev->next = elm->next; //перекидываем указатель n-1 элемента на n+1 (убираем пробел после удаления)
        free(elm);
        printf("%d\n", val);

    }

}

void delete_list(Node** head) { //удаления списка (чтобы было быстрее, удаляем с головы списка)
    Node* prev = NULL;
    while ((*head)->next) {
        prev = (*head);
        (*head) = (*head)->next;
        free(prev);
    }
    free(*head);
}

void from_array(Node** head, int* arr, int size) {// создание списка из массива при помощи push, масив обходим с конца
    int i = size - 1;
    if (arr == NULL || size == 0) return;
    do {
        push(head, arr[i]);
    } while (i-- != 0);
}

void print_list(Node* head) { //вывод списка
    while (head) {
        printf("%d ", head->value);
        head = head->next;
    }
    printf("\n");
}

void editing(Node** head, int n, int val) {
    Node* elm = get_nth(*head, n);
    printf("Старое значение: %d\n", elm->value);
    elm->value = val;
}

void bubbleSort(Node* head) {
    if (head == NULL || head->next == NULL) {
        return;
    }
    int swapped;
    Node* ptr1;
    Node* lptr = NULL;
    do {
        swapped = 0; //флаг
        ptr1 = head;
        while (ptr1->next != lptr) { //проверяем несравнивали ли мы этот элемент(самый большой узел в каждой итерации доходит до узла который больше его)
            if (ptr1->value > ptr1->next->value) { //кто больше
                int temp = ptr1->value;
                ptr1->value = ptr1->next->value;
                ptr1->next->value = temp;
                swapped = 1; //флаг 1, значит произошел обмен
            }
            ptr1 = ptr1->next; //прыгаем на следущий узел
        }
        lptr = ptr1; //чтобы неперебирать узел снова
    } while (swapped);
}

int length(Node* head) { //подсчет длины списка
    int len = 0;
    Node* current = head;
    while (current != NULL) {
        len++;
        current = current->next;
    }
    return len;
}

void comb_sort(Node* head) {
    int gap = length(head); // Изначально принимаем gap равным длине списка
    float shrink = 1.3; // Коэффициент уменьшения gap

    while (gap > 1) {
        // текущий и следующий указатели на элементы списка
        Node* curr = head;
        Node* next = head->next;
        int swapped = 0; // Флаг

        // Проходим по списку с шагом gap
        for (int i = 0; i < gap - 1; i++) {
            // Если текущий элемент меньше следующего, меняем их местами
            if (curr->value < next->value) {
                int temp = curr->value;
                curr->value = next->value;
                next->value = temp;
                swapped = 1;
            }
            // Переходим к следующей паре элементов
            curr = curr->next;
            next = next->next;
        }

        // Если не было перестановок, уменьшаем gap и продолжаем сортировку
        if (!swapped) {
            gap /= shrink;
            if (gap < 1) {
                gap = 1;
            }
        }
    }
}

void rus_lan() {
    setlocale(LC_ALL, "Rus");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
}
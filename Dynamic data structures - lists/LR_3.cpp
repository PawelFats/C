#include "LR_3.h"


int main()
{
    rus_lan();
    Node* head = NULL;
    int n, i, a, b, *mas, nn, m;

    printf("Введите кол-во узлов в списке: ");
    scanf("%d", &n);
    printf("Введите диапазон значений: от = ");
    scanf("%d", &a);
    printf("Введите диапазон значений: до = ");
    scanf("%d", &b);
    mas = malloc(n * sizeof(int));
    srand(time(NULL));
    for (i = 0; i < n; i++) {
        mas[i] = rand() % (b + 1 - a) + a;
    }
    //Создаём список из массива
    from_array(&head, mas, n);
    print_list(head);
    printf("\n");

    printf("Добавление новго узла после N-го узла\n");
    printf("Введите номер узлa = ");
    scanf("%d", &nn);
    printf("Введите значение которое будет храниться в узле = ");
    scanf("%d", &m);
    insert(head, nn, m);
    print_list(head);
    printf("\n");

    printf("Вставка любого значения в конец списка\n");
    printf("Введите значение которое хотите добавить в конец списка = ");
    scanf("%d", &m);
    push_back(head, m);
    print_list(head);
    printf("\n");

    printf("Удаление первого и последнего узла из списка\n");
    printf("Удалим в начале списка, значение = %d\n", pop(&head));
    printf("Удалим в конце списка, значение = %d\n", pop_back(&head));
    print_list(head);
    printf("\n");

    printf("Удаление N-го узла\n");
    printf("Введите номер узлa = ");
    scanf("%d", &nn);
    printf("Удалили из списка узел, со значением = ");
    delete_nth(&head, nn);
    print_list(head);
    printf("\n");

    printf("Редактирование N-го узла\n");
    printf("Введите номер узлa = ");
    scanf("%d", &nn);
    printf("Введите значение которое теперь будет храниться в узле = ");
    scanf("%d", &m);
    editing(&head, nn, m);
    print_list(head);
    printf("\n");

    printf("Соритруем список методом расчестки(соритровка вниз):\n");
    comb_sort(head);
    print_list(head);
    printf("\n");

    printf("Сортируем список методом пузырька(соритровка вверх):\n");
    bubbleSort(head);
    print_list(head);
    printf("\n");

    printf("Переворачиваем список\n");
    print_reverse(head);

    delete_list(&head);
    free(mas);
    _getch();
}
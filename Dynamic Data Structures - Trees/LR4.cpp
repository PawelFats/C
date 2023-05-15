#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>


typedef struct Node{
    int data;
    struct Node* left;
    struct Node* right;
    struct Node* parent;
    int height;
} Node;


Node* getTreeNode(Node*, int);
int Height(Node*), setBalance(Node**), Tsize = 0; //кол-во элементов в дереве;
void leftSwap(Node**), rightSwap(Node**), insert(Node**, int), output(Node*), clear(Node**), printTree(Node*, int), rus_lan();


int main()
{
    rus_lan();
    int d, n, a, b, * mas, choice;
    char ch; // continue
    Node* root = NULL;

    printf("Выберите способ ввода данных:\n");
    printf("1 - заполнение дерева рандомными числами\n");
    printf("2 - ввод чисел вручную\n");
    printf("3 - уже готовые значения\n");
    scanf("%d", &choice);
    switch (choice) {
    case 1:
        // Ввод диапазона и кол-во узлов дерева(заполнени случ.)
        printf("Введите кол-во узлов в списке: ");
        scanf("%d", &n);
        printf("Введите диапазон значений: от = ");
        scanf("%d", &a);
        printf("Введите диапазон значений: до = ");
        scanf("%d", &b);
        mas = malloc(n * sizeof(int));
        srand(time(NULL));
        for (int i = 0; i < n; i++) {
            mas[i] = rand() % (b + 1 - a) + a;
            insert(&root, mas[i]);
        }
        break;
    case 2:
        // Ввод значений узлов вручную
        do {
            printf("Введите число для вставки в узел = ");
            scanf("%d", &d);
            insert(&root, d);
            printf("Хотите ввести еще число?(Да - любой символ / Нет = ""n"")");
            scanf("%s", &ch);
        } while (ch != 'n');
        break;
    case 3:
        //Вывод уже готовых значений
        insert(&root, 25);
        insert(&root, 20);
        insert(&root, 32);
        insert(&root, 10);
        insert(&root, 22);
        insert(&root, 34);
        insert(&root, 28);
        insert(&root, 27);
        insert(&root, 30);
        /*insert(&root, 29);*/
        break;
    default:
        printf("Ошибка: неверный выбор.\n");
    }

    printf("--------------------------------------------\n");
    printf("Кол-во узлов в дереве = %d\n", Tsize);
    printf("--------------------------------------------\n");
    printf("Вывод дерева в симметричном порядке: \n");
    output(root);
    printf("\n--------------------------------------------\n");
    printf("Вывод дерева: \n");
    printTree(root, 0);
    printf("--------------------------------------------\n");
    clear(&root);
    printf("Дерево успешно очищенно!\n");

    _getch();
}

Node* getTreeNode(int value, Node* parent) {
    Node* tmp = (Node*)malloc(sizeof(Node));
    tmp->left = tmp->right = NULL;
    tmp->data = value;
    tmp->parent = parent;
    tmp->height = 0;
    return tmp;
}

int Height(Node* root) { //вычесляем высоту дерева
    if (root == NULL) return 0; //если пустое то = 0
    int hLeft = Height(root->left); //высота левого 
    int hRight = Height(root->right); //высота правого
    if (hLeft > hRight) { //если высота слева больше
        return hLeft + 1; //возвращаем ее + 1
    }
    else {
        return hRight + 1;
    }
}

int setBalance(Node** root) { //нахождение значения баланса для текущ. узла (разность высот правого и левого поддеревьев)
    if (*root != NULL) {
        (*root)->height = Height((*root)->right) - Height((*root)->left);
        return (*root)->height;
    }
}

void leftSwap(Node** root) {
    Node* r_swap, * r_swap_left;
    r_swap = (*root)->right;
    r_swap_left = r_swap->left;

    r_swap->left = (*root);
    (*root)->right = r_swap_left;
    (*root) = r_swap;
    setBalance(&(*root)->left);
    setBalance(root);
}

void rightSwap(Node** root) { //правый поворот
    Node* l_swap, * l_swap_right;//левое поддерево , правое поддерево левого поддерева
    l_swap = (*root)->left;
    l_swap_right = l_swap->right;

    l_swap->right = (*root); //теперь в лево-правом поддереве хранится корень
    (*root)->left = l_swap_right; //теперь в левом поддерево хранится значение лево-правого узла
    (*root) = l_swap; //в корне теперь у нас левое поддерево
    setBalance(&(*root)->right); //балансируем новое правое поддерево
    setBalance(root); //балансируем новый корень
}

void insert(Node** root, int value) { //добавление узла
    if (*root == NULL) {
        *root = getTreeNode(value, NULL); //Проверяем, если узла не существует, то создаем его
        Tsize++;
        return;
    }
    else { //текущий узел существует
        if (value > (*root)->data) { //идем в правое поддерево
            insert(&((*root)->right), value); //добавляем узел в парвое поддерево
            if ((setBalance(root)) > 1) { // если баланс АВЛ-дерева нарушен
                if (Height((*root)->right->right) < Height((*root)->right->left)) { //если нарушен баланс ребенка
                    rightSwap(&(*root)->right); //вращение вправо (вправо-лево)
                }
                leftSwap(root); //вращение влево

            }
        }
        else
            if (value < (*root)->data) { //идем в левое поддерево
                insert(&((*root)->left), value);
                if ((setBalance(root)) < -1) {
                    if (Height((*root)->left->left) < Height((*root)->left->right)) {
                        leftSwap(&(*root)->left);
                    }
                    rightSwap(root);
                }
            } //(Height((*root)->left) - Height((*root)->right)
        setBalance(root);//балансируем дерево
    }
}

void output(Node* p) { //печать в симметричном порядке
    if (p != NULL) {
        output(p->left);
        printf("%d ", p->data);
        output(p->right);
    }
}

void clear(Node** p) { //очистка
    if ((*p) != NULL) {
        clear(&(*p)->left);
        clear(&(*p)->right);

        free(*p);
        *p = NULL;
    }
}

void printTree(Node* root, int level) {
    if (root == NULL) {
        return;
    }
    printTree(root->right, level + 1);
    for (int i = 0; i < level; i++) {
        printf("         ");
    }
    printf("-------%d\n", root->data);
    printTree(root->left, level + 1);
}

void rus_lan() {
    setlocale(LC_ALL, "Rus");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
}



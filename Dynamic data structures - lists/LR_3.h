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


void push(Node** head, int data) {     //(������� � ������ ������) ��� ��� ��������� head ����������, �� ���������� ���������� ��������� �� ���������
    Node* tmp = malloc(sizeof(struct Node)); //�������� ������ ��� ����� ������ (��� ��������� (struct node))
    tmp->value = data; //������ � ����� ������ ������(�� ������� ��������� ���������) ������
    tmp->next = (*head); // ����������� ��������� tmp ����� ����������� ����
    (*head) = tmp; // ����������� ��������� head ����� ������ ����
}  //����� ������ �� ������� ���������� tmp ����� ����������

int pop(Node** head) { //�������� ������� ��������(�������� ����� ������)
    Node* prev = NULL;
    int val;
    if (head == NULL) exit(-1);
    prev = (*head); //��������� ����� ������� ����
    val = prev->value; //��������� ������ ������� ����
    (*head) = (*head)->next; //������������ ��������� head �� ��������� ����(��� ����� ������ ������ ���� �� ������)
    free(prev);// ������������ ������
    return val;
}

Node* get_nth(Node* head, int n) {//��������� ��������� �� n-�� ������� (��� ��� ������������ �����
    int counter = 0;//  ���� ���� Node �� ������� � ��� ��-�� ����� ��� node*
    while (counter < n && head) { //���� �� ����� �� n � ���� head != null(����� ������ ���� ����� ������ ������ ������)
        head = head->next;
        counter++;
    }
    return head;
}

Node* get_last(Node* head) {//��������� ��������� �� ��������� �������
    if (head == NULL) return NULL;
    while (head->next) head = head->next;//���� �� ����� �� ���������� ��������, ������� ����� ������� �� null
    return head;
}

void push_back(Node* head, int value) { //��������� ����� ������� � ����� ������
    Node* last = get_last(head);
    Node* tmp = malloc(sizeof(struct Node));
    tmp->value = value;
    tmp->next = NULL;
    last->next = tmp;//��� ��� �� ������ ��������� ������� ������ ������ ����������, ��� �� ���� ���������� node** head
}

void print_reverse(Node* head) { //������ ������ � ������
    if (head == NULL) {
        return;
    }
    print_reverse(head->next);
    printf("%d ", head->value);
}

int pop_back(Node** head) { //�������� ���������� ��������
    int val;
    Node* pFwd = NULL; //������� ����
    Node* pBwd = NULL; //���������� ���� 
    if (!head) exit(-1); //�������� NULL
    pFwd = *head;
    while (pFwd->next) { //���� ����������� NULL
        pBwd = pFwd;
        pFwd = pFwd->next;
    }
    val = pFwd->value;
    if (pBwd == NULL) { //���� � ������ ���� �������
        free(*head);
        *head = NULL;
    }
    else {
        free(pFwd->next);
        pBwd->next = NULL;
    }
    return val;
}

void insert(Node* head, int n, int val) {//��������� �� n-� ����� ����� ��������
    int i = 0;
    Node* tmp = (Node*)malloc(sizeof(Node));
    tmp->value = val;
    //������� ������ �������. ���� ����� �� ������� ������, �� ������� �� �����,
    //������ ������������� �� �����, ��������� ������� � �����
    while (i < n && head->next) {
        head = head->next;
        i++;
    }
    //���� ��� �� ��������� ������� 
    if (head->next) {
        tmp->next = head->next; //������ ���, ����� ����� ������� �������� �� ��������� ����� n-��
    }
    else {
        tmp->next = NULL;
    }
    head->next = tmp; //������������ ��������� next n-�� �������� �� ����� ��������� ����
}

int delete_nth(Node** head, int n) { //������� �������� n-�� ��������
    int val;
    if (n == 0) {
        return pop(head); //������� ������ �������
    }
    else {
        Node* prev = get_nth(*head, n - 1); //��������� �� n-1 �������
        Node* elm = prev->next; //��������� �� n �������(������� ����� �������)
        val = elm->value; //����� �� n ��������
        prev->next = elm->next; //������������ ��������� n-1 �������� �� n+1 (������� ������ ����� ��������)
        free(elm);
        printf("%d\n", val);

    }

}

void delete_list(Node** head) { //�������� ������ (����� ���� �������, ������� � ������ ������)
    Node* prev = NULL;
    while ((*head)->next) {
        prev = (*head);
        (*head) = (*head)->next;
        free(prev);
    }
    free(*head);
}

void from_array(Node** head, int* arr, int size) {// �������� ������ �� ������� ��� ������ push, ����� ������� � �����
    int i = size - 1;
    if (arr == NULL || size == 0) return;
    do {
        push(head, arr[i]);
    } while (i-- != 0);
}

void print_list(Node* head) { //����� ������
    while (head) {
        printf("%d ", head->value);
        head = head->next;
    }
    printf("\n");
}

void editing(Node** head, int n, int val) {
    Node* elm = get_nth(*head, n);
    printf("������ ��������: %d\n", elm->value);
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
        swapped = 0; //����
        ptr1 = head;
        while (ptr1->next != lptr) { //��������� ������������ �� �� ���� �������(����� ������� ���� � ������ �������� ������� �� ���� ������� ������ ���)
            if (ptr1->value > ptr1->next->value) { //��� ������
                int temp = ptr1->value;
                ptr1->value = ptr1->next->value;
                ptr1->next->value = temp;
                swapped = 1; //���� 1, ������ ��������� �����
            }
            ptr1 = ptr1->next; //������� �� �������� ����
        }
        lptr = ptr1; //����� ������������ ���� �����
    } while (swapped);
}

int length(Node* head) { //������� ����� ������
    int len = 0;
    Node* current = head;
    while (current != NULL) {
        len++;
        current = current->next;
    }
    return len;
}

void comb_sort(Node* head) {
    int gap = length(head); // ���������� ��������� gap ������ ����� ������
    float shrink = 1.3; // ����������� ���������� gap

    while (gap > 1) {
        // ������� � ��������� ��������� �� �������� ������
        Node* curr = head;
        Node* next = head->next;
        int swapped = 0; // ����

        // �������� �� ������ � ����� gap
        for (int i = 0; i < gap - 1; i++) {
            // ���� ������� ������� ������ ����������, ������ �� �������
            if (curr->value < next->value) {
                int temp = curr->value;
                curr->value = next->value;
                next->value = temp;
                swapped = 1;
            }
            // ��������� � ��������� ���� ���������
            curr = curr->next;
            next = next->next;
        }

        // ���� �� ���� ������������, ��������� gap � ���������� ����������
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
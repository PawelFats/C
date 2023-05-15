#define _CRT_SECURE_NO_WARNINGS

#include <locale.h>
#include <stdio.h>

void rus_lan();


int main() {
    rus_lan();
	int a, b, c, k, r, count = 0;
	

	printf("Введите размер емкости A: ");
	scanf("%d", &a);
	printf("Введите размер емкости B: ");
	scanf("%d", &b);
	printf("Введите размер емкости C: ");
	scanf("%d", &c);
	printf("Введите размер заполненной емкости : ");
	scanf("%d", &k);
	printf("Сколько литров необходимо отлить? Ввод : ");
	scanf("%d", &r);

	int state[3] = {k, 0, 0}, size[3] = {a, b, c};

	printf("------------------------------------------------\n");
	printf("|| ХОД || ЕМКОСТЬ A || ЕМКОСТЬ B || ЕМКОСТЬ С ||\n");


	while (state[0] != r && state[1] != r && state[2] != r) {

		count += 1;

		if (state[0] == size[0]) {
			while (state[0] != 0 && state[1] != size[1]) {
				state[0] -= 1;
				state[1] += 1;
			}
		}
		else if(state[1] == size[1]) {
			while (state[1] != 0 && state[2] != size[2]) {
				state[1] -= 1;
				state[2] += 1;
			}
		}
		else if (state[2] == size[2]) {
			while (state[2] != 0 && state[0] != size[0]) {
				state[2] -= 1;
				state[0] += 1;
			}
		}
		else if (state[0] != size[0] && state[2] != size[2] && state[1] == 0) {
			while (state[0] != 0 && state[1] != size[1]) {
				state[0] -= 1;
				state[1] += 1;
			}
		}
		else if (state[0] != size[0] && state[1] != size[1] && state[2] == 0) {
			while (state[1] != 0 && state[2] != size[2]) {
				state[1] -= 1;
				state[2] += 1;
			}
		}
		else if (state[2] != size[2] && state[1] != size[1] && state[0] == 0) {
			while (state[2] != 1 && state[0] != size[0]) {
				state[2] -= 1;
				state[0] += 1;
			}
		}
		
		printf("||%3d  ||%7d    ||%7d    ||%7d    ||\n",count, state[0], state[1], state[2]);
	}
	printf("------------------------------------------------\n");
	return 0;
}


 void rus_lan() {
	setlocale(LC_ALL, "Rus");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
}
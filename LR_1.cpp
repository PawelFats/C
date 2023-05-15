#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <Windows.h>

int get_guess_number(int, int), get_boo(float, int), update_score(int, float, int, float, float);
void rus_lan(), rules(char*), exit_game(char), hello();
float conversion(), get_attempts(char*), get_bet(char*, float), calc_win(float, float, float, float);

int main() {
	rus_lan();  //Подключение кириллицы

	char name[10], choose;
	float score, bet, win, attempts;
	int a_range, b_range, g_number, u_number, boo;

	hello();
	printf("--Введите свое имя: ");
	scanf("%10s", name);

	rules(name); //Правила

	score = conversion(); //Баланс
	
	while (1) {
		boo = 0;
		printf("--------------------------------------\n");
		printf("----------ПОДГОТОВКА К ИГРЕ!----------\n");
		printf("--------------------------------------\n");

		printf("----%s введите диапазон чисел: \n", name);
		printf("----Начальное число = ");
		scanf("%d", &a_range);
		printf("----Конечное число = ");
		scanf("%d", &b_range);

		attempts = get_attempts(name); //Ввод попыток

		bet = get_bet(name, score); //Ввод ставки

		win = calc_win(a_range, b_range, bet, attempts); // Расчет выйгрыша

		g_number = get_guess_number(a_range, b_range); //Загадываем число

		printf("--------------------------------------\n");
		printf("---------------НАЧИНАЕМ!--------------\n");
		printf("--------------------------------------\n");

		boo = get_boo(attempts, g_number); // Играем

		score = update_score(boo, score, g_number, win, bet); // Итог игры

		printf("----Вы хотите сыграть еще? (Д/д)");
		scanf("%s", &choose);
	
		exit_game(choose); //Выход

	}
	return 0;
}

void hello(){
	printf("--------------------------------------\n");
	printf("-------Добро пожаловать в игру--------\n");
	printf("-------------УГАДАЙ ЧИСЛО-------------\n");
	printf("----Получите 20$ за первый депозит----\n");
	printf("--------------------------------------\n");
	printf("-----Как я к вам могу обращаться?-----\n");
}

void exit_game(char choose) {
	if ((choose != 'Д') && (choose != 'д')) {
		printf("--------------------------------------\n");
		printf("-------------Заходите еще!------------\n");
		printf("----------------ВЫХОД-----------------\n");
		printf("--------------------------------------\n");
		Sleep(1000);
		exit(0);	
	}
}

int update_score(int boo, float score, int g_number, float win, float bet) {
	if (boo == 0) {
		printf("--------------------------------------\n");
		printf("----Попытки закончились, Вы проиграли.\n");
		printf("----Число которое мы загадали -- %d\n", g_number);
		score -= bet;
		printf("----На вашем счету доступно %.2f$\n", score);
	}
	else {
		printf("--------------------------------------\n");
		printf("---------Вы выйграли %.2f$-----------\n", win);
		score = score - bet + win;
		printf("----На вашем счету доступно %.2f$----\n", score);
		printf("--------------------------------------\n");
	}
	return score;
}

int get_boo(float attempts, int g_number) {
	int boo = 0, u_number;
	while (attempts > 0) {
		attempts -= 1;
		printf("----Введите число: ");
		scanf("%d", &u_number);
		if (u_number > g_number) printf("----Загаданное число меньше! У вас осталось %.0f попыток!\n", attempts);
		else if (u_number < g_number) printf("----Загаданное число больше! У вас осталось %.0f попыток!\n", attempts);
		else {
			boo = 1;
			break;
		}
	}
	return boo;
}

int get_guess_number(int a_range, int b_range) {
	srand(time(NULL));
	int number = rand() % (b_range + 1 - a_range) + a_range;
	Sleep(1000);
	return number;
}

float calc_win(float a_range, float b_range, float bet, float attempts) {
	float win;
	win = (((b_range - a_range)) / attempts) * bet;
	printf("Возможный выйгрыш %.2f$\n", win);
	return win;
}

float get_bet(char* name, float dollars) {
	float bet;
	do {
		printf("%s введите вашу ставку: ", name);
		if (scanf("%f", &bet) != 1 || bet <= 0 || bet > dollars) {
			printf("Ошибочный формат ставки! Повторите попытку!\n");
		}
	} while (bet <= 0 || bet > dollars);
	return bet;
}

float get_attempts(char* name) {
	float attempts;
	do {
		printf("%s введите количество попыток: ", name);
		if (scanf("%f", &attempts) != 1 || attempts < 1) {
			printf("Ошибочный формат количества попыток! Повторите попытку!\n");
		}
	} while (attempts < 1);
	return attempts;
}

float conversion() {
	float dollars, rubles;
	do {
		printf("--Введите кол-во рублей для конвертации и зачисления на игрвой счет : ");
		if (scanf("%f", &rubles) != 1 || rubles < 1) {
			printf("Ошибочный формат ввода! Повторите попытку!\n");
			fflush(stdin);
		}
	} while (rubles < 1);
	dollars = rubles / 76;
	printf("--Идет зачисление...\n");
	Sleep(1000);
	printf("--На счет зачисленно %.2f $ + 20$ за первый депозит, итого: %.2f $\n", dollars, dollars + 20);
	
	dollars += 20;
	return dollars;
}

void rules(char* name) {
	printf("--------------------------------------\n");
	printf("-------------ПРАВИЛА ИГРЫ-------------\n");
	printf("--В начале, вам необходимо задать следующие параметры:\n");
	printf("--1. Диапазон чисел;\n");
	printf("--2. Количество ваших попыток;\n");
	printf("--3. Размер ставки;\n");
	printf("--Далее мы вам показываем возможный выйгрыш и загадываем число в заданном диапазоне, вам остается лишь угадать число\n");
	printf("--за отведенное кол-во попыток. После каждого вашего хода вам будет дана подсказка!\n");
	printf("--%s ВАЖНО! В нашей игре принимаются ставки исключительно в доларах! Курс состовляет: 76 Р = 1 $\n", name);
	printf("--------------------------------------\n");
}

void rus_lan() {
	setlocale(LC_ALL, "Rus");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
}
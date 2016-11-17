// main.cpp : Defines the entry point for the console application.
//



#include "stdafx.h"
#include "gestorAtmosferico.h"
#include <algorithm>


#define INICIOX 9
#define INICIOY 13
#define FIN 80


class Personaje {
public:
	int x;
	int y;
	char * dibujo;
	Personaje(int px, int py, char * dib)
	{
		x = px;
		y = py;
		dibujo = dib;
	}

	void modificarX(int px) {
		x = px;
	}
};

class Bala {
public:
	int x;
	int y;
	char * dibujo;
	Bala(int px, int py, char * d)
	{
		x = px;
		y = py;
		dibujo = d;
	}

	void modificarX(int px) {
		x = px;
	}
};


void llover(std::vector<Gota>& lluvia, Escenario mundo);
void dibujarLuvia(std::vector<Gota> lluvia, Escenario mundo);
void secar(std::vector<Gota>& lluvia);
void dibujarEscenario(Escenario mundo);
void dibujarBala(std::vector<Bala> bala);
void moverBala(std::vector<Bala>& bala);
void dibujarPersonaje(Personaje pers);
void moverPersonaje(Personaje * pers, char * direccion);
void moverEnemigo(std::vector<Personaje>& enemigo);
void reaparicionEnemigo(Personaje pers, std::vector<Personaje>& enemigo);
void disparar(Personaje pers, std::vector<Bala>& balas, char * direccion);
void colisiones();
void hacerMundo();
void accionTecla();

Personaje prota(INICIOX, INICIOY, "*");
std::vector<Bala> balas;
std::vector<Personaje> enemigo;
Escenario escena(14);
std::vector<Gota> gotas;

void gotoxy(int x, int y)
{
	COORD pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(output, pos);
}

void dibujarEscenario(Escenario mundo)
{
	gotoxy(0, mundo.y);
	printf(mundo.escenario);
	return;
}

void llover(std::vector<Gota>& lluvia, Escenario mundo) {
	int indice = -2;
	int indiceEscenario = rand() % 79 + 1;

	indice = hacerGota(indiceEscenario, mundo);
	if (indice > 0)
		lluvia.push_back({ indice,TIEMPOSECADO,"g" });
}

void dibujarLuvia(std::vector<Gota> lluvia, Escenario mundo) {
	for (int i = 0; i < gotas.size(); i++) {
		gotoxy(lluvia[i].x, mundo.y);
		printf(lluvia[i].dibujo);
	}
}

void secar(std::vector<Gota>& lluvia) {
	for (int i = 0; i < lluvia.size(); i++) {
		secado(&lluvia[i]);
		if (lluvia[i].segundos < 0)
			lluvia.erase(gotas.begin() + i);
	}
}

void hacerMundo() {
	dibujarEscenario(escena);
	dibujarLuvia(gotas, escena);
	dibujarPersonaje(prota);


	moverBala(balas);
	dibujarBala(balas);

	moverEnemigo(enemigo);

	reaparicionEnemigo(prota, enemigo);
	for (int i = 0; i < enemigo.size(); i++)
		dibujarPersonaje(enemigo[i]);

}

void colisiones() {
	std::vector<int> indicesBalasDestruidos;
	std::vector<int> indicesEnemigosDestruidos;
	for (int j = 0; j < enemigo.size(); j++)
		for (int i = 0; i < balas.size(); i++)
			if (enemigo[j].x == balas[i].x || enemigo[j].x - 1 == balas[i].x || enemigo[j].x + 1 == balas[i].x) {
				if (std::find(indicesBalasDestruidos.begin(),indicesBalasDestruidos.end(),i))
				enemigo.erase(enemigo.begin() + j);
			}
	for (int i = 0; i < indicesBalasDestruidos.size(); i++) {
		balas.erase(balas.begin() + indicesBalasDestruidos[i]);
	}

	for (int i = 0; i < indicesBalasDestruidos.size(); i++) {
		enemigo.erase(enemigo.begin() + indicesEnemigosDestruidos[i]);
	}

	for (int i = 0; i < balas.size(); i++)
		if (balas[i].x <= 0 || balas[i].x > 80) {
			balas.erase(balas.begin() + i);
		}
}

void disparar(Personaje pers, std::vector<Bala>& bala, char * direccion)
{
	if (strcmp(direccion, "der") == 0)
		bala.push_back(Bala(pers.x + 1, pers.y, ">"));
	else if (strcmp(direccion, "izq") == 0)
		bala.push_back(Bala(pers.x - 1, pers.y, "<"));
	else return;
	return;

}

void dibujarPersonaje(Personaje pers)
{
	gotoxy(pers.x, pers.y);
	printf(pers.dibujo);
	return;
}

void dibujarBala(std::vector<Bala> bala) {
	for (int i = 0; i < bala.size(); i++) {
		gotoxy(bala[i].x, bala[i].y);
		printf(bala[i].dibujo);
	}
	return;
}

void moverBala(std::vector<Bala>& bala)
{
	for (int i = 0; i < bala.size(); i++) {
		if (strcmp(bala[i].dibujo, ">") == 0)
			bala[i].x++;
		else if (strcmp(bala[i].dibujo, "<") == 0)
			bala[i].x--;
		else return;
	}
	return;
}

void moverPersonaje(Personaje * pers, char * direccion)
{
	if (pers->x > 0 && pers->x < 80) {
		if (strcmp(direccion, "der") == 0)
			pers->x++;
		else if (strcmp(direccion, "izq") == 0)
			pers->x--;
	}
	else {
		if (pers->x == 0)
			pers->x++;
		else
			pers->x--;
	}
	return;
}

void moverEnemigo(std::vector<Personaje>& ene)
{

	int movimientoAleatorio;
	for (int i = 0; i < ene.size(); i++) {
		movimientoAleatorio = rand() % 2 + 1;
		if (ene[i].x > 0 && ene[i].x < FIN) {
			if (movimientoAleatorio == 1)
				ene[i].x++;
			else if (movimientoAleatorio == 2)
				ene[i].x--;
		}
		else {
			if (ene[i].x == 0)
				ene[i].x++;
			else
				ene[i].x--;
		}
	}
	return;
}

void reaparicionEnemigo(Personaje pers, std::vector<Personaje>& ene)
{
	int movimientoAleatorio = rand() % 79 + 1;
	int posibilidadAparacion = rand() % 100 + 1;
	if (posibilidadAparacion < 20 && ene.size() < 5)
		if (movimientoAleatorio != pers.x && movimientoAleatorio != pers.x + 1 && movimientoAleatorio != pers.x + 2 && movimientoAleatorio != pers.x - 1 && movimientoAleatorio != pers.x - 2) {
			ene.push_back(Personaje(movimientoAleatorio, INICIOY, "/"));
		}
	return;
}

void clear()
{
	system("cls");
}

void update() {
	clear();
	hacerMundo();
	colisiones();
	secar(gotas);
	llover(gotas, escena);
}

void accionTecla() {
	int c;
	if (_kbhit()) {
		c = _getch();
		if (c == 'a')
			moverPersonaje(&prota, "izq");
		else if (c == 'd')
			moverPersonaje(&prota, "der");
		else if (c == 'q')
			disparar(prota, balas, "izq");
		else if (c == 'e')
			disparar(prota, balas, "der");
		return;
	}
}

int main()
{
	srand(static_cast<unsigned int>(time(NULL)));
	while (1) {
		update();
		accionTecla();
		Sleep(100);
	}
	return 0;
}



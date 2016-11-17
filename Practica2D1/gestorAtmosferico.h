#pragma once

#define TIEMPOSECADO 30


class Escenario {
public:
	int y;
	char * escenario;

	Escenario(int py) {
		y = py;
		escenario = "--------------------------------------------------------------------------------";

	}
};

class Gota{
public:
	int x;
	int segundos;
	char * dibujo;
};

int hacerGota(int indiceEscenario, Escenario escenario);

int secado(Gota * gota);
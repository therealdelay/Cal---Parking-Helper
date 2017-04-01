#include <iostream>
#include <string>
#include <stdlib.h>
#include <cmath>
#include <fstream>
#include "Cidade.h"

using namespace std;


int getOption(int a, int b);

void clearBuffer();

void invalidOption();

bool refillTank();

void closestSpot();

void cheapestSpot();

int mainMenu(Cidade &c);

void readVertices();

void readRuas();

/*
void readArestas(){
	ifstream i("arestas.txt");
	char lixo;
	long long int rid, vsource, vdest;

}
*/

#include <iostream>
#include <string>
#include <stdlib.h>
#include <cmath>
#include <fstream>
#include "Cidade.h"
#include "util.h"

using namespace std;


int getOption(int a, int b);

void clearBuffer();

void invalidOption();

bool refillTank();

void closestSpot(Cidade &c);

void cheapestSpot(Cidade &c);

int mainMenu(Cidade &c);

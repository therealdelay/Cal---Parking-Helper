#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include "graphviewer.h"
#include "Graph.h"


class Cidade {
private:
	Graph<long long int> total;
	Graph<long long int> parcial;

	const static float latmax = 40.86086;
	const static float latmin = 40.8481;
	const static float lonmax = 8.403235;
	const static float lonmin = 8.387305;
public:
	Cidade(){}
	void readFromFile();
	int resizeLat(double lat);
	int resizeLong(double lon);
	float Haversine(double idNoOrigem, double idNoDestino);
};
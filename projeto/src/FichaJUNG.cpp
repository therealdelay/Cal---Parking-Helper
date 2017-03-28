#include <cstdio>
#include "graphviewer.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cmath>
/*
template <class T>
void exercicio3();*/

static float latmax = 40.86086;
static float latmin = 40.8481;
static float lonmax = 8.403235;
static float lonmin = 8.387305;

int resizeLat(double lat) {
	return (600 - (round(600 / (latmax - latmin) * (lat - latmin))));
}

int resizeLong(double lon) {
	return (round(600 / (lonmax - lonmin) * (lon - lonmin)));
}

void exercicio3()
{
	GraphViewer<unsigned> *gv = new GraphViewer<unsigned>(600, 600, false);

	gv->createWindow(600, 600);

	gv->defineEdgeColor("blue");
	gv->defineVertexColor("yellow");

	ifstream inFile;

	//Ler o ficheiro nos.txt
	inFile.open("vertices.txt");

	if (!inFile) {
		cerr << "Unable to open file datafile.txt";
		exit(1);   // call system to stop
	}

	std::string   line;

	long long int idNo=0;
	double X=0;
	double Y=0;
	double Xrad = 0;
	double Yrad = 0;
	string type = "";

	while(std::getline(inFile, line))
	{
		std::stringstream linestream(line);
		std::string         data;
		string lixo;

		linestream >> idNo;

		std::getline(linestream, data, ';');  // read up-to the first ; (discard ;).
		linestream >> X;
		std::getline(linestream, data, ';');  // read up-to the first ; (discard ;).
		linestream >> Y;
		std::getline(linestream, data, ';');
		linestream >> Xrad;
		std::getline(linestream, data, ';');
		linestream >> Yrad;
		linestream >> type;
		Vertex<long long int> *v(idNo);
		v->X = X;
		v->Y = Y;
		v->type = type;
		gv->addNode(v);

	}

	inFile.close();


	//Ler o ficheiro arestas.txt
	inFile.open("arestas0.txt");

	if (!inFile) {
		cerr << "Unable to open file datafile.txt";
		exit(1);   // call system to stop
	}

	int idAresta=0;
	long long int idNoOrigem=0;
	long long int idNoDestino=0;
	int counter = 0 ;

	while(std::getline(inFile, line))
	{
		std::stringstream linestream(line);
		std::string data;


		linestream >> idAresta;

		std::getline(linestream, data, ';');  // read up-to the first ; (discard ;).
		linestream >> idNoOrigem;
		std::getline(linestream, data, ';');  // read up-to the first ; (discard ;).
		linestream >> idNoDestino;
		gv->addEdge(counter,idNoOrigem,idNoDestino, EdgeType::UNDIRECTED);
		counter++;
	}

	inFile.close();

	gv->rearrange();
}
//template <class T>
//void fack(){
//	exercicio3();
//}

int main() {
	exercicio3();
	//fack();
	getchar();
	return 0;
}

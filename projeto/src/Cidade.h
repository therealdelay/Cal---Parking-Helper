#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include "graphviewer.h"
#include "Graph.h"
#include "ParkingSpot.h"


class Cidade {
private:
	GraphViewer *gv;
	Graph<long long int> total;
	Graph<long long int> parcial;
	vector<ParkingSpot> parkingSpots;
	vector<long long int> gasSpots;
	vector<long long int> lastPath;      //Isto e para limpar o trajeto anterior...
	map<string, long long int> spots;

	const static float latmax = 40.86086;
	const static float latmin = 40.8481;
	const static float lonmax = 8.403235;
	const static float lonmin = 8.387305;
	const static string defaultVertexColor;
	const static string defaultEdgeColor;
	const static string parkingSpotVertexColor;
	const static string pathEdgeColor;
	const static string srcVertexColor;
	const static string destVertexColor;

	void setPath(vector<long long int> path, string srcColor, string destColor, string edgeColor);
	void setPath(vector<long long int> path, string vertexColor, string edgeColor);
	vector<long long int> getPath(const long long int &src, const long long int &dest);
	long long int getClosestParkingSpot(const long long int &src);
	long long int getCheapestParkingSpot(const long long int &src, const long long int &dest);
	long long int getClosestGasStationSpot(const long long int &src);
	void clearGraphViewer();

public:
	Cidade(){}
	void readFromFile();
	int resizeLat(double lat);
	int resizeLong(double lon);
	float Haversine(double idNoOrigem, double idNoDestino);
	int getClosestRoute(const long long int &src, const string dest, bool gas);
	int getCheapestRoute(const long long int &src, string dest, bool gas);

};

#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <string.h>
#include "graphviewer.h"
#include "Graph.h"
#include "util.h"
#include "ParkingSpot.h"

/**
 * Class Cidade keeps information about the city, in particular it's streets, vertices, parking spots, gas spots and bus stops.
 * It also deals with everything related to finding a path from a place to another, chosen by the user. It is possible to choose
 * between the closest path and the cheapest path. Furthermore, it also paints on the screen the graphic, using the GraphicViewer.
 */
class Cidade {
private:
	GraphViewer *gv; ///< The GraphicViewer to draw the graph
	Graph<int> total; ///< Keeps the graph with the information about all the streets and vertices
	vector<ParkingSpot> parkingSpots; ///< Keeps all the parking spots of the map
	vector<int> gasSpots; ///< Keeps all the petrol Stations of the map
	vector<int> lastPath; ///< Serves to delete the last painted path on the map
	vector<Vertex<int> *> busStops; ///< Keeps all the bus stops of the map
	map<string, int> spots; ///<

	const static float latmax = 40.86086;
	const static float latmin = 40.8481;
	const static float lonmax = 8.403235;
	const static float lonmin = 8.387305;
	const static string defaultVertexColor;
	const static string defaultEdgeColor;
	const static string srcVertexColor;
	const static string destVertexColor;
	const static string parkingSpotVertexColor;
	const static string busStopVertexColor;
	const static string gasStationVertexColor;
	const static string drivingPathEdgeColor;
	const static string gasPathEdgeColor;
	const static string walkPathEdgeColor;
	const static string busPathEdgeColor;
	static double maxDist;
	static double mileage;
	static double busTicket;
	static unsigned int edgeNumber;
	static unsigned int lastedgeNumber;

	void setPath(vector<int> path, string srcColor, string destColor, string edgeColor);
	void setPath(vector<int> path, string vertexColor, string edgeColor);
	void setPathWalk(vector<int> path, string srcColor, string destColor, string edgeColor);
	vector<int> getPath(const int &src, const int &dest, double &dist);
	int getClosestParkingSpot(const int &src);
	int getClosestParkingSpotDest(const int &dest);
	int getClosestBusStop(const int &src);
	int getClosestBusStopDest(const int &dest);
	int getCheapestParkingSpotDest(const int &dest);
	int getClosestGasStationSpot(const int &dest);
	void clearGraphViewer();

public:
	Cidade(){}
	void static setMaxDist(double max);
	void static setMileage(double mil);
	void readFromFile();
	int resizeLat(double lat);
	int resizeLong(double lon);
	float Haversine(double idNoOrigem, double idNoDestino);
	vector<int> getBusPath(const int &src, const int &dest, double &dist);
	int getClosestRoute(const int &src, const string dest, bool gas);
	int getCheapestRoute(const int &src, string dest, bool gas);
};

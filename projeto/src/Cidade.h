#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <algorithm>
#include "graphviewer.h"
#include "Graph.h"
#include "util.h"
#include "ParkingSpot.h"
#include "matcher.h"

/**
 * Class Cidade keeps information about the city, in particular it's streets, vertexes, parking spots, gas spots and bus stops.
 * It also deals with everything related to finding a path from a place to another, chosen by the user. It is possible to choose
 * between the closest path and the cheapest path. Furthermore, it also paints on the screen the graphic, using the GraphicViewer.
 */
class Cidade {
private:
	GraphViewer *gv; ///< The GraphicViewer to draw the graph
	Graph<int> total; ///< Keeps the graph with the information about all the streets and vertexes
	vector<ParkingSpot> parkingSpots; ///< Keeps all the parking spots of the map
	vector<int> gasSpots; ///< Keeps all the petrol Stations of the map
	vector<int> lastPath; ///< Serves to delete the last painted path on the map
	vector<Vertex<int> *> busStops; ///< Keeps all the bus stops of the map
	map<string, int> spots; ///< map the type of the vertex to the correspondent id
	vector<string> streets;
	vector< vector<int> > streetParkingSpots;

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
	static double maxDist; ///< max distance to walk on foot or max distance from the cheapest parkingspot to the destiny
	static double mileage; ///< quantity of petrol that the user's car consumes per unity of distance
	static double busTicket; ///< price of the bus ticket
	static unsigned int edgeNumber;
	static unsigned int lastedgeNumber;

	bool vertexExists(int id);

	bool isParkingSpot(int id);
	/**
	 * This function paints, using the graphicviewer, the chosen path from the source vertex to the destiny vertex.
	 * It has specific colours to the source vertex, the path vertex and the destiny vertex.
	 *
	 * @param path is the chosen path to paint
	 * @param srcColor is the colour to paint the source vertex with
	 * @param destColor is the colour to paint the destiny vertex with
	 * @param edgeColor is the colour to paint the edges with
	 */
	void setPath(vector<int> path, string srcColor, string destColor, string edgeColor);
	/**
	 *  This function paints, using the graphicViewer, the chosen path with a given colour.
	 *
	 *  @param path is the chosen path to paint
	 *  @param vertexColor is the colour to paint the vertexes with
	 *  @param edgeColor is the colour to paint the edges with
	 */
	void setPath(vector<int> path, string vertexColor, string edgeColor);
	/**
	 * This function paints the walking path. It creates a new edge, a dashed one, that is undirected, and paints it.
	 *
	 * @param path is the chosen path to paint
	 * @param srcColor is the colour to paint the source vertex with
	 * @param destColor is the colour to paint the destiny vertex with
	 * @param edgeColor is the colour to paint the edges with
	 */
	void setPathWalk(vector<int> path, string srcColor, string destColor, string edgeColor);
	/**
	 * This function runs the dijkstraShortestPath algorithm and returns a vector with the id's of the vertexes
	 * that represent the path with the minimum distance from the src to the destiny. It also saves in the dist variable the distance from
	 * the src to the vertex closest to it.
	 *
	 * @param src is the vertex where we start
	 * @param dest is the vertex where we want to go
	 * @param dist is the minimum distance between the src and the dest
	 *
	 * @return vector of ints that represent the id's of the vertexes that represent the closest path.
	 */
	vector<int> getPath(const int &src, const int &dest, double &dist);
	/**
	 *	This function returns the id of the closest parking spot to the dest. If there isn't one, returns -1.
	 *
	 *	@param dest is the id of the vertex that will serve as destiny to calculate the path from the parking spot
	 *
	 *	@return -1 if parking spot isn't found, id of the closest parking spot if successful
	 */
	int getClosestParkingSpotDest(const int &dest);
	/**
	 * This function returns the id of the closest bus spot to the src. If there isn't one, returns -1.
	 *
	 * @param src is the id of the vertex that will serve as origin to calculate the path to the bus spot
	 *
	 * @return -1 if bus spot isn't found, id of the closest bus spot if successful
	 */
	int getClosestBusStop(const int &src);
	/**
	 *	This function returns the id of the closest bus spot to the dest. If there isn't one, returns -1.
	 *
	 *	@param dest is the id of the vertex that will serve as destiny to calculate the path from the bus spot
	 *
	 *	@return -1 if bus spot isn't found, id of the closest bus spot if successful
	 */
	int getClosestBusStopDest(const int &dest);
	/**
	 *	This function returns the id of the cheapest parking spot to the dest. If there isn't one or if it is
	 *	to far away (distance is bigger than the maxDist chosen by the user), returns -1.
	 *
	 *	@param dest is the id of the vertex that will serve as destiny to calculate the path from the parking spot
	 *
	 *	@return -1 if parking spot isn't found, id of the cheapest parking spot if successful
	 */
	int getCheapestParkingSpotDest(const int &dest);
	/**
	 *	This function returns the id of the closest gas spot to the dest. If there isn't one, returns -1.
	 *
	 *	@param dest is the id of the vertex that will serve as destiny to calculate the path from the gas spot
	 *
	 *	@return -1 if gas spot isn't found, id of the closest gas spot if successful
	 */
	int getClosestGasStationSpot(const int &dest);

	void setStreetPath(const unsigned int &streetIndex);
	/**
	 * This function clears the graphicviewer, that is, it sets all the colours to the default ones.
	 */
	void clearGraphViewer();

public:
	/**
	 * Class constructor
	 */
	Cidade(){}
	/**
	 * This function sets the attribute maxDist with the valor of the variable max.
	 *
	 * @param max is the maxDist chosen by the user
	 */
	void static setMaxDist(double max);
	/**
	 * This function sets the attribute mileage with the valor of the variable mil.
	 *
	 * @param mil is the quantity of petrol/km that the user's car consumes.
	 */
	void static setMileage(double mil);
	/**
	 * This function reads all the vertexes and edges from the .txt files and saves them in a Graph. It also
	 * identifies the parking spots, the interest points, the petrol stations and ht bus stops.
	 */
	void readFromFile();
	/**
	 * This function changes the distance between vertexes so they can be properly painted by the graphicviewer.
	 * If it wasn't made, the vertexes were painted one above the others.
	 *@param lat is the latitude (in degrees)
	 */
	int resizeLat(double lat);
	/**
	 * This function changes the distance between vertexes so they can be properly painted by the graphicviewer.
	 * If it wasn't made, the vertexes were painted one above the others.
	 *@param lon is the longitude (in degrees)
	 */
	int resizeLong(double lon);
	/**
	 * This function calculates the distance between the source and the destiny (idNoOrigem, idNoDestino).
	 * It calculates using the angles in radians and the haversine algorithm.
	 *
	 * @param idNoOrigem is the ID of the source vertex
	 * @param idNoDestino is the ID of the destiny vertex
	 *
	 * @return distance between the source and the destiny
	 */
	float Haversine(double idNoOrigem, double idNoDestino);
	/**
	 * This function calculates the path of the bus, from the source vertex to the destiny vertex. It calculates
	 * the stops the bus has to made and returns the path that goes through all the stops from the src to the dest.
	 *
	 * @param src is the ID of the source vertex
	 * @param dest is the ID of the destiny vertex
	 * @param dist is the distance of the path from the src to the dest
	 *
	 * @return vector that contains the ID of the vertexes that represent the path of the bus
	 */
	vector<int> getBusPath(const int &src, const int &dest, double &dist);
	/**
	 * This function calculates the closest route from the source to the destiny. The source consists of a vertex
	 * chosen by the user and the destiny consist of a interest point(restaurant, cafe, shop...) also chosen by the user.
	 * The user can also choose if it needs to fill the deposit of it's car. The path is calculated by going through
	 * all the possibilities of each part of the process and choosing the one with less distance.
	 *
	 * @param src is the ID of the source vertex
	 * @param dest is the name of the destiny vertex
	 * @param gas is a boolean that represent if the user wants to go to a gas(petrol) station or not
	 *
	 * @return 1 if error, 0 if success
	 */
	int getClosestRoute(const int &src, const string dest, bool gas);
	/**
	 * This function calculates the cheapest path from the source to the destiny. The source consists of a vertex
	 * chosen by the user and the destiny consist of a interest point(restaurant, cafe, shop...) also chosen by the user.
	 * The user can also choose if it needs to fill the deposit of it's car. The path is calculated by going through all the
	 * possibilites and calculating the cheapest path by car, that is, the closest one (closest means less money spent on gas(petrol)).
	 * Then it is calculated the path from the source to the destiny by bus. If the bus path is cheaper, then this is the path returned.
	 * Even if the bus path is the cheapest, the path calculated can go through a gas(petrol) station, if the user has chosen so.
	 *
	 * @param src is the ID of the source vertex
	 * @param dest is the name of the destiny vertex
	 * @param gas is a boolean that represent if the user wants to go to a gas(petrol) station or not
	 *
	 * @return 1 if error, 0 if success
	 */
	int getCheapestRoute(const int &src, string dest, bool gas);


	struct sort_first {
	    bool operator()(const std::pair<float,int> &left, const std::pair<float,int> &right) {
	        return left.first < right.first;
	    }
	};

	void exactStreetSearch(string toSearch);

	void aproxStreetSearch(string toSearch);
};

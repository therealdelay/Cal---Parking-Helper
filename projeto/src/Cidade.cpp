#include "Cidade.h"
using namespace std;

const string Cidade::defaultVertexColor = "yellow";
const string Cidade::defaultEdgeColor = "blue";
const string Cidade::srcVertexColor = "blue";
const string Cidade::destVertexColor = "green";
const string Cidade::parkingSpotVertexColor = "red";
const string Cidade::busStopVertexColor = "orange";
const string Cidade::gasStationVertexColor = "black";
const string Cidade::drivingPathEdgeColor = "red";
const string Cidade::gasPathEdgeColor = "black";
const string Cidade::walkPathEdgeColor = "green";
const string Cidade::busPathEdgeColor = "orange";
double Cidade::maxDist= 0;
double Cidade::mileage= 0;
double Cidade::busTicket = 1.00;
unsigned int Cidade::edgeNumber = 0;
unsigned int Cidade::lastedgeNumber = 0;


int Cidade::resizeLat(double lat) {
	return (600 - (round(600 / (latmax - latmin) * (lat - latmin))));
}

int Cidade::resizeLong(double lon) {
	return (round(600 / (lonmax - lonmin) * (lon - lonmin)));
}

void Cidade::setMaxDist(double max){
	maxDist = max;
}

void Cidade::setMileage(double mil) {
	mileage = mil;
}

float Cidade::Haversine(double idNoOrigem, double idNoDestino){
	typename vector<Vertex<int>*>::iterator it= total.getVertexSet().begin();
	typename vector<Vertex<int>*>::iterator ite= total.getVertexSet().end();
	double lat1, lat2, lon1, lon2;
	bool coord1 = false, coord2 = false;
	for (; it!=ite; it++){
		if ((*it)->getId() == idNoOrigem){
			lat1 = (*it)->getXrad();
			lon1= (*it)->getYrad();
			coord1 = true;
		}

		if ((*it)->getId() == idNoDestino){
			lat2 = (*it)->getXrad();
			lon2= (*it)->getYrad();
			coord2 = true;
		}

		if(coord1 && coord2)
			break;
	}
	float R = 6371; //raio da terra
	float dLat = lat2-lat1;
	float dLon = lon2-lon1;
	float a = sin(dLat/2) * sin(dLat/2) + cos(lat1) * cos(lat2) * sin(dLon/2) * sin(dLon/2);
	float c = 2 * atan2(sqrt(a), sqrt(1-a));
	float d = R*c;
	return d;
}

void Cidade::readFromFile()
{
	gv = new GraphViewer(600, 600, false);
	gv->createWindow(600, 600);
	gv->defineEdgeColor(defaultEdgeColor);
	gv->defineVertexColor(defaultVertexColor);

	ifstream inFile;
	//Ler o ficheiro nos.txt
	inFile.open("vertices.txt");
	if (!inFile) {
		cerr << "Unable to open file datafile.txt";
		exit(1);   // call system to stop
	}
	std::string   line;

	int idNo = 0;
	double X=0;
	double Y=0;
	double Xrad = 0;
	double Yrad = 0;
	string type = "";


	while(std::getline(inFile, line))
	{
		std::stringstream linestream(line);
		std::string data;
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
		linestream.ignore(255, ';');
		getline(linestream, type, ';');
		if(type != ""){
			if(type[0] != ' ' && type != "Estacionamento" && type != "Bombas de Gasolina" && type != "Paragem")
				spots[type] = idNo;
		}
		total.addVertex(idNo, X, Y, Xrad, Yrad, type);
		if(type == "Estacionamento") {
			double price;
			linestream >> price;
			parkingSpots.push_back(ParkingSpot(idNo,price));
		}
		if(type == "Bombas de Gasolina"){
			gasSpots.push_back(idNo);
		}
	}

	int in[] = { 492,290,276,45,350,370,88,105,113,122,450,469,439,16,407,158,168,177,226,266,245,320 };
	for(unsigned int l = 0; l < 22; l++){
		Vertex<int> *v = total.getVertex(in[l]);
		busStops.push_back(v);
	}

	for(unsigned int i = 0; i < total.getVertexSet().size(); i++){
		gv->addNode(total.getVertexSet()[i]->getId(), resizeLat(total.getVertexSet()[i]->getX()), resizeLong(-total.getVertexSet()[i]->getY()));
		gv->setVertexLabel(total.getVertexSet()[i]->getId(), total.getVertexSet()[i]->getType());
	}
	inFile.close();

	//Ler o ficheiro arestas.txt
	inFile.open("arestas0.txt");
	if (!inFile) {
		cerr << "Unable to open file datafile.txt";
		exit(1);   // call system to stop
	}
	int idAresta=0;
	int idNoOrigem=0;
	int idNoDestino=0;
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
		total.addEdge(counter,idNoOrigem, idNoDestino,Haversine(idNoOrigem, idNoDestino));
		gv->addEdge(counter, idNoOrigem, idNoDestino, EdgeType::DIRECTED);
		counter++;
		total.addEdge(counter,idNoDestino, idNoOrigem,Haversine(idNoOrigem, idNoDestino));
		gv->addEdge(counter, idNoDestino, idNoOrigem, EdgeType::DIRECTED);
		counter++;
	}

	edgeNumber = counter;
	lastedgeNumber = edgeNumber+1;
	inFile.close();
	gv->rearrange();
}

void Cidade::setPath(vector<int> path, string srcColor, string destColor, string edgeColor){
	if(path.size() == 0)
		return;
	gv->setVertexColor(path[0], srcColor);
	for(unsigned int i = 0; i < path.size()-1; i++){
		Vertex<int> *base = total.getVertex(path[i]);
		for(unsigned int j = 0; j < base->getAdj().size(); j++){
			if(base->getAdj()[j].getDest()->getId() == path[i+1]){
				gv->setEdgeColor(base->getAdj()[j].getId(), edgeColor);
			}
		}
	}
	gv->setVertexColor(path[path.size()-1], destColor);
}

void Cidade::setPath(vector<int> path, string vertexColor, string edgeColor){
	for(unsigned int i = 0; i < path.size(); i++){
		gv->setVertexColor(path[i], vertexColor);
		Vertex<int> *base = total.getVertex(path[i]);
		for(unsigned int j = 0; j < base->getAdj().size(); j++){
			if(base->getAdj()[j].getDest()->getId() == path[i+1]){
				gv->setEdgeColor(base->getAdj()[j].getId(), edgeColor);
			}
		}
	}
}

void Cidade::setPathWalk(vector<int> path, string srcColor, string destColor, string edgeColor){
	if(path.size() == 0)
		return;
	int counter = lastedgeNumber;
	gv->setVertexColor(path[0], srcColor);
	for(unsigned int i = 0; i < path.size()-1; i++){
		gv->addEdge(counter, path[i], path[i+1], EdgeType::UNDIRECTED);
		gv->setEdgeColor(counter, edgeColor);
		gv->setEdgeDashed(counter, true);
		counter++;
	}
	gv->setVertexColor(path[path.size()-1], destColor);
	lastedgeNumber = counter;
}

vector<int> Cidade::getPath(const int &src, const int &dest, double &dist){
	total.dijkstraShortestPath(src);
	Vertex<int> *t = total.getVertex(dest);
	dist = t->getDist();
	return total.getPath(src, dest);
}

int Cidade::getClosestParkingSpotDest(const int &dest){
	Vertex<int> *d = total.getVertex(dest);
	if(d == NULL)
		return -1;
	double minDist = INT_INFINITY;
	unsigned int min = 0;
	for(unsigned int i = 0; i < parkingSpots.size(); i++){
		total.dijkstraShortestPath(parkingSpots[i].getId());
		if(d->getDist() < minDist){
			min = i;
			minDist = d->getDist();
		}
	}
	if(minDist == INT_INFINITY)
		return -1;
	else
		return parkingSpots[min].getId();
}

int Cidade::getClosestBusStop(const int &src) {
	Vertex<int> *t = total.getVertex(src);
	if(t == NULL)
		return -1;

	total.dijkstraShortestPath(src);

	double minDist = INT_INFINITY;
	unsigned int min = 0;
	for(unsigned int i = 0; i < busStops.size(); i++){
		//cout << busStops[i]->getDist() << endl;
		if(busStops[i]->getDist() < minDist){
			min = i;
			minDist = busStops[i]->getDist();
		}
	}

	if(minDist == INT_INFINITY)
		return -1;
	else
		return busStops[min]->getId();
}

int Cidade::getClosestBusStopDest(const int &dest){
	//VERIFICAR SE A SRC EXISTE
	Vertex<int> *d = total.getVertex(dest);
	if(d == NULL)
		return -1;
	double minDist = INT_INFINITY;
	unsigned int min = 0;
	for(unsigned int i = 0; i < busStops.size(); i++){
		total.dijkstraShortestPath(busStops[i]->getId());
		if(d->getDist() < minDist){
			min = i;
			minDist = d->getDist();
		}
	}
	if(minDist == INT_INFINITY)
		return -1;
	else
		return busStops[min]->getId();
}

int Cidade::getCheapestParkingSpotDest(const int &dest) {
	Vertex<int> *t = total.getVertex(dest);
	if(t == NULL)
		return -1;
	total.dijkstraShortestPath(dest);
	double minPrice = INT_INFINITY;
	int id = 0;
	for(unsigned int i = 0; i < parkingSpots.size(); i++){
		Vertex<int> *v = total.getVertex(parkingSpots[i].getId());
		if(v == NULL)
			return -1;
		if((parkingSpots[i].getPrice() < minPrice) && (v->getDist() < maxDist)){
			minPrice = parkingSpots[i].getPrice();
			id = parkingSpots[i].getId();
		}
	}
	if(minPrice == INT_INFINITY)
		return -1;
	else
		return id;
}

int Cidade::getClosestGasStationSpot(const int &dest){
	//VERIFICAR SE A SRC EXISTE
	Vertex<int> *d = total.getVertex(dest);
	if(d == NULL)
		return -1;
	double minDist = INT_INFINITY;
	unsigned int min = 0;
	for(unsigned int i = 0; i < gasSpots.size(); i++){
		total.dijkstraShortestPath(gasSpots[i]);
		if(d->getDist() < minDist){
			min = i;
			minDist = d->getDist();
		}
	}
	if(minDist == INT_INFINITY)
		return -1;
	else
		return gasSpots[min];
}

void Cidade::clearGraphViewer(){
	if(lastPath.size() == 0)
		return;
	setPath(lastPath, defaultVertexColor, defaultEdgeColor);
	for(unsigned int i = lastedgeNumber; i > edgeNumber; i--)
		gv->removeEdge(i);
	lastedgeNumber = edgeNumber+1;
}

vector<int> Cidade::getBusPath(const int &src, const int &dest, double &dist){
	vector<int> path;
	int sIndex = -1, dIndex = -1;
	for(unsigned int i = 0; i < busStops.size(); i++){
		if(src == busStops[i]->getId())
			sIndex = i;
		if(dest == busStops[i]->getId())
			dIndex = i;
		if(sIndex !=-1 && dIndex !=-1){
			break;
		}
	}
	if(sIndex == -1 || dIndex == -1)
		return path;
	if(sIndex == dIndex){
		path.push_back(busStops[sIndex]->getId());
		return path;
	}
	int dir;
	if(sIndex < dIndex)
		dir = 1;
	else
		dir = -1;

	double distTotal = 0;
	double distmp ;
	int i = sIndex+dir;
	while(true){
		if(dir == 1){
			if(i > dIndex)
				break;
		}
		else{
			if(i < dIndex)
				break;
		}
		vector<int> tmp = getPath(busStops[i-dir]->getId(), busStops[i]->getId(), distmp);
		if(distmp == 0)
			break;
		if(path.size() != 0)
			path.pop_back();
		path.insert(path.end(), tmp.begin(), tmp.end());
		distTotal += distmp;
		i += dir;
	}
	return path;
}

int Cidade::getClosestRoute(const int &src, string dest, bool gas){
	map<string, int>::const_iterator it = spots.begin();
	for(; it != spots.end(); it++){
		if(compareNames(dest.c_str(), it->first.c_str()) == 0)
			break;
	}
	if(it == spots.end())
		return 1;
	double trash;
	double dist;

	//CLEAR VIEWER
	clearGraphViewer();

	vector<int> gasPath;
	int gasid;

	int parkingid = getClosestParkingSpotDest(it->second);
	if(parkingid == -1)
		return 1;

	if(gas){
		gasid = getClosestGasStationSpot(parkingid);
		if(gasid == -1)
			return 1;
		gasPath= getPath(src, gasid, trash);
	}

	vector<int> walkPath = getPath(parkingid, it->second,dist);

	vector<int> drivingPath;
	if(gas)
		drivingPath = getPath(gasid, parkingid, trash);
	else
		drivingPath = getPath(src, parkingid, trash);
	if(drivingPath.size() == 1)
		return 1;

	if(dist > maxDist){
		cout << "Minimum walking distance " << dist << " is too long\n";
		gv->rearrange();
		return 1;
	}

	if(gas){
		setPath(gasPath, srcVertexColor, gasStationVertexColor, gasPathEdgeColor);
		setPath(drivingPath, gasStationVertexColor, destVertexColor, drivingPathEdgeColor);
		setPathWalk(walkPath, parkingSpotVertexColor, destVertexColor, walkPathEdgeColor);
		lastPath = gasPath;
		lastPath.pop_back();
		lastPath.insert(lastPath.end(), drivingPath.begin(), drivingPath.end()-1);
		lastPath.insert(lastPath.end(), walkPath.begin(), walkPath.end());
	}
	else{
		setPathWalk(walkPath,  parkingSpotVertexColor, destVertexColor, walkPathEdgeColor);
		setPath(drivingPath, srcVertexColor, parkingSpotVertexColor, drivingPathEdgeColor);
		lastPath = drivingPath;
		lastPath.pop_back();
		lastPath.insert(lastPath.end(), walkPath.begin(), walkPath.end());
	}

	gv->rearrange();
	return 0;
}

int Cidade::getCheapestRoute(const int &src, string dest, bool gas){
	map<string, int>::const_iterator it = spots.begin();
	for(; it != spots.end(); it++){
		if(compareNames(dest.c_str(), it->first.c_str()) == 0)
			break;
	}
	if(it == spots.end())
		return 1;

	//CLEAR GRAPHVIEWER
	clearGraphViewer();

	double distmp;
	double distgas = 0;
	double distbus;
	double distTotal = 0;
	vector<int> gasPath;
	vector<int> drivingPath;
	vector<int> drivingPathBus;
	vector<int> busPath;
	vector<int> finalPath;
	vector<int> walkPath;
	vector<int> walkPath2;
	int gasid;

	int parkingid = getCheapestParkingSpotDest(it->second);
	if(parkingid == -1){
		cout << "Walking distance is too long\n";
		gv->rearrange();
		return 1;
	}

	if(gas){
		gasid = getClosestGasStationSpot(src);
		if(gasid == -1)
			return 1;
		gasPath = getPath(src, gasid, distgas);
		drivingPath = getPath(gasid, parkingid,distmp);
	}
	else
		drivingPath = getPath(src, parkingid,distmp);

	double distbuspark;
	int busparking;
	vector<int> busParkingPath;
	int busrc;
	if(!gas){
		busrc = getClosestBusStop(src);
		busparking = getClosestParkingSpotDest(busrc);
		busParkingPath = getPath(src,busparking, distbuspark);
	}
	else {
		busrc = getClosestBusStop(gasid);
		busparking = getClosestParkingSpotDest(busrc);
		busParkingPath = getPath(gasid,busparking, distbuspark);
	}
	if(drivingPath.size() == 1)
		return 1;

	cout << "Driving Cost " << distmp* mileage << endl;
	cout << "Bus Cost " << busTicket+mileage*distbuspark << endl << endl;

	if(!gas){
		if((distmp*mileage) > (busTicket+mileage*distbuspark)){
			finalPath = busParkingPath;
			double distmp2;
			walkPath = getPath(busparking,busrc,distmp2);
			int busdest = getClosestBusStopDest(it->second);
			busPath = getBusPath(busrc,busdest,distbus);
			double distmp3;
			walkPath2 = getPath(busdest,it->second,distmp3);

			setPath(finalPath, srcVertexColor, parkingSpotVertexColor, drivingPathEdgeColor);
			setPathWalk(walkPath, parkingSpotVertexColor, destVertexColor, walkPathEdgeColor);
			setPathWalk(busPath, destVertexColor, busStopVertexColor, busPathEdgeColor);
			setPathWalk(walkPath2, busStopVertexColor, "cyan", walkPathEdgeColor);
			walkPath.insert(walkPath.end(),finalPath.begin(), finalPath.end()-1);
			walkPath.insert(walkPath.end(),busPath.begin(), busPath.end()-1);
			walkPath.insert(walkPath.end(), walkPath2.begin(), walkPath2.end());
			lastPath = walkPath;
		}
		else { //carro
			walkPath = getPath(parkingid, it->second,distmp);
			if(walkPath.size() == 0)
				return 1;
			setPathWalk(walkPath, parkingSpotVertexColor, destVertexColor, walkPathEdgeColor);
			setPath(drivingPath, srcVertexColor, parkingSpotVertexColor, drivingPathEdgeColor);
			lastPath = drivingPath;
			lastPath.pop_back();
			lastPath.insert(lastPath.end(), walkPath.begin(), walkPath.end());
		}
	}
	else {
		if((distmp*mileage) > (busTicket+mileage*distbuspark)){
			finalPath = busParkingPath;
			double distmp2;
			walkPath = getPath(busparking,busrc,distmp2);
			int busdest = getClosestBusStopDest(it->second);
			busPath = getBusPath(busrc,busdest,distbus);
			double distmp3;
			walkPath2 = getPath(busdest,it->second,distmp3);
			setPath(gasPath, srcVertexColor, gasStationVertexColor, gasPathEdgeColor);
			setPath(finalPath, gasStationVertexColor, parkingSpotVertexColor, drivingPathEdgeColor);
			setPathWalk(walkPath,parkingSpotVertexColor, destVertexColor, "walkPathEdgeColor");
			setPathWalk(busPath, destVertexColor, busStopVertexColor, busPathEdgeColor);
			setPathWalk(walkPath2, busStopVertexColor, "cyan", walkPathEdgeColor);
			lastPath = gasPath;
			lastPath.pop_back();
			walkPath.insert(walkPath.end(),finalPath.begin(), finalPath.end()-1);
			walkPath.insert(walkPath.end(),busPath.begin(), busPath.end()-1);
			walkPath.insert(walkPath.end(), walkPath2.begin(), walkPath2.end());
			lastPath.insert(lastPath.end(), walkPath.begin(), walkPath.end());
		}
		else { //carro
			walkPath = getPath(parkingid, it->second,distmp);
			setPath(gasPath, srcVertexColor, gasStationVertexColor, gasPathEdgeColor);
			setPathWalk(walkPath, parkingSpotVertexColor,  destVertexColor,  walkPathEdgeColor);
			setPath(drivingPath, gasStationVertexColor, parkingSpotVertexColor, drivingPathEdgeColor);
			lastPath = gasPath;
			lastPath.pop_back();
			lastPath.insert(lastPath.end(), drivingPath.begin(), drivingPath.end()-1);
			lastPath.insert(lastPath.end(), walkPath.begin(), walkPath.end());
		}
	}
	gv->rearrange();
	return 0;
}

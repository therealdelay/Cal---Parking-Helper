#include "Cidade.h"
using namespace std;

const string Cidade::defaultVertexColor = "yellow";
const string Cidade::defaultEdgeColor = "blue";
const string Cidade::parkingSpotVertexColor = "green";
const string Cidade::pathEdgeColor = "red";
const string Cidade::srcVertexColor = "red";
const string Cidade::destVertexColor = "blue";
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

float Cidade::Haversine(double idNoOrigem, double idNoDestino){
	typename vector<Vertex<long long int>*>::iterator it= total.getVertexSet().begin();
	typename vector<Vertex<long long int>*>::iterator ite= total.getVertexSet().end();
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

	long long int idNo = 0;
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
		if(type == "Paragem"){
			Vertex<int> *v = new Vertex<int>(idNo ,X, Y, Xrad, Yrad, type);
			busStops.push_back(v);
		}
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
	long long int idAresta=0;
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
		total.addEdge(counter,idNoOrigem, idNoDestino,Haversine(idNoOrigem, idNoDestino));
		gv->addEdge(counter, idNoOrigem, idNoDestino, EdgeType::DIRECTED);
		/*
		bidirectional.addEdge(counter2,idNoOrigem, idNoDestino,Haversine(idNoOrigem, idNoDestino));
		counter2++;
		bidirectional.addEdge(counter2, idNoDestino, idNoOrigem, Haversine(idNoOrigem, idNoDestino));
		counter2++;
		 */
		//MOSTRAR NOMES
		//if(streetName != "noname")
		//gv->setEdgeLabel(counter, streetName);
		//MOSTRAR NOMES
		counter++;
		total.addEdge(counter,idNoDestino, idNoOrigem,Haversine(idNoOrigem, idNoDestino));
		gv->addEdge(counter, idNoDestino, idNoOrigem, EdgeType::DIRECTED);
		counter++;
	}

	edgeNumber = counter;
	inFile.close();
	gv->rearrange();
}

void Cidade::setPath(vector<long long int> path, string vertexColor, string edgeColor){
	for(unsigned int i = 0; i < path.size(); i++){
		gv->setVertexColor(path[i], vertexColor);
		Vertex<long long int> *base = total.getVertex(path[i]);
		for(unsigned int j = 0; j < base->getAdj().size(); j++){
			if(base->getAdj()[j].getDest()->getId() == path[i+1]){
				gv->setEdgeColor(base->getAdj()[j].getId(), edgeColor);
			}
		}
	}
}

void Cidade::setPathWalk(vector<long long int> path, string srcColor, string destColor, string edgeColor){
	if(path.size() == 0)
		return;
	int counter = edgeNumber+1;
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

void Cidade::setPath(vector<long long int> path, string srcColor, string destColor, string edgeColor){
	if(path.size() == 0)
		return;
	gv->setVertexColor(path[0], srcColor);
	for(unsigned int i = 0; i < path.size()-1; i++){
		Vertex<long long int> *base = total.getVertex(path[i]);
		for(unsigned int j = 0; j < base->getAdj().size(); j++){
			if(base->getAdj()[j].getDest()->getId() == path[i+1]){
				gv->setEdgeColor(base->getAdj()[j].getId(), edgeColor);
			}
		}
	}
	gv->setVertexColor(path[path.size()-1], destColor);
}

vector<long long int> Cidade::getPath(const long long int &src, const long long int &dest, double &dist){
	total.dijkstraShortestPath(src);
	Vertex<long long int> *t = total.getVertex(dest);
	dist = t->getDist();
	return total.getPath(src, dest);
}

/*
vector<long long int> Cidade::getPathWalk(const long long int &src, const long long int &dest, double &dist){
	bidirectional.dijkstraShortestPath(src);
	Vertex<long long int> *t = bidirectional.getVertex(dest);
	dist = t->getDist();
	return bidirectional.getPath(src, dest);
}
 */

vector<long long int> Cidade::getBusPath(const long long int &src, const long long int &dest, double &dist){
	vector<long long int> path;
	int sIndex = -1, dIndex = -1;
	for(unsigned int i = 0; i < busStops.size(); i++){
		//cout << busStops[i]->getId() << endl;
		if(src == busStops[i]->getId())
			sIndex = i;
		if(dest == busStops[i]->getId())
			dIndex = i;
		if(sIndex !=-1 && dIndex !=-1){
			break;
		}
	}
	/*cout << sIndex << endl;
	cout << dIndex << endl;*/

	if(sIndex == -1 || dIndex == -1)
		return path;

	cout << "chegou aqui\n";
	int dir;
	if(sIndex < dIndex)
		dir = 1;
	else
		dir = -1;

	double distTotal = 0;
	double distmp ;
	int i = sIndex+dir;
	while(true){
		//cout << i << endl;
		if(dir == 1){
			if(i > dIndex)
				break;
		}
		else{
			if(i < dIndex)
				break;
		}
		//cout << "src  " << busStops[i-dir]->getId() << "   dest "<< busStops[i]->getId() << endl;
		vector<long long int> tmp = getPath(busStops[i-dir]->getId(), busStops[i]->getId(), distmp);
		if(distmp == 0)
			break;
		if(path.size() != 0)
			path.pop_back();
		path.insert(path.end(), tmp.begin(), tmp.end());
		distTotal += distmp;
		i += dir;
	}

	/*
	for(unsigned int i = 0; i < path.size(); i++)
		cout << path[i] << endl;
	 */
	setPath(path, destVertexColor, parkingSpotVertexColor, "green");
	return path;
}

void Cidade::clearGraphViewer(){
	if(lastPath.size() == 0)
		return;
	setPath(lastPath, defaultVertexColor, defaultEdgeColor);
	for(unsigned int i = lastedgeNumber; i > edgeNumber; i--)
		gv->removeEdge(i);
}

void Cidade::setMaxDist(double max){
	maxDist = max;
}

void Cidade::setMileage(double mil) {
	mileage = mil;
}

long long int Cidade::getClosestParkingSpotDest(const long long int &dest){
	Vertex<long long int> *d = total.getVertex(dest);
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

long long int Cidade::getClosestParkingSpot(const long long int &src){
	Vertex<long long int> *t = total.getVertex(src);
	if(t == NULL)
		return -1;
	total.dijkstraShortestPath(src);

	double minDist = INT_INFINITY;
	unsigned int min = 0;
	for(unsigned int i = 0; i < parkingSpots.size(); i++){
		Vertex<long long int> *v = total.getVertex(parkingSpots[i].getId());
		if(v == NULL)
			return -1;
		if(v->getDist() < minDist){
			min = i;
			minDist = v->getDist();
		}
	}
	if(minDist == INT_INFINITY)
		return -1;
	else
		return parkingSpots[min].getId();
}

long long int Cidade::getClosestBusStop(const long long int &src) {
	Vertex<long long int> *t = total.getVertex(src);
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

long long int Cidade::getClosestBusStopDest(const long long int &dest){
	//VERIFICAR SE A SRC EXISTE
	Vertex<long long int> *d = total.getVertex(dest);
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

long long int Cidade::getClosestGasStationSpot(const long long int &dest){
	//VERIFICAR SE A SRC EXISTE
	Vertex<long long int> *d = total.getVertex(dest);
	if(d == NULL)
		return -1;
	double minDist = INT_INFINITY;
	unsigned int min = 0;
	for(unsigned int i = 0; i < gasSpots.size(); i++){
		total.bellmanFordShortestPath(gasSpots[i]);
		//total.dijkstraShortestPath(gasSpots[i]);
		//cout << d->getDist() << endl;
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

int Cidade::getClosestRoute(const long long int &src, string dest, bool gas){
	map<string, long long int>::const_iterator it = spots.begin();
	for(; it != spots.end(); it++){
		if(compareNames(dest.c_str(), it->first.c_str()) == 0)
			break;
	}
	if(it == spots.end())
		return 1;
	//cout << it->second << endl;
	double distmp;
	double distTotal = 0;

	//LIMPAR VIEWER
	clearGraphViewer();

	vector<long long int> gasPath;
	long long int gasid;

	long long int parkingid = getClosestParkingSpotDest(it->second);
	if(parkingid == -1)
		return 1;

	if(gas){
		gasid = getClosestGasStationSpot(parkingid);
		//cout << gasid << endl;
		if(gasid == -1)
			return 1;
		gasPath= getPath(src, gasid, distmp);
		distTotal += distmp;
	}

	vector<long long int> walkPath = getPath(parkingid, it->second,distmp);
	distTotal += distmp;

	vector<long long int> drivingPath;
	if(gas)
		drivingPath = getPath(gasid, parkingid, distmp);
	else
		drivingPath = getPath(src, parkingid, distmp);
	if(drivingPath.size() == 1)
		return 1;
	distTotal += distmp;

	if(distTotal > maxDist){
		cout << "Path " << distTotal << " is too long\n";
		gv->rearrange();
		return 1;
	}

	if(gas){
		setPath(gasPath, srcVertexColor, "black", "black");
		setPath(drivingPath, "black", destVertexColor, pathEdgeColor);
		setPath(walkPath, destVertexColor, parkingSpotVertexColor, "green");
		lastPath = gasPath;
		lastPath.pop_back();
		lastPath.insert(lastPath.end(), drivingPath.begin(), drivingPath.end()-1);
		lastPath.insert(lastPath.end(), walkPath.begin(), walkPath.end());
	}
	else{
		setPathWalk(walkPath, destVertexColor, parkingSpotVertexColor, "green");
		setPath(walkPath, destVertexColor, parkingSpotVertexColor, "green");
		setPath(drivingPath, srcVertexColor, destVertexColor, pathEdgeColor);
		lastPath = drivingPath;
		lastPath.pop_back();
		lastPath.insert(lastPath.end(), walkPath.begin(), walkPath.end());
	}

	gv->rearrange();
	return 0;
}

long long int Cidade::getCheapestParkingSpot(const long long int &dest) {
	Vertex<long long int> *t = total.getVertex(dest);
	if(t == NULL)
		return -1;
	total.dijkstraShortestPath(dest);
	double minPrice = INT_INFINITY;
	long long int id = 0;
	for(unsigned int i = 0; i < parkingSpots.size(); i++){
		Vertex<long long int> *v = total.getVertex(parkingSpots[i].getId());
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

int Cidade::getCheapestRoute(const long long int &src, string dest, bool gas){
	if(spots.find(dest) == spots.end())
		return 1;
	double distmp;
	double distgas = 0;
	double distbus;
	double distTotal = 0;
	clearGraphViewer();
	vector<long long int> gasPath;
	vector<long long int> drivingPath;
	vector<long long int> drivingPathBus;
	vector<long long int> busPath;
	vector<long long int> finalPath;
	vector<long long int> walkPath;
	vector<long long int> walkPath2;
	long long int gasid;

	long long int parkingid = getCheapestParkingSpot(spots.at(dest));
	if(parkingid == -1){
		cout << "Path " << distTotal << " is too long\n";
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
	/*if(walkPath.size() == 1)
		return 1;*/

	double distbuspark;
	long long int busparking;
	vector<long long int> busParkingPath;
	long long int busrc;
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
	//if(true) ///distmp*mileage) > (busTicket+mileage*distbuspark)){ //autocarro
	cout << distmp* mileage << endl;
	cout << busTicket+mileage*distbuspark << endl;
	if(!gas){
		if((distmp*mileage) > (busTicket+mileage*distbuspark)){
			finalPath = busParkingPath;
			double distmp2;
			walkPath = getPath(busparking,busrc,distmp2);
			long long int busdest = getClosestBusStopDest(spots.at(dest));
			busPath = getBusPath(busrc,busdest,distbus);
			double distmp3;
			walkPath2 = getPath(busdest,spots.at(dest),distmp3);

			setPath(finalPath, srcVertexColor, destVertexColor, pathEdgeColor);
			setPath(walkPath, destVertexColor, "green", "green");
			setPath(busPath, "green", "orange", "orange");
			setPath(walkPath2, "orange", "cyan", "green");
			walkPath.insert(walkPath.end(),finalPath.begin(), finalPath.end()-1);
			walkPath.insert(walkPath.end(),busPath.begin(), busPath.end()-1);
			walkPath.insert(walkPath.end(), walkPath2.begin(), walkPath2.end());
			lastPath = walkPath;
		}
		else { //carro
			walkPath = getPath(parkingid, spots.at(dest),distmp);
			setPath(walkPath, destVertexColor, parkingSpotVertexColor, "green");
			setPath(drivingPath, srcVertexColor, destVertexColor, pathEdgeColor);
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
			long long int busdest = getClosestBusStopDest(spots.at(dest));
			busPath = getBusPath(busrc,busdest,distbus);
			double distmp3;
			walkPath2 = getPath(busdest,spots.at(dest),distmp3);
			setPath(gasPath, srcVertexColor, "black", "black");
			setPath(finalPath, srcVertexColor, destVertexColor, pathEdgeColor);
			setPath(walkPath, destVertexColor, "green", "green");
			setPath(busPath, "green", "orange", "orange");
			setPath(walkPath2, "orange", "cyan", "green");
			walkPath.insert(walkPath.end(),finalPath.begin(), finalPath.end()-1);
			walkPath.insert(walkPath.end(),busPath.begin(), busPath.end()-1);
			walkPath.insert(walkPath.end(), walkPath2.begin(), walkPath2.end());
			lastPath = walkPath;
		}
		else { //carro
			walkPath = getPath(parkingid, spots.at(dest),distmp);
			setPath(gasPath, srcVertexColor, "black", "black");
			setPath(walkPath, destVertexColor, parkingSpotVertexColor, "green");
			setPath(drivingPath, srcVertexColor, destVertexColor, pathEdgeColor);
			lastPath = drivingPath;
			lastPath.pop_back();
			lastPath.insert(lastPath.end(), walkPath.begin(), walkPath.end());
		}
	}
	gv->rearrange();
	return 0;
}


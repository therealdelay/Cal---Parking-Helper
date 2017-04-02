#include "Cidade.h"
using namespace std;

const string Cidade::defaultVertexColor = "yellow";
const string Cidade::defaultEdgeColor = "blue";
const string Cidade::parkingSpotVertexColor = "green";
const string Cidade::pathEdgeColor = "red";
const string Cidade::srcVertexColor = "red";
const string Cidade::destVertexColor = "blue";

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

	ifstream inFile, inFile2;
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
			if(type[0] != ' ' && type != "Estacionamento" && type != "Bombas de Gasolina")
				spots[type] = idNo;
		}
		//ISTO POE VERTICES COM E
		//if(type == "")
		//type = "E";
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

	/*
    map<string, long long int>::iterator itera = spots.begin();
    for(; itera != spots.end(); itera++)
    	cout << itera->first << "   " << itera->second << endl;
	 */

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
	inFile2.open("ruas.txt");
	if (!inFile2) {
		cerr << "Unable to open file datafile.txt";
		exit(1);   // call system to stop
	}
	long long int prevIdAresta = -1;
	bool direction = false;
	string streetName = "noname";
	long long int idAresta=0;
	long long int idNoOrigem=0;
	long long int idNoDestino=0;
	double weight;
	int counter = 0 ;
	while(std::getline(inFile, line))
	{
		std::stringstream linestream(line);
		std::string data;
		linestream >> idAresta;
		//TRATAR DE ATUALIZAR AS RUAS
		if(idAresta != prevIdAresta){
			string line2;
			std::getline(inFile2, line2);
			stringstream linestream2(line2);
			linestream2 >> prevIdAresta;
			std::getline(linestream2, data, ';');
			std::getline(linestream2, streetName, ';');
			string dir;
			std::getline(linestream2, dir);
			if(dir == "False")
				direction = false;
			else
				direction = true;
		}
		std::getline(linestream, data, ';');  // read up-to the first ; (discard ;).
		linestream >> idNoOrigem;
		std::getline(linestream, data, ';');  // read up-to the first ; (discard ;).
		linestream >> idNoDestino;
		//cout << Haversine(idNoOrigem, idNoDestino) << endl;
		total.addEdge(counter,idNoOrigem, idNoDestino,Haversine(idNoOrigem, idNoDestino));
		gv->addEdge(counter, idNoOrigem, idNoDestino, EdgeType::DIRECTED);
		//MOSTRAR NOMES
		//if(streetName != "noname")
		//gv->setEdgeLabel(counter, streetName);
		//MOSTRAR NOMES
		//gv->setEdgeLabel(counter, streetName);
		if(direction){
			counter++;
			total.addEdge(counter,idNoDestino, idNoOrigem,Haversine(idNoOrigem, idNoDestino));
			gv->addEdge(counter, idNoDestino, idNoOrigem, EdgeType::DIRECTED);
			//MOSTRAR NOMES
			//if(streetName != "noname")
			//gv->setEdgeLabel(counter, streetName);
		}
		counter++;
	}
	inFile.close();
	inFile2.close();

	//EXPERIMENTAR DIFERENTES PONTOS

	//this->getClosestParkingSpot(1663052161);
	//this->getClosestParkingSpot(1662970220);
	//this->getClosestParkingSpot(1663029440);
	//this->getClosestParkingSpot(1663052212);
	//this->getClosestParkingSpot(1663052071);
	//this->getClosestParkingSpot(1489430296);
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

vector<long long int> Cidade::getPath(const long long int &src, const long long int &dest){
	total.bellmanFordShortestPath(src);
	//total.dijkstraShortestPath(src);
	Vertex<long long int> *t = total.getVertex(dest);
	//cout << "Min dist 2 " << t->getDist() << endl;
	return total.getPath(src, dest);
}

void Cidade::clearGraphViewer(){
	if(lastPath.size() == 0)
		return;
	setPath(lastPath, defaultVertexColor, defaultEdgeColor);
}

long long int Cidade::getClosestParkingSpot(const long long int &src){
	//VERIFICAR SE A SRC EXISTE
	Vertex<long long int> *t = total.getVertex(src);
	if(t == NULL)
		return -1;
	total.bellmanFordShortestPath(src);

	double minDist = INT_INFINITY;
	unsigned int min = 0;
	for(unsigned int i = 0; i < parkingSpots.size(); i++){
		Vertex<long long int> *v = total.getVertex(parkingSpots[i].getId());
		if(v == NULL)
			return -1;
		//cout << v->getDist() << endl;
		if(v->getDist() < minDist){
			min = i;
			minDist = v->getDist();
		}
	}

	//cout << "Min Dist1 "<< minDist << endl;

	if(minDist == INT_INFINITY)
		return -1;
	else
		return parkingSpots[min].getId();
}

long long int Cidade::getClosestGasStationSpot(const long long int &src){
	//VERIFICAR SE A SRC EXISTE
	Vertex<long long int> *t = total.getVertex(src);
	if(t == NULL)
		return -1;

	total.bellmanFordShortestPath(src);

	double minDist = INT_INFINITY;
	unsigned int min = 0;
	for(unsigned int i = 0; i < gasSpots.size(); i++){
		Vertex<long long int> *v = total.getVertex(gasSpots[i]);
		if(v == NULL)
			return -1;
		cout << v->getDist() << endl;
		if(v->getDist() < minDist){
			min = i;
			minDist = v->getDist();
		}
	}

	cout << "Min Dist1 "<< minDist << endl;

	if(minDist == INT_INFINITY)
		return -1;
	else
		return gasSpots[min];
}

int Cidade::getClosestRoute(const long long int &src, string dest, bool gas){
	if(spots.find(dest) == spots.end())
		return 1;
	cout << "Chegou aqui\n";
	//LIMPAR VIEWER
	clearGraphViewer();
	vector<long long int> gasPath;
	long long int gasid;
	if(gas){
		gasid = getClosestGasStationSpot(src);
		cout << gasid << endl;
		if(gasid == -1)
			return 1;
		gasPath= getPath(src, gasid);
	}
	long long int parkingid =  getClosestParkingSpot(spots.at(dest));
	//vector<long long int> p = getPath(walkPath[walkPath.size()-1], spots.at(dest));
	//vector<long long int> pr = getPath(spots.at(dest), walkPath[walkPath.size()-1]);
	vector<long long int> walkPath = getPath(parkingid, spots.at(dest));

	if(walkPath.size() == 1)
		return 1;

	vector<long long int> drivingPath;
	if(gas)
		drivingPath = getPath(gasid, parkingid);
	else
		drivingPath = getPath(src, parkingid);
	if(drivingPath.size() == 1)
		return 1;

	if(gas){
		setPath(gasPath, srcVertexColor, "black", "black");
		setPath(drivingPath, "black", destVertexColor, pathEdgeColor);
		setPath(walkPath, destVertexColor, parkingSpotVertexColor, "green");
		lastPath = gasPath;
		lastPath.pop_back();
		lastPath.insert(lastPath.end(), drivingPath.begin(), drivingPath.end()-1);
		lastPath.insert(lastPath.end(), walkPath.begin(), walkPath.end()-1);
	}
	else{
		setPath(walkPath, destVertexColor, parkingSpotVertexColor, "green");
		setPath(drivingPath, srcVertexColor, destVertexColor, pathEdgeColor);
		lastPath = drivingPath;
		lastPath.pop_back();
		lastPath.insert(lastPath.end(), walkPath.begin(), walkPath.end());
	}

	gv->rearrange();
	return 0;
}

vector<long long int> Cidade::getCheapestPath(const long long int &src, const long long int &dest){
	total.unweightedShortestPath(src);
}

int Cidade::getCheapestRoute(const long long int &src, string dest, bool gas){
	if(spots.find(dest) == spots.end())
		return 1;
	cout << "Chegou aqui\n";
	//LIMPAR VIEWER
	clearGraphViewer();
	vector<long long int> gasPath;
	long long int gasid;
	if(gas){
		gasid = getClosestGasStationSpot(src);
		cout << gasid << endl;
		if(gasid == -1)
			return 1;
		gasPath = getPath(src, gasid);
	}
	long long int parkingid =  getClosestParkingSpot(spots.at(dest));
	//vector<long long int> p = getPath(walkPath[walkPath.size()-1], spots.at(dest));
	//vector<long long int> pr = getPath(spots.at(dest), walkPath[walkPath.size()-1]);
	vector<long long int> walkPath = getPath(parkingid, spots.at(dest));

	if(walkPath.size() == 1)
		return 1;

	vector<long long int> drivingPath;
	if(gas)
		drivingPath = getPath(gasid, parkingid);
	else
		drivingPath = getPath(src, parkingid);
	if(drivingPath.size() == 1)
		return 1;

	if(gas){
		setPath(gasPath, srcVertexColor, "black", "black");
		setPath(drivingPath, "black", destVertexColor, pathEdgeColor);
		setPath(walkPath, destVertexColor, parkingSpotVertexColor, "green");
		lastPath = gasPath;
		lastPath.pop_back();
		lastPath.insert(lastPath.end(), drivingPath.begin(), drivingPath.end()-1);
		lastPath.insert(lastPath.end(), walkPath.begin(), walkPath.end()-1);
	}
	else{
		setPath(walkPath, destVertexColor, parkingSpotVertexColor, "green");
		setPath(drivingPath, srcVertexColor, destVertexColor, pathEdgeColor);
		lastPath = drivingPath;
		lastPath.pop_back();
		lastPath.insert(lastPath.end(), walkPath.begin(), walkPath.end());
	}

	gv->rearrange();
	return 0;
}


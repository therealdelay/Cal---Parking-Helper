#include "Cidade.h"


int Cidade::resizeLat(double lat) {
	return (600 - (round(600 / (latmax - latmin) * (lat - latmin))));
}

int Cidade::resizeLong(double lon) {
	return (round(600 / (lonmax - lonmin) * (lon - lonmin)));
}

float Haversine(double lat1, double lat2, double lon1, double lon2){
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
	GraphViewer *gv = new GraphViewer(600, 600, false);
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

	Graph<long long int> g;

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
		gv->addNode(idNo, resizeLat(X), resizeLong(-Y));
		g.addVertex(idNo, X, Y, Xrad, Yrad, type);

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
		g.addEdge(idNoOrigem, idNoDestino,0);
	}
	inFile.close();
	gv->rearrange();
}

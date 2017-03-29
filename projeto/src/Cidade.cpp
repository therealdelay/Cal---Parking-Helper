#include "Cidade.h"
using namespace std;


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
    for (; it!=ite; it++){
        if ((*it)->getId() == idNoOrigem){
            lat1 = (*it)->getXrad();
            lon1= (*it)->getYrad();
            break;
        }
    }
    it = total.getVertexSet().begin();
    for (; it!=ite; it++){
        if ((*it)->getId() == idNoOrigem){
            lat2 = (*it)->getXrad();
            lon2= (*it)->getYrad();
            break;
        }
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

    long long int idNo = NULL;
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
        total.addVertex(idNo, X, Y, Xrad, Yrad, type);
        idNo = NULL;
    }

    for(int i = 0; i < total.getVertexSet().size(); i++){
        gv->addNode(total.getVertexSet()[i]->getId(), resizeLat(total.getVertexSet()[i]->getX()), resizeLong(-total.getVertexSet()[i]->getY()));
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
        total.addEdge(idNoOrigem, idNoDestino,Haversine(idNoOrigem, idNoDestino));
        counter++;
    }
    inFile.close();
    counter = 0;
    for(int i = 0; i < total.getVertexSet().size(); i++){
        for(int j = 0; j < total.getVertexSet()[i]->getAdj().size(); j++){
            gv->addEdge(counter, total.getVertexSet()[i]->getId(), total.getVertexSet()[i]->getAdj()[j].getDest()->getId(), EdgeType::UNDIRECTED);
            counter++;
        }
    }

    gv->rearrange();
}

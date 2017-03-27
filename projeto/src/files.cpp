#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Graph.h"

using namespace std;

void readVertexes(){
	ifstream i("vertices.txt");
	char lixo;
	int vid, vlatdeg, vlatrad, vlondeg, vlonrad;
	while(!i.eof()){
		i >> vid >> lixo >> vlatdeg >> lixo >> vlondeg >> lixo >> vlatrad >> lixo >> vlonrad;
		cout << vid << endl << vlatdeg << endl << vlondeg << endl << vlatrad << endl << vlonrad << endl;
	}
}

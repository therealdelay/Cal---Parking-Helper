#include <iostream>
#include <string>
#include <stdlib.h>
#include <cmath>
#include <fstream>

using namespace std;

int getOption(int a, int b){
	cout << "> ";
	int c;
	cin >> c;
	if(c >= a && c <= b)
		return c;
	else
		return 0;
}

void invalidOption(){
	cout << "Invalid option\n";
	cin.clear();
	cin.ignore(1000, '\n');
}

bool refillTank(){
	string s;
	cout << "Do you want to refill your tank in the way?(Y/N)\n> ";
	cin >> s;
	for(int i = 0; i < s.size(); i++){
		s[i] = toupper(s[i]);
	}

	if(s == "Y" || s == "YES")
		return true;
	else if(s == "N" || s == "NO")
		return false;
	else{
		invalidOption();
		refillTank();
	}
}

void closestSpot(){
	if(refillTank()){
		//chose the shortest path with a fuel pump in the way
	}
	else{
		//chose the shortest path
	}
}

void cheapestSpot(){
	if(refillTank()){
		//chose the cheapest path with a fuel pump in the way
	}
	else{
		//chose the cheapest path
	}
}

int mainMenu(){
	cout << "Park your car!\n\n\n";
	cout << "1 - Closest parking spot\n"
			<< "2 - Cheapest parking spot\n\n"
			<< "3 - Exit\n";

	switch(getOption(1,3)){
	case 0:
		invalidOption();
		mainMenu();
		break;
	case 1:
		closestSpot();
		break;
	case 2:
		cheapestSpot();
		break;
	case 3:
		exit(0);
	}

	return 0;
}

void readVertices(){
	ifstream i("vertices.txt");
	char lixo;
	long long int vid;
	float vlatdeg, vlatrad, vlondeg, vlonrad;
	while(!i.eof()){
		i >> vid >> lixo >> vlatdeg >> lixo >> vlondeg >> lixo >> vlatrad >> lixo >> vlonrad;
		cout << vid << endl << vlatdeg << endl << vlondeg << endl << vlatrad << endl << vlonrad << endl;
	}
}

void readRuas(){
	ifstream i("ruas.txt");
	char lixo;
	long long int rid;
	string rname, rbool;
	bool rtwoways;
	while(!i.eof()){
		i >> rid >> lixo; getline(i,rname,';');
		getline(i, rbool);
		rtwoways = (rbool == "False") ? false : true;
		cout << rid << endl << rname << endl << rtwoways << endl;
	}
}

void readArestas(){
	ifstream i("arestas.txt");
	char lixo;
	long long int rid, vsource, vdest;

}


int main(){
	//readVertices();
	readRuas();
	mainMenu();
	return 0;
}

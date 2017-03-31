#include "menu.h"

int getOption(int a, int b){
	std::cout << "> ";
	int c;
	cin >> c;
	if(cin.fail())
		return -1;
	if(c == 0)
		return 0;
	if(c < a || c > b)
		return -1;
	return c;
}

void invalidOption(){
	std::cout << "Invalid option\n";
	std::cin.clear();
	std::cin.ignore(1000, '\n');
}

bool refillTank(){
	std::string s;
	std::cout << "Do you want to refill your tank in the way?(Y/N)\n> ";
	std::cin >> s;
	for(unsigned int i = 0; i < s.size(); i++){
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

int mainMenu(Cidade &c){
	int option;
	while(true){
		cout << "Park your car!\n\n\n";
		cout << "1 - Closest parking spot\n"
			 << "2 - Cheapest parking spot\n\n"
			 << "0 - Exit\n";
		option = getOption(1,2);
		if(option == 0)
			break;
		if(option)
		switch(option){
		case -1:
			invalidOption();
			break;
		case 1:
			long long int id;
			cout << "Input ID\n> ";
			cin >> id;
			if(cin.fail()){
				invalidOption();
				break;
			}
			c.getClosestParkingSpot(id);
			break;
		case 2:
			//cheapestSpot();
			c.clearGraphViewer();
			break;
		}

	}
	return 0;
}

void readVertices(){
	std::ifstream i("vertices.txt");
	char lixo;
	long long int vid;
	float vlatdeg, vlatrad, vlondeg, vlonrad;
	while(!i.eof()){
		i >> vid >> lixo >> vlatdeg >> lixo >> vlondeg >> lixo >> vlatrad >> lixo >> vlonrad;
		std::cout << vid << std::endl << vlatdeg << std::endl << vlondeg << std::endl << vlatrad << std::endl << vlonrad << std::endl;
	}
}

void readRuas(){
	std::ifstream i("ruas.txt");
	char lixo;
	long long int rid;
	std::string rname, rbool;
	bool rtwoways;
	while(!i.eof()){
		i >> rid >> lixo; getline(i,rname,';');
		std::getline(i, rbool);
		rtwoways = (rbool == "False") ? false : true;
		std::cout << rid << std::endl << rname << std::endl << rtwoways << std::endl;
	}
}

/*
void readArestas(){
	ifstream i("arestas.txt");
	char lixo;
	long long int rid, vsource, vdest;

}
*/

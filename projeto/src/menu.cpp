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


void clearBuffer(){
	std::cin.clear();
	std::cin.ignore(1000, '\n');
}
void invalidOption(){
	std::cout << "Invalid option\n";
	clearBuffer();
}

void closestSpot(Cidade &c){
	clearBuffer();
	long long int id;
	string dest;
	cout << "Destination\n> ";
	std::getline(cin,dest);
	dest = removeSpaces(dest);
	cout << "Input ID\n> ";
	cin >> id;
	if(cin.fail()){
		invalidOption();
		return;
	}
	bool gas = false;
	cin.ignore(255, '\n');
	cout << "Gas(Y/N)\n> ";
	char choice;
	cin >> choice;
	if(cin.fail()){
		invalidOption();
		return;
	}

	if(choice == 'Y' || choice == 'y')
		gas = true;
	if(choice == 'Y' || choice == 'y')
		gas = true;
	else if (choice == 'N' || choice == 'n')
		gas = false;
	else{
		invalidOption();
		return;
	}

	cin.ignore(255, '\n');
	double mDist;
	cout << "MaxDist\n> ";
	cin >> mDist;
	if(cin.fail()){
		invalidOption();
		return;
	}
	if(mDist <= 0)
		return;
	c.setMaxDist(mDist);
	c.getClosestRoute(id, dest, gas);
	return;
}

void cheapestSpot(Cidade &c){
	clearBuffer();
	long long int id;
	string dest;
	cout << "Destination\n> ";
	std::getline(cin,dest);
	dest = removeSpaces(dest);
	cout << "Input ID\n> ";
	cin >> id;
	if(cin.fail()){
		invalidOption();
		return;
	}
	bool gas = false;
	cin.ignore(255, '\n');
	cout << "Gas(Y/N)\n> ";
	char choice;
	cin >> choice;
	if(cin.fail()){
		invalidOption();
		return;
	}
	if(choice == 'Y' || choice == 'y')
		gas = true;
	else if (choice == 'N' || choice == 'n')
		gas = false;

	else{
		invalidOption();
		return;
	}
	cin.ignore(255, '\n');
	double mDist;
	cout << "MaxDist\n> ";
	cin >> mDist;
	if(cin.fail()){
		invalidOption();
		return;
	}
	if(mDist <= 0)
		return;
	c.setMaxDist(mDist);
	double mileage;
	cout << "L/km\n> ";
	cin >> mileage;
	if(cin.fail()){
		invalidOption();
		return;
	}
	if(mileage <= 0)
		return;
	c.setMileage(mileage);
	c.getCheapestRoute(id, dest, gas);
}

void mainMenu(Cidade &c){
	int option;
	while(true){
		cout << "Park your car!\n\n\n";
		cout << "1 - Closest parking spot\n"
				<< "2 - Cheapest parking spot\n\n"
				<< "0 - Exit\n";

		option = getOption(1,2);

		if(option == 0)
			break;

		switch(option){
		case -1:
			invalidOption();
			break;
		case 1:
		{
			closestSpot(c);
			break;
		}
		case 2:
			cheapestSpot(c);
			break;
		}
	}
	return;
}

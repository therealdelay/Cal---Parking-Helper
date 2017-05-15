#include "menu.h"

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

void exactStreetSearch(Cidade &c){
	clearBuffer();
	string streetName;
	cout << "Street Name\n> ";
	getline(cin, streetName);
	if(streetName != "")
		c.exactStreetSearch(streetName);
	else
		invalidOption();
}

void aproxStreetSearch(Cidade &c){
	clearBuffer();
	string streetName;
	cout << "Street Name\n> ";
	getline(cin, streetName);
	if(streetName != "")
		c.aproxStreetSearch(streetName);
	else
		invalidOption();
}

void mainMenu(Cidade &c){
	int option;
	while(true){
		cout << "Park your car!\n\n\n";
		cout << "1 - Closest parking spot\n"
				<< "2 - Cheapest parking spot\n"
				<< "3 - Exact Street Search\n"
				<< "4 - Approximate Street Search\n\n"
				<< "0 - Exit\n";

		option = getOption(1,4);

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

		case 3:
			exactStreetSearch(c);
			getchar();
			break;
		case 4:
			aproxStreetSearch(c);
			getchar();
			break;
		}
	}
	return;
}

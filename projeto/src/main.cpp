//#include "Cidade.h"
#include "menu.h"

//using namespace std;

int main() {
	Cidade c;
	c.readFromFile();
	mainMenu(c);
	//getchar();
	return 0;
}

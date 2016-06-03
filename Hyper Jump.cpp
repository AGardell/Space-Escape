//The purpose of this program is to simulate a hyperspace jump where the "Hero" cannot return the way he came. One caveat
//is that the program is based on the fuel amount being 5. Calculations are hardcoded based on that.


#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <math.h>

class Map {
	std::vector<char> x;
	std::vector<char> y;
	std::vector<int> cost;
public:
	Map(){};
	char getX(int i) {return x.at(i);}
	char getY(int i) {return y.at(i);}
	int getCost(int i) {return cost.at(i);}
	void pushData(char a, char b, int i) {x.push_back(a), y.push_back(b), cost.push_back(i); }
	int getSize() {return x.size();}
	void markX(int i) {x.at(i) = '*';}
}map;

struct Hero {
	char currentPlanet;
	char destination;
	int fuel;
	Hero():currentPlanet('A'), fuel(5){}; //initalize fuel to 5
	void Jump();
	void Reverse();
	int checkFuel() {return fuel;}
	int neededForReturn() {return (fuel - (fuel / 2));}
}hero;

struct escapeRoute { //structure escape route as a linked list.
	char planet;
	escapeRoute* ptr;
	escapeRoute():planet('A'), ptr(NULL){};
	void add(const char& ch);
};

struct returnRoute:escapeRoute {
	returnRoute* ptr;
	void addReturn(const char& ch);
};


escapeRoute* begin;
returnRoute* beginReturn;

void Hero::Jump() {
	int fuelCost = 0;

	for (int i = 0; i < map.getSize(); ++i) { // begin loop through our "Data" coordinates.
		if (map.getX(i) == currentPlanet) {
			if (destination == '\0') { // if Hero has no destination just assign the first one.
				destination = map.getY(i);
				fuelCost = map.getCost(i);
			}
			else if (destination < map.getY(i) && fuel - map.getCost(i) >= 3) { //determine how much further the new destination is and fuel cost.
				destination = map.getY(i);
				fuelCost = map.getCost(i);
				map.markX(i); // mark the planets you jumped from so the program knows not to return.
			}
		}
	}

	std::cout << currentPlanet << '-' << destination;
	begin->add(destination);
	currentPlanet = destination;
	fuel = fuel - fuelCost;
	std::cout << "\nFuel left: " << fuel << '\n';

	if (hero.fuel <= 3) { // if our Hero has more than half his fuel (in this case 3) continue jumping.
		beginReturn->planet = currentPlanet; //otherwise begin return route.
		Reverse();
		return;
	} else
		Jump();

}

void Hero::Reverse() { //reverse his trip to return home. Planets already touched are now marked as "*".
	int fuelCost = 0;

	for (int i = 0; i < map.getSize(); ++i) {
		if (map.getY(i) == hero.currentPlanet) {
			if (destination > map.getX(i) && map.getX(i) != '*') { 
				destination = map.getX(i);
				fuelCost = map.getCost(i);
			}
		}
	}

	if (currentPlanet == 'A') {
		std::cout << "HOME\n";
		return;
	}

	std::cout << currentPlanet << '-' << destination;
	beginReturn->addReturn(destination);
	currentPlanet = destination;
	fuel = fuel - fuelCost;
	std::cout << "\nFuel left: " << fuel << '\n';
	Reverse();
}

void escapeRoute::add(const char& ch) { // method for adding nodes to your escape route.
	escapeRoute* next = begin;

	while (next->ptr != 0)
		next = next->ptr;

	next->ptr = new escapeRoute;
	next = next->ptr;
	next->planet = ch;
	next->ptr = 0;

}

void returnRoute::addReturn(const char& ch) {
	returnRoute* next = beginReturn;

	while (next->ptr != 0)
		next = next->ptr;

	next->ptr = new returnRoute;
	next = next->ptr;
	next->planet = ch;
	next->ptr = 0;

}

int main() {
	std::ifstream data;

	data.open("Coordinates.txt");

	std::stringstream ss;
	std::string temp;
	char a;
	char b;
	int coord;

	if (!data.is_open())
		std::cout << "\nERROR OPENING FILE!\n";

	while(std::getline(data,temp)) {
		ss.clear();
		//ss.str(std::string());
		ss.str(temp);
		while (ss >> a >> b >> coord) {
			map.pushData(a, b, coord); // Pushes back the a coordinate, b coordinate, and fuel cost 
		}                              //into their respective vectors.
	}

	begin = new escapeRoute; // create a linked list representing the "escape route".
	begin->planet = 'A'; // begin planet will always be planet A.
	begin->ptr = 0;

	beginReturn = new returnRoute; // create a linked list representing the "return route" for our Hero.
	beginReturn->planet = '\0'; // set to null.
	beginReturn->ptr = 0;

	hero.Jump(); //begin jump
	escapeRoute* read = begin->ptr;
	std::cout << "Route away: " << begin->planet;
	while (read != 0) {
		std::cout << '-' << read->planet;
		read = read->ptr;
	}
	std::cout << std::endl;
	returnRoute* read2 = beginReturn->ptr;
	std::cout << "Route back: " << beginReturn->planet;
	while (read2 != 0) {
		std::cout << '-' << read2->planet;
		read2 = read2->ptr;
	}
	std::cout << std::endl;
	delete read;
	delete begin;
}


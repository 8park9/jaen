#ifndef HORSE_H
#define HORSE_H
#include <vector>
#include <string>

using namespace std;

class Horse {
private:
	int index;
	string name;
	int stamina;
	int decSize;
	int speed;
	int weight;
	int loyalty;
	vector<string> career;
public:
	Horse(int idx, string name, int stamina, int speed, int weight, int loyalty, vector<string> car);
	void displayInfo() const;
	int getIdx() const;
	string getName() const;
	int getStamina() const;
	int getDecSize() const;
	int getSpeed() const;
	int getWeight() const;
	int getLoyalty() const;
	vector<string> getCareer() const;
};


#endif // HORSE_H
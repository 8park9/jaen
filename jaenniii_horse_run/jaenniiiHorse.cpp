#include "jaenniiiHorse.h"
#include <iostream>
#include <random>

using namespace std;

Horse::Horse(int idx, string name, int stamina, int speed, int weight, int loyalty, vector<string> car) {
    index = idx;
    name = name;
    stamina = stamina;
    speed = speed;
    weight = weight;
    loyalty = loyalty;
    career = car;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(10, 20);
    int decSize = distrib(gen);
}

void Horse::displayInfo() const {
    cout << "Index: " << index << "\n"
        << "Name: " << name << "\n"
        << "Stamina: " << stamina << "\n"
        << "Decrease Size: " << decSize << "\n"
        << "Speed: " << speed << "\n"
        << "Weight: " << weight << "\n"
        << "Loyalty: " << loyalty << "\n"
        << "Career: ";
    for (const auto& c : career) {
        cout << c << " ";
    }
    cout << endl;
}

int Horse::getIdx() const {
    return index;
}

string Horse::getName() const {
    return name;
}

int Horse::getStamina() const {
    return stamina;
}

int Horse::getDecSize() const {
    return decSize;
}
int Horse::getSpeed() const {
    return speed;
}
int Horse::getWeight() const {
    return weight;
}

int Horse::getLoyalty() const {
    return loyalty;
}

vector<string> Horse::getCareer() const {
    return career;
}
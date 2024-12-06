#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include <tuple>
#include <iostream>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

using namespace std;

class Player {

private:

	string name;

	int money;

	vector<int> each_round_money;

	int max_rounds;

	vector<tuple<int, int, int>> betting_info;



public:

	Player(int max_rounds_);

	void setPlayerName(string name);

	void setBettingInfo(int round_num, int my_horse, int betting_money);

	void decreaseMoney(int amount);

	void increaseMoney(int amount);

	void savePlayerMoney(int round_num);

	bool isBankrupt();

	string getPlayerName();

	int getMyHorse(int round_num);

	int getBettingMoney(int round_num);

	int getMoney();

	int getEachRoundMoney(int round_num);

};


#endif // Player_H

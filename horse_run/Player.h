#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>

using namespace std;

class Player {

private:

	string name;

	vector<int> my_horse;

	int money;

	vector<int> each_round_money;

	int max_rounds;

public:

	Player(int max_rounds_);

	void setPlayerName();

	void setMyHorse(int round_num);

	void decreaseMoney(int amount);

	void increaseMoney(int amount);

	void savePlayerMoney(int round_num);

	void showPlayerMoney(int round_num);

	bool isBankrupt();

	string getPlayerName();

	int getMyHorse(int round_num);

	int getMoney();

	int getEachRoundMoney(int round_num);

};


#endif // Player_H

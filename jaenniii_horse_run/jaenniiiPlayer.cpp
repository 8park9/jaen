#include "jaenniiiPlayer.h"
#include <iostream>
#include <limits>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

using namespace std;

Player::Player(int max_rounds_) : max_rounds(max_rounds_), money(500) {
    each_round_money.resize(max_rounds);
}

void Player::setPlayerName(string name_) {
    name = name_;
}


void Player::setBettingInfo(int round_num, int my_horse, int betting_money) {
    betting_info.emplace_back(round_num, my_horse, betting_money);
    decreaseMoney(betting_money);
}

void Player::decreaseMoney(int amount) {
    if (amount >= 0) {
        money -= amount;
    }
}

void Player::increaseMoney(int amount) {
    if (amount >= 0) {
        money += amount;
    }
}

void Player::savePlayerMoney(int round_num) {
    if (round_num <= max_rounds && round_num > 0) {
        each_round_money[round_num - 1] = money;
    }
}

bool Player::isBankrupt() {
    return money <= 0;
}

string Player::getPlayerName() {
    return name;
}

int Player::getMyHorse(int round_num) {
    if (round_num <= max_rounds && round_num > 0) {
        return get<1>(betting_info[round_num - 1]);
    }
    else {
        return -1;
    }
}

int Player::getBettingMoney(int round_num) {
    if (round_num <= max_rounds && round_num > 0) {
        return get<2>(betting_info[round_num - 1]);
    }
    else {
        return -1;
    }
}

int Player::getMoney() {
    return money;
}

int Player::getEachRoundMoney(int round_num) {
    if (round_num <= max_rounds && round_num > 0) {
        return each_round_money[round_num - 1];
    }
    else {
        return -1;
    }
}

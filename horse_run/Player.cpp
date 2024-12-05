#include "Player.h"
#include <iostream>
#include <limits>

using namespace std;

Player::Player(int max_rounds_) : max_rounds(max_rounds_), money(500) {
    my_horse.resize(max_rounds);
    each_round_money.resize(max_rounds);
}

void Player::setPlayerName() {
    cout << "이름: ";
    getline(cin, name);
}

void Player::setMyHorse(int round_num) {
    if (round_num <= max_rounds && round_num > 0) {
        cout << "선택할 말의 번호: ";
        cin >> my_horse[round_num - 1];
    }
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

void Player::showPlayerMoney(int round_num) {
    if (round_num <= max_rounds && round_num > 0) {
        cout << round_num << "라운드 ";
        cout << name << "님의 현재 잔액: " << each_round_money[round_num - 1] << endl;
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
        return my_horse[round_num - 1];
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

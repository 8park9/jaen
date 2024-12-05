#include "Queue.h"
#include "Horse.h"
#include "Player.h"

#include <iostream>
#include <random>
#include <vector>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <cstdlib>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

using namespace std;

mutex mtx;     //데이터 충돌 보호 뮤텍스
const int TRACK_LENGTH = 100; // 트랙 길이
bool raceFinished = false;   // 레이스 종료 여부

// HORSE RUN 출력
void printHorseRun() {
    cout << R"(
     _   _  _____ ______  _____  _____   _____  _   _  _   _ 
    | | | ||  _  || ___ \/  ___||  ___| | ___ \| | | || \ | |
    | |_| || | | || |_/ /\ `--. | |__   | |_/ /| | | ||  \| |
    |  _  || | | ||    /  `--. \|  __|  |    / | | | || . ` |
    | | | |\ \_/ /| |\ \ /\__/ /| |___  | |\ \ | |_| || |\  |
    \_| |_/ \___/ \_| \_|\____/ \____/  \_| \_| \___/ \_| \_/

   )" << endl;
}

// 말의 상태를 출력
void printTrack(const vector<int>& positions, int numHorses, const vector<int>& baseSpeeds, const vector<int>& fatigue) {
    lock_guard<mutex> lock(mtx);
    cout << "\033[2J\033[1;1H"; // 화면 지우기 (ANSI Escape) 콘솔 화면을 지우고 커서를 맨 위로 이동 해주는 법
    for (int i = 0; i < numHorses; ++i) {
        cout << "Horse " << i + 1 << ": ";
        for (int j = 0; j < TRACK_LENGTH; ++j) {
            if (j == positions[i]) {
                //cout << ((positions[i] % 2 == 0) ? ">==o" : "||==>"); // 뛰는 애니메이션
                cout << u8"🐎";
            }
            else {
                cout << "-";
            }
        }
        cout << " ||" << endl;
        if (i == 0) {                                        //몇번 말 설정해줘서 상태 띄워주기
            cout << "    1번 기수 탈락" << endl;
        }
        cout << endl;
        cout << "  100까지 달려보자: " << positions[i] << endl;
        cout << "  기본속도: " << baseSpeeds[i] << "  Fatigue: " << fatigue[i] << endl;
        cout << endl;

    }
}

// 말이 달리는 쓰레드 함수
void runHorse(int id, vector<int>& positions, vector<int>& rankings, int& finishedCount, vector<int>& baseSpeeds, vector<int>& fatigue) {
    //vector<int> baseSpeeds = { 300, 100, 150, 120, 250 }; // 말마다 기본 속도 설정
    int fatigueFactor = 5; // 거리 비례 피로도

    while (positions[id] < TRACK_LENGTH - 1) {
        int fatigueLevel = positions[id] * fatigueFactor; //피로도 계산 트랙을 가면 갈수록 피로도가 커지게 계산
        int currentSpeed = max(baseSpeeds[id] + rand() % 300 - fatigueLevel, 50); // 현재 속도    rand()%300 이면 0 ~ 300
        fatigue[id] = fatigueLevel;
        this_thread::sleep_for(chrono::milliseconds(currentSpeed)); //속도에 따라 대기
        positions[id]++;
        if (positions[id] >= TRACK_LENGTH - 1) {
            lock_guard<mutex> lock(mtx);
            rankings.push_back(id + 1);
            finishedCount++;
            break;
        }
    }
}


vector<int> startRound(Horse& horse1, Horse& horse2, Horse& horse3, Horse& horse4, Horse& horse5) {

    // Horse class 참조해서 멤버변수 바탕으로 해당경기에서 예상 말의속도 반환, 0~100 사이
    // 매 라운드 마다 실행 해주면됨
    //

    random_device rd;   // 하드웨어 엔트로피 기반 난수
    mt19937 gen(rd());  // 난수 생성 엔진 (Mersenne Twister)
    vector<int> avg_speed;

    Horse horses[] = { horse1, horse2, horse3, horse4, horse5 };

    for (int i = 0; i < 5; i++) {
        Horse horse = horses[i];
        double stamina = horse.getStamina();
        double speed = horse.getSpeed();
        double weight = horse.getWeight();
        //horse.getLoyalty()
        double p = 0.05;// 높은 말 0.2 낯은말은 0.05

        normal_distribution<double> stamina_result(stamina, 3.0);
        normal_distribution<double> speed_result(speed, 3.0);
        normal_distribution<double> weight_result(weight, 3.0);
        bernoulli_distribution bernoulli(p);

        double num1 = stamina_result(gen); // 정규 분포 난수 생성
        double num2 = speed_result(gen);
        double num3 = weight_result(gen);
        bool fall = bernoulli(gen);

        if (fall == true) {
            cout << "낙마" << endl;
            avg_speed.push_back(0); // 낙마하면 0
        }
        else {
            int result = (num1 + num2 + num3) / 3;
            avg_speed.push_back(result);
        }

    }

    return avg_speed;

}



int main() {

    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8); // UTF-8 설정
    #endif
    
    setlocale(LC_ALL, "en_US.UTF-8");

    const int numHorses = 5; // 말의 수
    vector<int> positions(numHorses); // 각 말의 위치
    vector<int> rankings;               // 순위 기록
    vector<int> fatigue(numHorses);  // 각 말의 피로도 초기화
    vector<int> baseSpeeds = { 300, 100, 150, 120, 250 };
    int finishedCount = 0;

    // 쓰레드 생성
    vector<thread> horses;
    for (int i = 0; i < numHorses; ++i) {
        horses.emplace_back(runHorse, i, ref(positions), ref(rankings), ref(finishedCount), ref(baseSpeeds), ref(fatigue));
    }

    // 화면 갱신 쓰레드
    thread displayThread([&]() {
        while (!raceFinished) {
            printTrack(positions, numHorses, baseSpeeds, fatigue);
            this_thread::sleep_for(chrono::milliseconds(100)); // 화면 갱신 주기
            if (finishedCount == numHorses) {
                raceFinished = true; // 모든 말이 도착하면 종료
            }
        }
        });

    // 쓰레드 종료 대기
    for (auto& horse : horses) {
        horse.join();
    }
    displayThread.join();

    // 결과 출력
    cout << "\nRace finished!\nFinal Rankings:" << std::endl;
    for (size_t i = 0; i < rankings.size(); ++i) {
        wcout << i + 1 << L"위: Horse " << rankings[i] << std::endl;
    }

    return 0;
}


#include "jaenniiiQueue.h"
#include "jaenniiiHorse.h"
#include "jaenniiiPlayer.h"

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
#include <windows.h> // 유니코드 출력을 위한 헤더

using namespace std;

mutex mtx;     //데이터 충돌 보호 뮤텍스
const int TRACK_LENGTH = 100; // 트랙 길이
bool raceFinished = false;   // 레이스 종료 여부
vector<Player> players; // 플레이어 객체 배열

// 한국어 설정
void setupConsole() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8); // 콘솔에 한국어 및 유니코드 출력 설정
    SetConsoleCP(CP_UTF8); // 콘솔에 한국어 및 유니코드 입력 설정
#endif
    setlocale(LC_ALL, "en_US.UTF-8");
}

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

// 게임 시작 화면
void gameStart() {
    int num;
    printHorseRun();
    wcout << L"게임설명\n" << endl; // 막 이 게임은 17세기 영국에서부터 시작돼서 블라블라
    wcout << L"시작하시겠습니까? ";
    wcout << L"시작하면 더이상 돌이킬 수 없습니다.\n" << endl;
    wcout << L"1. YES  2. 네   ";
    wcout << L"(번호를 입력해주세요.)\n";
    cin >> num;
    if (num == 1 || num == 2) {
        cout << "\033[2J\033[1;1H";
    }
    else {
        wcout << L"게임을 종료합니다." << endl;
        exit(0);
    }
}

// 플레이어 이름 입력
void inputPlayerName(int index) {
    string player_name;
    wcout << index + 1 << L"번 플레이어의 이름을 설정해주세요. ";
    cin >> player_name;
    players[index].setPlayerName(player_name);
}

// 각 라운드별 플레이어 배팅 정보 입력
void inputBettingInfo(int index, int round_num_) {
    int round_num = round_num_;
    int my_horse;
    int betting_money;
    wcout << L"배팅할 말의 번호를 입력해주세요. ";
    cin >> my_horse;
    if (my_horse < 0 || my_horse > 5) {
        wcout << L"말을 다시 선택해주세요.\n배팅할 말의 번호를 입력해주세요. ";
        cin >> my_horse;
    }
    wcout << L"얼마나 배팅하시겠습니까? ";
    cin >> betting_money;
    if (betting_money < 0 || betting_money > players[index].getMoney()) {
        wcout << L"배팅금액을 다시 설정해주세요.\n얼마나 배팅하시겠습니까? ";
        cin >> betting_money;
    }
    players[index].setBettingInfo(round_num, my_horse, betting_money);
}

// 배당률 계산 및 배당금 나눠주기
void betting(int round_num, int first_horse) {
    int total_betting_amount = 0; // 총 배팅액
    int first_horse_betting_amount = 0; // 1등마의 총 배팅액
    double betting_rate = 0.0;
    vector<bool> betting_player_flag(players.size(), false);// 각 플레이어가 1등마에 배팅했는지 확인
    for (int i = 0; i < players.size(); i++) {
        total_betting_amount += players[i].getBettingMoney(round_num);
        if (players[i].getMyHorse(round_num) == first_horse) {
            first_horse_betting_amount += players[i].getBettingMoney(round_num);
            betting_player_flag[i] = true;
        }
    }
    if (first_horse_betting_amount != 0) {
        betting_rate = static_cast<double> (total_betting_amount) / first_horse_betting_amount; // 나머지는 더러우니까 빼자.. ㅎㅎ
        cout << round_num;
        wcout << L"라운드의 배당률은 " << betting_rate << L"입니다.\n" << endl;
        for (int i = 0; i < players.size(); i++) {
            int betting_amount = 0; // 1등마에 배팅한 각 플레이어가 받는 배팅액
            if (betting_player_flag[i] == true) {
                betting_amount = players[i].getBettingMoney(round_num) * betting_rate;
                players[i].increaseMoney(betting_amount);
            }
            players[i].savePlayerMoney(round_num);
        }
    }
    else {
        return;
    }
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
            wcout << L"    1번 기수 탈락" << endl;
        }
        cout << endl;
        wcout << L"  100까지 달려보자: " << positions[i] << endl;
        wcout << L"  기본속도: " << baseSpeeds[i] << "  Fatigue: " << fatigue[i] << endl;
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

// Horse class 참조해서 멤버변수 바탕으로 해당경기에서 예상 말의속도 반환, 0~100 사이
// 매 라운드 마다 실행 해주면됨
vector<int> startRound(Horse& horse1, Horse& horse2, Horse& horse3, Horse& horse4, Horse& horse5) {

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
            wcout << L"낙마" << endl;
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

    const int numHorses = 5; // 말의 수
    int max_rounds = 5; // 최대 라운드 수
    vector<int> positions(numHorses); // 각 말의 위치
    vector<int> rankings;               // 순위 기록
    vector<int> fatigue(numHorses);  // 각 말의 피로도 초기화
    vector<int> baseSpeeds = { 300, 100, 150, 120, 250 };
    int finishedCount = 0;
    int players_number;

    setupConsole();

    gameStart();

    wcout << L"참가 인원을 입력해주세요. ";
    cin >> players_number;
    cout << endl;

    // 플레이어 객체를 동적으로 생성
    players.reserve(players_number);

    for (int i = 0; i < players_number; i++) {
        players.emplace_back(max_rounds); // Player 객체 추가
    }

    for (int i = 0; i < players.size(); i++) {
        inputPlayerName(i); // 플레이어 이름 설정
    }
    wcout << L"모든 플레이어의 이름 설정이 완료되었습니다.";
    cout << "\033[2J\033[1;1H";

    for (int round = 1; round <= max_rounds; round++) {
        cout << "---------------\n";
        wcout << round << L"라운드 시작!\n";
        cout << "---------------\n" << endl;

        for (int i = 0; i < players.size(); i++) {

            cout << players[i].getPlayerName();
            wcout << L"님의 배팅을 시작하겠습니다.\n";

            inputBettingInfo(i, round); //베팅 정보 세팅

            wcout << L"현재 ";
            cout << players[i].getPlayerName();
            wcout << L"님의 잔액은 ";
            cout << players[i].getMoney();
            wcout << L"입니다.\n";
        }

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
        cout << endl;
        cout << "Race finished!\nFinal Rankings:" << endl;
        for (size_t i = 0; i < rankings.size(); ++i) {
            wcout << i + 1 << L"위: Horse " << rankings[i] << endl;
        }
        cout << endl;

        betting(round, rankings[0]);
        for (int i = 0; i < players.size(); i++) {
            cout << players[i].getPlayerName();
            wcout << L"님의 잔액은 ";
            cout << players[i].getEachRoundMoney(round);
            wcout << L"입니다.\n";
        }
        cout << endl;

        string check; // 문자열로 입력 받기
        wcout << L"다음 라운드를 진행하시겠습니까? (엔터를 누르면 진행, 다른 문자를 입력하면 종료됩니다.)\n";
        cin.ignore(); // 이전 입력 버퍼 비우기
        getline(cin, check); // 입력 받기

        if (check.empty()) { // 엔터만 입력된 경우
            cout << "\033[2J\033[1;1H"; // 콘솔 창 지우기
        }
        else { // 다른 입력이 있는 경우
            wcout << L"게임을 종료합니다." << endl;
            exit(0); // 루프 종료
        }
    }
    return 0;
}


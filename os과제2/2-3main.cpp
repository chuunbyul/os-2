// 2-3만 구현
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
#include <thread>
#include <mutex>
#include <cstdlib>
#include <Windows.h>

using namespace std;

#define Y   400

void Echo(const vector<string>& tokens, int repeatCount, int period, int duration) {
    for (int i = 0; i < repeatCount; ++i) {
        for (size_t j = 1; j < tokens.size(); ++j) {
            cout << tokens[j] << " ";
        }
        cout << endl;

        this_thread::sleep_for(chrono::seconds(period));

        if (period * repeatCount > duration) {
            exit(0);
        }
    }
}

void Dummy(const vector<string>& tokens, int repeatCount, int period, int duration) {
    for (int i = 0; i < repeatCount; ++i) {
        // 아무 작업도 하지 않음

        this_thread::sleep_for(chrono::seconds(period));

        if (period * repeatCount > duration) {
            exit(0);
        }
    }
}

void GCD(const vector<string>& tokens, int repeatCount, int period, int duration) {
    for (int i = 0; i < repeatCount; ++i) {
        if (tokens.size() != 3) {
            cerr << "Invalid arguments for gcd command" << endl;
            return;
        }

        int x = stoi(tokens[1]);
        int y = stoi(tokens[2]);

        // 유클리드 호제법을 사용한 최대공약수 계산
        while (y != 0) {
            int temp = x % y;
            x = y;
            y = temp;
        }

        cout << x << endl;

        this_thread::sleep_for(chrono::seconds(period));

        if (period * repeatCount > duration) {
            exit(0);
        }
    }
}

bool isPrime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

void Prime(const vector<string>& tokens, int repeatCount, int period, int duration) {
    for (int i = 0; i < repeatCount; ++i) {
        if (tokens.size() != 2) {
            cerr << "Invalid arguments for prime command" << endl;
            return;
        }

        int x = stoi(tokens[1]);
        int primeCount = 0;

        for (int i = 2; i <= x; ++i) {
            if (isPrime(i)) {
                ++primeCount;
            }
        }

        cout << primeCount << endl;

        this_thread::sleep_for(chrono::seconds(period));

        if (period * repeatCount > duration) {
            exit(0);
        }
    }
}

void Sum(const vector<string>& tokens, int repeatCount, int period, int duration) {
    for (int i = 0; i < repeatCount; ++i) {
        int x = stoi(tokens[1]);
        long long sum = 0;
        const int MOD = 1000000;

        for (int i = 1; i <= x; ++i) {
            sum = (sum + i) % MOD;
        }

        cout << sum << endl;

        this_thread::sleep_for(chrono::seconds(period));

        if (period * repeatCount > duration) {
            exit(0);
        }
    }
}

void processCommands(const string& filename) {
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        istringstream iss(line);
        vector<string> tokens;
        string token;

        while (iss >> token) {
            tokens.push_back(token);
        }

        cout << "prompt> ";
        for (const auto& token : tokens) {
            cout << token << " ";
        }
        cout << endl;

        int repeatCount = 1;
        int period = 0; // 기본적으로 반복 주기를 0으로 설정하여 한 번만 실행하도록 함
        int duration = 300; // 기본적으로 300초(5분)로 설정

        for (size_t i = 0; i < tokens.size(); ++i) {
            if (tokens[i] == "-n") {
                if (i + 1 < tokens.size()) {
                    repeatCount = stoi(tokens[i + 1]);
                }
            }
            else if (tokens[i] == "-p") {
                if (i + 1 < tokens.size()) {
                    period = stoi(tokens[i + 1]);
                }
            }
            else if (tokens[i] == "-d") {
                if (i + 1 < tokens.size()) {
                    duration = stoi(tokens[i + 1]);
                }
            }
        }

        vector<string> commandT;
        for (size_t i = 0; i < tokens.size(); ++i) {
            if (tokens[i] != "-n" && tokens[i] != "-p" && tokens[i] != "-d") {
                commandT.push_back(tokens[i]);
            }
            else {
                break;
            }
        }

        if (!commandT.empty()) {
            const string& command = commandT[0];
            if (command == "echo") {
                Echo(commandT, repeatCount, period, duration);
            }
            else if (command == "dummy") {
                Dummy(commandT, repeatCount, period, duration);
            }
            else if (command == "gcd") {
                GCD(commandT, repeatCount, period, duration);
            }
            else if (command == "prime") {
                Prime(commandT, repeatCount, period, duration);
            }
            else if (command == "sum") {
                Sum(commandT, repeatCount, period, duration);
            }
            else {
                cerr << "Unknown command: " << command << endl;
            }
        }
    }
}

int main() {
    string filename = "commands.txt";

    processCommands(filename);

    return 0;
}

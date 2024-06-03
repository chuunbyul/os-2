#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <sstream>
#include <chrono>
#include <algorithm>
#include <mutex>
#include <fstream>

#define Y 500
using namespace std;

void Echo(const vector<string>& t) {
    for (size_t j = 1; j < t.size(); ++j) {
        cout << t[j] << " ";
    }
    cout << endl;
}

void GCD(const vector<string>& t) {
    if (t.size() != 3) {
        cerr << "Invalid arguments for gcd command" << endl;
        return;
    }
    int x = stoi(t[1]);
    int y = stoi(t[2]);
    while (y != 0) {
        int temp = x % y;
        x = y;
        y = temp;
    }
    cout << x << endl;
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

void Prime(const vector<string>& t) {
    if (t.size() != 2) {
        cerr << "Invalid arguments for prime command" << endl;
        return;
    }
    int x = stoi(t[1]);
    int pc = 0;
    for (int i = 2; i <= x; ++i) {
        if (isPrime(i)) {
            ++pc;
        }
    }
    cout << pc << endl;
}

void Sum(const vector<string>& t) {
    if (t.size() != 2) {
        cerr << "Invalid arguments for sum command" << endl;
        return;
    }
    int x = stoi(t[1]);
    long long s = 0;
    const int MOD = 1000000;
    for (int i = 1; i <= x; ++i) {
        s = (s + i) % MOD;
    }
    cout << s << endl;
}

void executeCommand(const vector<string>& t, bool bg) {
    const string& c = t[0];
    if (c == "echo") {
        Echo(t);
    }
    else if (c == "gcd") {
        GCD(t);
    }
    else if (c == "prime") {
        Prime(t);
    }
    else if (c == "sum") {
        Sum(t);
    }
    else {
        cerr << "Unknown command: " << c << endl;
        return;
    }
}

void procFile(const string& filename, bool bg) {
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        vector<string> t;
        string tk;
        while (iss >> tk) {
            t.push_back(tk);
        }
        if (t.empty()) continue;

        if ((bg && t[0][0] == '&') || (!bg && t[0][0] != '&')) {
            cout << "prompt> ";
            for (const auto& tk : t) {
                cout << tk << " ";
            }
            cout << endl;

            if (t[0][0] == '&') {
                t[0].erase(0, 1);  // Remove '&' character for BG commands
            }

            executeCommand(t, bg);
        }
    }
}

int main() {
    string fn = "commands.txt";

    thread FG([&]() {
        procFile(fn, false);
        });

    thread BG([&]() {
        procFile(fn, true);
        });

    FG.join();
    BG.join();

    return 0;
}

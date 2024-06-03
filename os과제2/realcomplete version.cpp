#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <iomanip>
#include <Windows.h>
#include <chrono>
#include <stdlib.h>

#define Y   500

using namespace std;
using namespace std::chrono;
mutex mtx;

class CommandQueue {
private:
    queue<vector<string>> commands;
    mutex mtx;
    condition_variable cv;
    bool done = false;

public:
    void push(const vector<string>& command) {
        {
            lock_guard<mutex> lock(mtx);
            commands.push(command);
        }
        cv.notify_one();
    }

    vector<string> pop() {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [&] { return !commands.empty() || done; });
        if (commands.empty() && done) return {};
        vector<string> command = commands.front();
        commands.pop();
        return command;
    }

    void setDone() {
        {
            lock_guard<mutex> lock(mtx);
            done = true;
        }
        cv.notify_all();
    }

    bool isDone() {
        lock_guard<mutex> lock(mtx);
        return done && commands.empty();
    }
};

void Echo(const vector<string>& t) {
    for (size_t j = 1; j < t.size(); ++j) {
        cout << t[j] << " ";
    }
    cout << endl;
}

void Dummy() {

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

void excSC(const vector<string>& t) {
    const string& c = t[0];
    if (c == "echo") {
        Echo(t);
    }
    else if (c == "dummy") {
        Dummy();
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
    }
}

void executeCommand(const vector<string>& t) {
    int r = 1;
    int p = 0;
    int d = 300;
    int y = 20;

    for (size_t i = 0; i < t.size(); ++i) {
        if (t[i] == "-n") {
            if (i + 1 < t.size()) {
                r = stoi(t[i + 1]);
            }
        }
        else if (t[i] == "-p") {
            if (i + 1 < t.size()) {
                p = stoi(t[i + 1]);
            }
        }
        else if (t[i] == "-d") {
            if (i + 1 < t.size()) {
                d = stoi(t[i + 1]);
            }
        }
    }

    vector<string> CT;
    for (size_t i = 0; i < t.size(); ++i) {
        if (t[i] != "-n" && t[i] != "-p" && t[i] != "-d") {
            CT.push_back(t[i]);
        }
        else {
            break;
        }
    }

    int rp = r * p * y;
    for (int count = 0; count < r; ++count) {
        excSC(CT);

        if (p > 0 && d > 0) {
            Sleep(p * y);
            if (rp >= d) {
                cout << "실행시간 초과" << endl;
                exit(1);
            }
        }
        else if (p > 0) {
            Sleep(p * y);
        }
    }
}

void Monitor(int fgCount, int bgCount, int doneFg, int doneBg) {
    cout << endl;
    cout << "Running: [" << fgCount << "F] [" << bgCount << "B]" << endl;
    cout << "---------------------------" << endl;
    cout << "DQ: P => ";
    for (int i = 0; i < doneFg; i++) {
        cout << "[" << i << "F] ";
    }
    for (int i = 0; i < doneBg; i++) {
        cout << " [" << i << "B]";
    }
    cout << " (bottom / top)" << endl;
    cout << "---------------------------" << endl;
    cout << "WQ: []" << endl;
    cout << endl;
}

void procFile(const string& filename, CommandQueue& fgQueue, CommandQueue& bgQueue, int& fgCount, int& bgCount, int& doneFg, int& doneBg) {
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
        cout << "prompt> ";
        for (const auto& tk : t) {
            cout << tk << " ";
        }
        cout << endl;
        if (t[0][0] == '&') {
            t[0].erase(0, 1);  // Remove '&' character for BG commands
            bgQueue.push(t);
            bgCount++;
        }
        else {
            fgQueue.push(t);
            fgCount++;
        }
        Sleep(Y);
    }

    fgQueue.setDone();
    bgQueue.setDone();
}

void proC(CommandQueue& queue, mutex& printMutex, int& fgCount, int& bgCount, int& doneFg, int& doneBg, bool isFG) {
    while (true) {
        vector<string> command = queue.pop();
        if (command.empty()) break;
        {
            lock_guard<mutex> lock(printMutex);
            executeCommand(command);
        }
        if (isFG) {
            doneFg++;
        }
        else {
            doneBg++;
        }
        lock_guard<mutex> lock(printMutex);
        Monitor(fgCount, bgCount, doneFg, doneBg);
    }
}

int main() {
    string fn = "commands.txt";
    CommandQueue fgQueue;
    CommandQueue bgQueue;
    mutex printMutex;
    int fgCount = 0;
    int bgCount = 0;
    int doneFg = 0;
    int doneBg = 0;

    thread fgThread(proC, ref(fgQueue), ref(printMutex), ref(fgCount), ref(bgCount), ref(doneFg), ref(doneBg), true);
    thread bgThread(proC, ref(bgQueue), ref(printMutex), ref(fgCount), ref(bgCount), ref(doneFg), ref(doneBg), false);

    procFile(fn, fgQueue, bgQueue, fgCount, bgCount, doneFg, doneBg);

    fgThread.join();
    bgThread.join();

    return 0;
}

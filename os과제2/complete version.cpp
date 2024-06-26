// 2-2 only completed shape
// 2-3 Completed
// 1);, & complete
// 2) echo, dummy, gcd, prime, sum complete
// 3) -n, -d, -p complete, -m failure
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
#include <atomic>

#define Y 500
#define DEFAULT_DURATION 300
#define DEFAULT_PERIOD 1

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
};

void Echo(const vector<string>& t) {
    for (size_t j = 1; j < t.size(); ++j) {
        cout << "echo: " << t[j] << " ";
    }
    cout << endl;
}

void Dummy() {}

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
    cout << "gcd: " << x << endl;
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
    cout << "prime: " << pc << endl;
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
    cout << "sum: " << s << endl;
}

void executeSingleCommand(const vector<string>& commandTokens) {
    lock_guard<mutex> lock(mtx);
    const string& c = commandTokens[0];
    if (c == "echo") {
        Echo(commandTokens);
    }
    else if (c == "dummy") {
        Dummy();
    }
    else if (c == "gcd") {
        GCD(commandTokens);
    }
    else if (c == "prime") {
        Prime(commandTokens);
    }
    else if (c == "sum") {
        Sum(commandTokens);
    }
    else {
        cerr << "Unknown command: " << c << endl;
    }
}

int executeCommand(const vector<string>& t) {
    int r = 1;
    int p = DEFAULT_PERIOD;
    int d = DEFAULT_DURATION;

    // Parse
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

    // Extract command tokens
    vector<string> commandTokens;
    for (size_t i = 0; i < t.size(); ++i) {
        if (t[i] != "-n" && t[i] != "-p" && t[i] != "-d") {
            commandTokens.push_back(t[i]);
        }
        else {
            break;
        }
    }

    // Launch threads
    vector<thread> threads;
    for (int i = 0; i < r; ++i) {
        threads.emplace_back([commandTokens, p, d]() {
            auto start = steady_clock::now();
            while (true) {
                executeSingleCommand(commandTokens);

                if (duration_cast<seconds>(steady_clock::now() - start).count() >= d) {
                    break;
                }

                if (p > 0) {
                    this_thread::sleep_for(seconds(p));
                }
            }
            });
    }

    for (auto& th : threads) {
        th.join();
    }
    return r;
}

void Monitor(int& fgCount, int& bgCount, atomic<bool>& stop_flag) {
    while (!stop_flag.load()) {
        this_thread::sleep_for(seconds(3));
        lock_guard<mutex> lock(mtx);
        cout << endl;
        cout << "Running: [" << fgCount << "F] [" << bgCount << "B]" << endl;
        cout << "---------------------------" << endl;
        cout << "DQ: P => ";
        for (int i = 0; i < fgCount; i++) {
            cout << "[" << i << "F] ";
            if (i != 0 && i % 3 == 0) cout << endl << '\t';
        }
        cout << endl << '\t';
        for (int i = 0; i < bgCount; i++) {
            cout << "[" << i << "B] ";
            if (i != 0 && i % 3 == 0) cout << endl << '\t';
        }
        cout << " (bottom / top)" << endl;
        cout << "---------------------------" << endl;
        cout << "WQ: [";
        cout << fgCount << "F ";
        cout << bgCount << "B";
        cout << "]" << endl;
        cout << endl;
    }
}

void procFile(const string& filename, CommandQueue& fgQueue, CommandQueue& bgQueue, int& fgCount, int& bgCount) {
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

        size_t i = 0;
        while (i < t.size()) {
            vector<string> command;
            if (t[i][0] == '&') {
                command.push_back(t[i].substr(1));
                ++i;
                while (i < t.size() && t[i] != ";") {
                    command.push_back(t[i]);
                    ++i;
                }
                bgQueue.push(command);
                int r = 1;
                for (size_t j = 0; j < command.size(); ++j) {
                    if (command[j] == "-n" && j + 1 < command.size()) {
                        r = stoi(command[j + 1]);
                    }
                }
                bgCount += r;
            }
            else {
                command.push_back(t[i]);
                ++i;
                while (i < t.size() && t[i] != ";") {
                    command.push_back(t[i]);
                    ++i;
                }
                fgQueue.push(command);
                int r = 1;
                for (size_t j = 0; j < command.size(); ++j) {
                    if (command[j] == "-n" && j + 1 < command.size()) {
                        r = stoi(command[j + 1]);
                    }
                }
                fgCount += r;
            }
            if (i < t.size() && t[i] == ";") {
                ++i;
            }
        }
        Sleep(Y);
    }

    fgQueue.setDone();
    bgQueue.setDone();
}

void proC(CommandQueue& queue, mutex& printMutex, int& fgCount, int& bgCount, bool isFG) {
    while (true) {
        vector<string> command = queue.pop();
        if (command.empty()) break;
        {
            lock_guard<mutex> lock(printMutex);
            fgCount -= executeCommand(command);
        }
    }
}

void bgWorker(CommandQueue& bgQueue, mutex& printMutex, int& bgCount) {
    while (true) {
        vector<string> command = bgQueue.pop();
        if (command.empty()) break;

        thread([command, &printMutex, &bgCount]() {
            bgCount -= executeCommand(command);
            }).detach();
    }
}

int main() {
    string fn = "commands.txt";
    CommandQueue fgQueue;
    CommandQueue bgQueue;
    mutex printMutex;
    int fgCount = 0;
    int bgCount = 0;
    atomic<bool> stop_flag(false);

    thread fgThread(proC, ref(fgQueue), ref(printMutex), ref(fgCount), ref(bgCount), true);
    thread bgThread(bgWorker, ref(bgQueue), ref(printMutex), ref(bgCount));

    thread monitorThread(Monitor, ref(fgCount), ref(bgCount), ref(stop_flag));

    procFile(fn, fgQueue, bgQueue, fgCount, bgCount);

    fgThread.join();
    bgThread.join();
    stop_flag.store(true);
    monitorThread.join();

    return 0;
}
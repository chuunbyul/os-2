#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <fstream>
#include <windows.h>
#include <unordered_map>
#include <list>
#include <sstream>

using namespace std;

mutex mtx;
condition_variable cv;
queue<string> commandQueue;
bool finished = false;

struct Process {
    string name;
    bool running;
};

struct StackNode {
    list<Process> processes;
    StackNode* next;
    StackNode() : next(nullptr) {}
};

class DynamicQueue {
public:
    DynamicQueue() : top(nullptr) {}

    void push(const list<Process>& processes) {
        StackNode* newNode = new StackNode();
        newNode->processes = processes;
        newNode->next = top;
        top = newNode;
    }

    void pop() {
        if (top != nullptr) {
            StackNode* temp = top;
            top = top->next;
            delete temp;
        }
    }

    StackNode* getTop() {
        return top;
    }

    bool isEmpty() {
        return top == nullptr;
    }

    void print() {
        StackNode* current = top;
        while (current != nullptr) {
            for (const auto& p : current->processes) {
                cout << p.name << " is " << (p.running ? "running" : "stopped") << endl;
            }
            current = current->next;
        }
    }

private:
    StackNode* top;
};

DynamicQueue dynamicQueue;

void FG_Process();
void BG_Process();
void handleCommand(const string& command);

void FG_Process() {
    ifstream file("commands.txt");
    string command;

    while (getline(file, command)) {
        this_thread::sleep_for(chrono::seconds(1)); // 1초마다 명령어 입력 가정
        {
            unique_lock<mutex> lock(mtx);
            commandQueue.push(command);
        }
        cv.notify_one(); // BG 프로세스에 알림
    }

    {
        unique_lock<mutex> lock(mtx);
        finished = true;
    }
    cv.notify_one(); // BG 프로세스에 종료 알림
}

void BG_Process() {
    while (true) {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [] { return !commandQueue.empty() || finished; });

        while (!commandQueue.empty()) {
            string command = commandQueue.front();
            commandQueue.pop();
            handleCommand(command);
        }

        if (finished) break;

        this_thread::sleep_for(chrono::seconds(1)); // 일정 시간마다 상태 출력 가정
        // 상태 출력 코드
        cout << "System monitoring: Queue size is " << commandQueue.size() << endl;
        dynamicQueue.print();
    }
}

void handleCommand(const string& command) {
    istringstream iss(command);
    string cmd, process;
    iss >> cmd;

    if (cmd == "START_PROCESS") {
        iss >> process;
        list<Process> newProcesses = { {process, true} };
        dynamicQueue.push(newProcesses);
        cout << "Started process: " << process << endl;
    }
    else if (cmd == "STOP_PROCESS") {
        iss >> process;
        dynamicQueue.push({ {process, false} });
        cout << "Stopped process: " << process << endl;
    }
    else if (cmd == "RESTART_PROCESS") {
        iss >> process;
        dynamicQueue.push({ {process, false} });
        cout << "Stopped process: " << process << endl;
        this_thread::sleep_for(chrono::seconds(1)); // Restart delay
        dynamicQueue.push({ {process, true} });
        cout << "Restarted process: " << process << endl;
    }
    else if (cmd == "STATUS") {
        cout << "Current system status:" << endl;
        dynamicQueue.print();
    }
    else if (cmd == "EXIT") {
        cout << "Exiting..." << endl;
        finished = true;
    }
}

int main() {
    thread fg(FG_Process);
    thread bg(BG_Process);

    fg.join();
    bg.join();

    cout << "== End of main() ==" << endl;
    return 0;
}

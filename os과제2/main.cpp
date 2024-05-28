#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <fstream>
#include <windows.h>

using namespace std;

mutex mtx;
condition_variable cv;
queue<string> commandQueue;
bool finished = false;

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
        cout << "모니터링: 큐 사이즈 " << commandQueue.size() << endl;
    }
}

void handleCommand(const string& command) {
    // 명령어 처리
    cout << "명렁어: " << command << endl;
}

int main() {
    thread fg(FG_Process);
    thread bg(BG_Process);

    fg.join();
    bg.join();

    cout << "== End of main() ==" << endl;
    return 0;
}

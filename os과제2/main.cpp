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
        this_thread::sleep_for(chrono::seconds(1)); // 1�ʸ��� ��ɾ� �Է� ����
        {
            unique_lock<mutex> lock(mtx);
            commandQueue.push(command);
        }
        cv.notify_one(); // BG ���μ����� �˸�
    }

    {
        unique_lock<mutex> lock(mtx);
        finished = true;
    }
    cv.notify_one(); // BG ���μ����� ���� �˸�
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

        this_thread::sleep_for(chrono::seconds(1)); // ���� �ð����� ���� ��� ����
        // ���� ��� �ڵ�
        cout << "����͸�: ť ������ " << commandQueue.size() << endl;
    }
}

void handleCommand(const string& command) {
    // ��ɾ� ó��
    cout << "����: " << command << endl;
}

int main() {
    thread fg(FG_Process);
    thread bg(BG_Process);

    fg.join();
    bg.join();

    cout << "== End of main() ==" << endl;
    return 0;
}

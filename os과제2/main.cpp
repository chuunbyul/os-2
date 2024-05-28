#include <iostream>
#include <thread>
#include <mutex>
#include <windows.h>

using namespace std;

mutex mtx;

void worker(int, int);

void worker(int id, int delay_ms) {
	for (int i = 0; i < 10; ++i) {
		Sleep(delay_ms); mtx.lock();
		cout << "thread#" << id << ": " << i << endl;
		mtx.unlock();
	}
}

int main() {
	thread t1(worker, 1, 1000);
	thread t2(worker, 2, 300);
	t1.join();
	t2.join();
	printf("== = end of main() == = \n");
}
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string>
#include <queue>
#include <thread>
#include <mutex>
#include <semaphore>
#include <atomic>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;
mutex mtx;
#define	Y	500

void shell(int, int);
void dummy();
void FG(string);

void shell(int id, int delay_ms) {
	Sleep(delay_ms);
	mtx.lock();
	cout << "prompt> ";

	//FG()
	mtx.unlock();
}

void dummy() {}

void FG(string str) {

}

int main() {
	string str;
	vector<string> vec;
	int i = 0;
	ifstream file("commands.txt");

	while (getline(file, str, ' ')) {
		vec.push_back(str);
		cout << vec[i] << endl;
		//Sleep(500);
		i++;
	}

	/*for (auto&i:vec) {
		cout << i << endl;
		if (i == "echo") {
			cout << i << endl;
		}
		Sleep(500);
	}*/
	for (i = 0; i < vec.size(); i++) {
		if (vec[i] == "echo") {
			cout << i;
			cout << vec[i + 1] << endl;
		}
		if (vec[i] == "dummy") {
			dummy();
		}
		Sleep(500);
	}


	thread t1(shell, 1, Y);
	t1.join();
	printf("--------------------- \n");

	return 0;
}
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
string line;
vector<string> vec;
#define	Y	500

void read(const string& filename);
void shell(int, int, const string&);
void dummy();
int gcd(int, int);
int prime(int);
long long sum(int);
void FG(string);

void read(const string& filename) {
	string str;
	ifstream file(filename);

	while (getline(file, line)) {
		istringstream iss(line);
		while (iss >> str) {
			vec.push_back(str);
		}
	}
	for (auto& str1 : vec) {
		cout << str1 << endl;
	}
}

void shell(int id, int delay_ms, const string& filename) {
	Sleep(delay_ms);
	mtx.lock();
	string str;
	ifstream file(filename);
	int i = 0;
	int x, y;

	for (i = 0; i < vec.size(); i++) {
		if (vec[i] == "echo") {
			getline(file, str);
			cout << "prompt> ";
			cout << str << endl;
			cout << vec[i + 1] << endl;
		}
		if (vec[i] == "dummy") {
			getline(file, str);
			cout << "prompt> ";
			cout << str << endl;
			dummy();
		}
		if (vec[i] == "gcd") {
			getline(file, str);
			cout << "prompt> ";
			cout << str << endl;
			x = stoi(vec[i + 1]);
			y = stoi(vec[i + 2]);
			cout << gcd(x, y) << endl;
		}
		if (vec[i] == "prime") {
			getline(file, str);
			cout << "prompt> ";
			cout << str << endl;
			x = stoi(vec[i + 1]);
			if (x > 100000) {
				cerr << "10���� �ʰ��Ͽ����ϴ�." << endl;
			}
			else {
				cout << prime(x) << endl;
			}
		}
		if (vec[i] == "sum") {
			getline(file, str);
			cout << "prompt> ";
			cout << str << endl;
			x = stoi(vec[i + 1]);
			if (x > 1000000) {
				cerr << "100���� �ʰ��Ͽ����ϴ�." << endl;
			}
			else {
				cout << sum(x) << endl;
			}
		}
	}
	mtx.unlock();
}

void dummy() {}

int gcd(int x, int y) {
	while (y != 0) {
		int temp = y;
		y = x % y;
		x = temp;
	}

	return x;
}

int prime(int x) {
	if (x < 2) return 0;

	vector<bool> isPrime(x + 1, true);
	isPrime[0] = isPrime[1] = false;

	for (int i = 2; i * i <= x; ++i) {
		if (isPrime[i]) {
			for (int j = i * i; j <= x; j += i) {
				isPrime[j] = false;
			}
		}
	}

	return count(isPrime.begin(), isPrime.end(), true);
}

long long sum(int x) {
	long long sum = static_cast<long long>(x) * (x + 1) / 2;
	return sum % 1000000;
}

void FG(string str) {

}

int main() {
	string filename = "commands.txt";
	read(filename);

	thread t1(shell, 1, Y, filename);
	t1.join();
	printf("--------------------- \n");

	return 0;
}
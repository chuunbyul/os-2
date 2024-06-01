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
vector<string> vec, vec_FG;
#define	Y	200

void read(const string& filename);
void FG(int, int);
void BG(int, int, const string&);
void dummy();
int gcd(int, int);
int prime(int);
long long sum(int);

void read(const string& filename) {
	string str, line;
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

void FG(int id, int delay_ms) {
	string inputLine;

	cout << "prompt> ";
	getline(cin, inputLine);

	istringstream iss(inputLine);
	string token;
	while (iss >> token) {
		size_t pos = 0;
		while ((pos = token.find_first_of(";&")) != string::npos) {
			if (pos > 0) {
				vec_FG.push_back(token.substr(0, pos));
			}
			vec_FG.push_back(string(1, token[pos]));
			token.erase(0, pos + 1);
		}
		if (!token.empty()) {
			vec_FG.push_back(token);
		}
	}
	for (auto& str : vec_FG) {
		cout << str << endl;
	}
}

void BG(int id, int delay_ms, const string& filename) {
	string str;
	ifstream file(filename);
	int i = 0;
	int x, y;
	for (i = 0; i < vec.size(); i++) {
		Sleep(delay_ms);
		mtx.lock();
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
				cerr << "10만을 초과하였습니다." << endl;
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
				cerr << "100만을 초과하였습니다." << endl;
			}
			else {
				cout << sum(x) << endl;
			}
		}
		mtx.unlock();
	}
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



int main() {
	string filename = "commands.txt";
	read(filename);

	thread t1(BG, 1, Y, filename);
	thread t2(FG, 2, Y);
	t1.join();
	t2.join();
	printf("--------------------- \n");

	return 0;
}
//#include <iostream>
//#include <stdio.h>
//#include <stdlib.h>
//#include <windows.h>
//#include <string>
//#include <thread>
//#include <mutex>
//#include <semaphore>
//#include <atomic>
//#include <fstream>
//#include <sstream>
//#include <vector>
//#include <cstdlib>
//
//using namespace std;
//mutex mtx;
//vector<string> vec_BG, vec_FG;
//#define	Y	200
//
//void read(const string& filename);
//void shell(int, int);
//void FG(int, int);
//void BG(int, int, const string&);
//void dummy();
//int gcd(int, int);
//int prime(int);
//long long sum(int);
//
//void read(const string& filename) {
//	string str, line;
//	ifstream file(filename);
//
//	while (getline(file, line)) {
//		istringstream iss(line);
//		while (iss >> str) {
//			vec_BG.push_back(str);
//		}
//	}
//	for (auto& str1 : vec_BG) {
//		cout << str1 << endl;
//	}
//}
//
//void shell(int id, int delay_ms) {
//	string inputLine;
//
//	//cout << "prompt> ";
//	getline(cin, inputLine);
//
//	istringstream iss(inputLine);
//	string token;
//	while (iss >> token) {
//		size_t pos = 0;
//		while ((pos = token.find_first_of(";&")) != string::npos) {
//			if (pos > 0) {
//				vec_FG.push_back(token.substr(0, pos));
//			}
//			vec_FG.push_back(string(1, token[pos]));
//			token.erase(0, pos + 1);
//		}
//		if (!token.empty()) {
//			vec_FG.push_back(token);
//		}
//	}
//	for (auto& str : vec_FG) {
//		cout << str << endl;
//	}
//}
//
//void FG(int id, int delay_ms) {
//	int x, y;
//	
//	for (int i = 0; i < vec_FG.size(); i++) {
//		Sleep(delay_ms);
//		mtx.lock();
//		if (vec_FG[i] == "&") { i++;
//			if (vec_FG[i] == "gcd") {
//				 ++i; vec_BG.push_back(vec_FG[i]);
//				 ++i; vec_BG.push_back(vec_FG[i]);
//				 ++i; vec_BG.push_back(vec_FG[i]);
//			}
//			else {
//				++i; vec_BG.push_back(vec_FG[i]);
//				++i; vec_BG.push_back(vec_FG[i]);
//			}
//		}
//		else {
//			if (vec_FG[i] == "echo") { 
//				++i;
//				cout << vec_FG[i] << endl;
//			}
//			if (vec_FG[i] == "dummy") {
//				dummy();
//			}
//			if (vec_FG[i] == "gcd") {
//				 ++i; x = stoi(vec_FG[i]);
//				 ++i; y = stoi(vec_FG[i]);
//				cout << gcd(x, y) << endl;
//			}
//			if (vec_FG[i] == "prime") {
//				++i; x = stoi(vec_FG[i]);
//				if (x > 100000) {
//					cerr << "10만을 초과하였습니다." << endl;
//				}
//				else {
//					cout << prime(x) << endl;
//				}
//			}
//			if (vec_FG[i] == "sum") {
//				++i; x = stoi(vec_FG[i]);
//				if (x > 1000000) {
//					cerr << "100만을 초과하였습니다." << endl;
//				}
//				else {
//					cout << sum(x) << endl;
//				}
//			}
//		}
//		mtx.unlock();
//	}
//}
//
//void BG(int id, int delay_ms, const string& filename) {
//	string str;
//	ifstream file(filename);
//	int x, y;
//	for (int i = 0; i < vec_BG.size(); i++) {
//		Sleep(delay_ms);
//		if (vec_BG[i] == "echo") {
//			getline(file, str);
//			cout << "prompt> ";
//			cout << str << endl;
//			i++; cout << vec_BG[i] << endl;
//		}
//		if (vec_BG[i] == "dummy") {
//			getline(file, str);
//			cout << "prompt> ";
//			cout << str << endl;
//			dummy();
//		}
//		if (vec_BG[i] == "gcd") {
//			getline(file, str);
//			cout << "prompt> ";
//			cout << str << endl;
//			i++; x = stoi(vec_BG[i]);
//			i++; y = stoi(vec_BG[i]);
//			cout << gcd(x, y) << endl;
//		}
//		if (vec_BG[i] == "prime") {
//			getline(file, str);
//			cout << "prompt> ";
//			cout << str << endl;
//			i++; x = stoi(vec_BG[i]);
//			if (x > 100000) {
//				cerr << "10만을 초과하였습니다." << endl;
//			}
//			else {
//				cout << prime(x) << endl;
//			}
//		}
//		if (vec_BG[i] == "sum") {
//			getline(file, str);
//			cout << "prompt> ";
//			cout << str << endl;
//			i++; x = stoi(vec_BG[i]);
//			if (x > 1000000) {
//				cerr << "100만을 초과하였습니다." << endl;
//			}
//			else {
//				cout << sum(x) << endl;
//			}
//		}
//	}
//}
//
//void dummy() {}
//
//int gcd(int x, int y) {
//	while (y != 0) {
//		int temp = y;
//		y = x % y;
//		x = temp;
//	}
//
//	return x;
//}
//
//int prime(int x) {
//	if (x < 2) return 0;
//
//	vector<bool> isPrime(x + 1, true);
//	isPrime[0] = isPrime[1] = false;
//
//	for (int i = 2; i * i <= x; ++i) {
//		if (isPrime[i]) {
//			for (int j = i * i; j <= x; j += i) {
//				isPrime[j] = false;
//			}
//		}
//	}
//
//	return count(isPrime.begin(), isPrime.end(), true);
//}
//
//long long sum(int x) {
//	long long sum = static_cast<long long>(x) * (x + 1) / 2;
//	return sum % 1000000;
//}
//
//int main() {
//	string filename = "commands.txt";
//	read(filename);
//
//	thread t1(shell, 2, Y);
//	thread t2(FG, 1, Y);
//	thread t3(BG, 1, Y, filename);
//	t1.join();
//	t2.join();
//	t3.join();
//	FG(1, Y);
//	printf("--------------------- \n");
//
//	return 0;
//}
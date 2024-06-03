//// 2-2, 2-3
//// 2-2는 정확하지 않음
//// 2-3은 gpt 없이 70% 구현
////(-m 구현 못함)
//// parse()는 vector로 구현
//#include <iostream>
//#include <fstream>
//#include <sstream>
//#include <string>
//#include <vector>
//#include <chrono>
//#include <thread>
//#include <cstdlib>
//#include <Windows.h>
//#include <list>
//#include <memory>
//#include <mutex>
//#include <atomic>
//#include <condition_variable>
//#include <map>
//#include <algorithm>
//#include <sstream>
//
//#define Y   500
//using namespace std;
//
//class J {
//public:
//    int i;
//    bool f;
//    string c;
//    bool p = false;
//    int t = 0;
//
//    J(int i, bool f, const string& c)
//        : i(i), f(f), c(c) {}
//
//    string fmt() const {
//        stringstream ss;
//        ss << "[" << i << (f ? "F" : "B");
//        if (p) ss << "*";
//        ss << "]";
//        return ss.str();
//    }
//
//    string fmtTQ() const {
//        stringstream ss;
//        ss << i << (f ? "F" : "B") << ":" << t;
//        return ss.str();
//    }
//};
//
//class JQ {
//private:
//    mutex m;
//    condition_variable cv;
//    list<shared_ptr<J>> fj;
//    list<shared_ptr<J>> bj;
//    map<int, shared_ptr<J>> tq;
//    atomic<int> bc{ 0 };
//
//public:
//    atomic<int> tc{ 0 };
//    void addJ(shared_ptr<J> j) {
//        lock_guard<mutex> lock(m);
//        if (j->f) {
//            fj.push_back(j);
//        }
//        else {
//            bj.push_back(j);
//            bc++;
//        }
//        tc++;
//    }
//
//    void sleep(int i, int s) {
//        lock_guard<mutex> lock(m);
//        auto it = find_if(fj.begin(), fj.end(), [&i](const auto& j) { return j->i == i; });
//        if (it != fj.end()) {
//            tq[i] = *it;
//            fj.erase(it);
//        }
//        else {
//            it = find_if(bj.begin(), bj.end(), [&i](const auto& j) { return j->i == i; });
//            if (it != bj.end()) {
//                tq[i] = *it;
//                bj.erase(it);
//                bc--;
//            }
//        }
//        tq[i]->t = s;
//    }
//
//    void wake() {
//        lock_guard<mutex> lock(m);
//        for (auto it = tq.begin(); it != tq.end();) {
//            if (--it->second->t <= 0) {
//                if (it->second->f) {
//                    fj.push_back(it->second);
//                }
//                else {
//                    bj.push_back(it->second);
//                    bc++;
//                }
//                it = tq.erase(it);
//            }
//            else {
//                ++it;
//            }
//        }
//    }
//
//    void display(int fpc) {
//        Sleep(Y);
//        lock_guard<mutex> lock(m);
//        cout << endl;
//        cout << "Running: [" << bc.load() << "B] [" << fpc << "F]" << endl;
//        cout << "---------------------------" << endl;
//        cout << "DQ: ";
//        if (!bj.empty()) {
//            cout << "(bottom) ";
//            for (const auto& j : bj) {
//                cout << j->fmt() << " ";
//            }
//            cout << endl << "P => ";
//        }
//        else {
//            cout << "(bottom/top)" << endl;
//        }
//        if (fj.empty()) {
//            cout << "[ ";
//            for (int i = 0; i < fpc; ++i) {
//                cout << i << "F ";
//            }
//            cout << "]";
//            cout << "(bottom/top)" << endl;
//        }
//        else {
//            cout << "(bottom/top)" << endl;
//        }
//        cout << "---------------------------" << endl;
//        cout << "WQ: [";
//        for (const auto& wj : tq) {
//            cout << wj.second->fmtTQ() << " ";
//        }
//        cout << "]" << endl << endl;
//    }
//};
//
//class Semaphore {
//private:
//    mutex mtx;
//    condition_variable cv;
//    int count;
//
//public:
//    explicit Semaphore(int count_ = 0) : count(count_) {}
//
//    void notify() {
//        unique_lock<mutex> lock(mtx);
//        count++;
//        cv.notify_one();
//    }
//
//    void wait() {
//        unique_lock<mutex> lock(mtx);
//        while (count == 0) {
//            cv.wait(lock);
//        }
//        count--;
//    }
//};
//
//void Echo(const vector<string>& t, int r, int p, int d, JQ& jq) {
//    Sleep(Y);
//    for (int i = 0; i < r; ++i) {
//        for (size_t j = 1; j < t.size(); ++j) {
//            cout << t[j] << " ";
//        }
//        cout << endl;
//
//        this_thread::sleep_for(chrono::seconds(p));
//
//        if (p * r > d) {
//            exit(0);
//        }
//    }
//}
//
//void Dummy(const vector<string>& t, int r, int p, int d, JQ& jq) {
//    Sleep(Y);
//    for (int i = 0; i < r; ++i) {
//        this_thread::sleep_for(chrono::seconds(p));
//
//        if (p * r > d) {
//            exit(0);
//        }
//    }
//}
//
//void GCD(const vector<string>& t, int r, int p, int d, JQ& jq) {
//    Sleep(Y);
//    for (int i = 0; i < r; ++i) {
//        if (t.size() != 3) {
//            cerr << "Invalid arguments for gcd command" << endl;
//            return;
//        }
//
//        int x = stoi(t[1]);
//        int y = stoi(t[2]);
//
//        while (y != 0) {
//            int temp = x % y;
//            x = y;
//            y = temp;
//        }
//
//        cout << x << endl;
//
//        this_thread::sleep_for(chrono::seconds(p));
//
//        if (p * r > d) {
//            exit(0);
//        }
//    }
//}
//
//bool isPrime(int n) {
//    if (n <= 1) return false;
//    if (n <= 3) return true;
//    if (n % 2 == 0 || n % 3 == 0) return false;
//    for (int i = 5; i * i <= n; i += 6) {
//        if (n % i == 0 || n % (i + 2) == 0) return false;
//    }
//    return true;
//}
//
//void Prime(const vector<string>& t, int r, int p, int d, JQ& jq) {
//    Sleep(Y);
//    for (int i = 0; i < r; ++i) {
//        if (t.size() != 2) {
//            cerr << "Invalid arguments for prime command" << endl;
//            return;
//        }
//
//        int x = stoi(t[1]);
//        int pc = 0;
//
//        for (int i = 2; i <= x; ++i) {
//            if (isPrime(i)) {
//                ++pc;
//            }
//        }
//
//        cout << pc << endl;
//
//        this_thread::sleep_for(chrono::seconds(p));
//
//        if (p * r > d) {
//            exit(0);
//        }
//    }
//}
//
//void Sum(const vector<string>& t, int r, int p, int d, JQ& jq) {
//    Sleep(Y);
//    for (int i = 0; i < r; ++i) {
//        if (t.size() != 2) {
//            cerr << "Invalid arguments for sum command" << endl;
//            return;
//        }
//
//        int x = stoi(t[1]);
//        long long s = 0;
//        const int MOD = 1000000;
//
//        for (int i = 1; i <= x; ++i) {
//            s = (s + i) % MOD;
//        }
//
//        cout << s << endl;
//
//        this_thread::sleep_for(chrono::seconds(p));
//
//        if (p * r > d) {
//            exit(0);
//        }
//    }
//}
//
//void procFile(const string& filename, int& fpc, JQ& jq) {
//    ifstream file(filename);
//    string line;
//
//    Semaphore fg_sem(1); // ForeGround semaphore
//
//    while (getline(file, line)) {
//        istringstream iss(line);
//        vector<string> t;
//        string tk;
//
//        while (iss >> tk) {
//            t.push_back(tk);
//        }
//
//        bool bg = false;
//        if (!t.empty() && t[0].front() == '&') {
//            bg = true;
//            t[0] = t[0].substr(1);
//        }
//
//        cout << "prompt> ";
//        for (const auto& tk : t) {
//            cout << tk << " ";
//        }
//        cout << endl;
//
//        int rc = 1;
//        int pd = 0;
//        int dr = 300;
//
//        for (size_t i = 0; i < t.size(); ++i) {
//            if (t[i] == "-n") {
//                if (i + 1 < t.size()) {
//                    rc = stoi(t[i + 1]);
//                }
//            }
//            else if (t[i] == "-p") {
//                if (i + 1 < t.size()) {
//                    pd = stoi(t[i + 1]);
//                }
//            }
//            else if (t[i] == "-d") {
//                if (i + 1 < t.size()) {
//                    dr = stoi(t[i + 1]);
//                }
//            }
//        }
//
//        vector<string> ctk;
//        for (size_t i = 0; i < t.size(); ++i) {
//            if (t[i] != "-n" && t[i] != "-p" && t[i] != "-d") {
//                ctk.push_back(t[i]);
//            }
//            else {
//                break;
//            }
//        }
//
//        auto command_exec = [ctk, rc, pd, dr, &jq, &fg_sem, bg]() {
//            if (!bg) {
//                fg_sem.wait();
//            }
//
//            const string& c = ctk[0];
//            if (c == "echo") {
//                Echo(ctk, rc, pd, dr, jq);
//            }
//            else if (c == "dummy") {
//                Dummy(ctk, rc, pd, dr, jq);
//            }
//            else if (c == "gcd") {
//                GCD(ctk, rc, pd, dr, jq);
//            }
//            else if (c == "prime") {
//                Prime(ctk, rc, pd, dr, jq);
//            }
//            else if (c == "sum") {
//                Sum(ctk, rc, pd, dr, jq);
//            }
//            else {
//                cerr << "Unknown command: " << c << endl;
//            }
//
//            if (!bg) {
//                fg_sem.notify();
//            }
//            };
//
//        thread th(command_exec);
//        if (bg) {
//            jq.addJ(make_shared<J>(jq.tc++, false, line));
//            th.detach();
//        }
//        else {
//            th.join();
//            fpc++;
//        }
//
//        jq.display(fpc);
//    }
//}
//
//int main() {
//    int fpc = 0;
//    string fn = "commands.txt";
//    JQ jq;
//
//    procFile(fn, fpc, jq);
//
//    return 0;
//}

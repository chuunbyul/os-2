//#define _CRT_SECURE_NO_WARNINGS
//#include <iostream>
//#include <vector>
//#include <string>
//#include <cstring>
//#include <sstream>
//#include <thread>
//#include <mutex>
//#include <algorithm>
//#include <chrono>
//#include <future>
//#include <map>
//#include <queue>
//
//// ����ü�� ���� ����
//struct Process {
//    int pid;
//    bool isBG;
//    bool isPromoted;
//    int remainingTime;
//    std::string command;
//};
//
//std::mutex fgMutex;
//std::mutex bgMutex;
//std::mutex printMutex;
//std::vector<Process> fgProcesses;
//std::vector<Process> bgProcesses;
//std::vector<Process> wqProcesses;
//int pidCounter = 0;
//
//// parse �Լ� ����
//char** parse(const std::string& command) {
//    std::vector<std::string> tokens;
//    std::istringstream iss(command);
//    std::string token;
//
//    while (iss >> token) {
//        tokens.push_back(token);
//    }
//
//    char** args = new char* [tokens.size() + 1];
//    for (size_t i = 0; i < tokens.size(); ++i) {
//        args[i] = new char[tokens[i].length() + 1];
//        std::strcpy(args[i], tokens[i].c_str());
//    }
//    args[tokens.size()] = nullptr;  // null-terminate the array
//
//    return args;
//}
//
//// �޸� ���� �Լ� ����
//void freeArgs(char** args) {
//    for (int i = 0; args[i] != nullptr; ++i) {
//        delete[] args[i];
//    }
//    delete[] args;
//}
//
//// exec �Լ� ����
//void exec(char** args) {
//    std::string command;
//    for (int i = 0; args[i] != nullptr; ++i) {
//        command += args[i];
//        if (args[i + 1] != nullptr) {
//            command += " ";
//        }
//    }
//    std::system(command.c_str());
//    freeArgs(args);
//}
//
//// ��ɾ� ���� �Լ� ����
//void executeWithDuration(const std::function<void()>& function, int durationInSeconds) {
//    auto startTime = std::chrono::steady_clock::now();
//    std::future<void> future = std::async(std::launch::async, function);
//
//    while (future.wait_for(std::chrono::milliseconds(100)) != std::future_status::ready) {
//        auto currentTime = std::chrono::steady_clock::now();
//        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
//        if (elapsed >= durationInSeconds) {
//            break;
//        }
//    }
//}
//
//// ���μ��� ���� ��� �Լ� ����
//void printProcessStatus() {
//    std::lock_guard<std::mutex> lock(printMutex);
//    std::cout << "---------------------------" << std::endl;
//    std::cout << "DQ: ";
//    for (const auto& proc : fgProcesses) {
//        std::cout << "[" << proc.pid << (proc.isPromoted ? "*" : "") << (proc.isBG ? "B" : "F") << "] ";
//    }
//    std::cout << std::endl;
//    std::cout << "WQ: ";
//    for (const auto& proc : wqProcesses) {
//        std::cout << "[" << proc.pid << (proc.isPromoted ? "*" : "") << (proc.isBG ? "B" : "F") << ":" << proc.remainingTime << "] ";
//    }
//    std::cout << std::endl;
//}
//
//// ��ɾ� ���� �Լ� ����
//void executeCommand(const std::vector<std::string>& commandTokens, int repeatCount, int period, int duration, bool isBG) {
//    std::string command = commandTokens[0];
//    for (size_t i = 1; i < commandTokens.size(); ++i) {
//        command += " " + commandTokens[i];
//    }
//
//    auto function = [command, repeatCount, period]() {
//        for (int i = 0; i < repeatCount; ++i) {
//            exec(parse(command));
//            std::this_thread::sleep_for(std::chrono::seconds(period));
//        }
//        };
//
//    if (isBG) {
//        std::thread bgThread([&]() { executeWithDuration(function, duration); });
//        bgThread.detach();
//    }
//    else {
//        executeWithDuration(function, duration);
//    }
//}
//
//// ����� ��ɾ� �Է� �� ó�� �Լ� ����
//void processUserCommands() {
//    std::string line;
//    while (true) {
//        std::cout << "prompt> ";
//        std::getline(std::cin, line);
//
//        if (line.empty()) continue;
//
//        std::istringstream iss(line);
//        std::vector<std::string> tokens;
//        std::string token;
//
//        while (iss >> token) {
//            tokens.push_back(token);
//        }
//
//        int repeatCount = 1;
//        int period = 0;
//        int duration = 300;
//        bool isBG = false;
//        bool isPromoted = false;
//
//        for (size_t i = 0; i < tokens.size(); ++i) {
//            if (tokens[i] == "-n") {
//                if (i + 1 < tokens.size()) {
//                    repeatCount = std::stoi(tokens[i + 1]);
//                }
//            }
//            else if (tokens[i] == "-p") {
//                if (i + 1 < tokens.size()) {
//                    period = std::stoi(tokens[i + 1]);
//                }
//            }
//            else if (tokens[i] == "-d") {
//                if (i + 1 < tokens.size()) {
//                    duration = std::stoi(tokens[i + 1]);
//                }
//            }
//            else if (tokens[i] == "&") {
//                isBG = true;
//            }
//        }
//
//        std::string command;
//        for (const auto& token : tokens) {
//            if (token != "-n" && token != "-p" && token != "-d" && token != "&") {
//                command += token + " ";
//            }
//        }
//        command.pop_back(); // Remove trailing space
//
//        Process process = { pidCounter++, isBG, isPromoted, duration, command };
//        if (isBG) {
//            bgProcesses.push_back(process);
//        }
//        else {
//            fgProcesses.push_back(process);
//        }
//
//        executeCommand(tokens, repeatCount, period, duration, isBG);
//
//        std::cout << "Running: [" << process.pid << (isBG ? "B" : "F") << "]" << std::endl;
//        printProcessStatus();
//    }
//}
//
//int main() {
//    processUserCommands();
//    return 0;
//}

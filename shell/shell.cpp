//
// Created by max on 17.04.18.
//

#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <unistd.h>
#include <wait.h>
#include <sstream>
#include <cstring>
#include <string>
#include <cerrno>

char ** split(std::string const& s) {
    std::vector<std::string> v;
    auto in = std::istringstream(s);
    std::string arg;
    while(in >> arg) {
        v.emplace_back(std::move(arg));
    }

    auto sz = v.size();
    auto res = new char* [sz+1];
    for (int i = 0; i < sz; ++i) {
        res[i] = const_cast<char*>(v[i].data());
    }
    res[sz] = nullptr;
    return res;
}

void _perror(std::string const& message) {
    std::cerr << message << std::endl << strerror(errno) << std::endl;
}

void execute(char * args[], char * envp[]) {
    pid_t pid;
    int status;

    pid = fork();
    if(pid == 0) {
        if(execve(args[0], args, envp) == -1) {
            _perror("Execution is failed.");
            exit(-1);
        }
    } else if(pid < 0) {
        _perror("Error forking.");
    } else {
        if(waitpid(pid, &status, 0) == -1) {
            _perror("Parent process is failed.");
        }
    }
}

void _print() {
    std::cout << "$ ";
    std::cout.flush();
}

int main(int argc, char * argv[], char * envp[]) {
    std::string line;
    _print();
    while(std::getline(std::cin, line)) {
        if(line == "exit") break;
        auto args = split(line);
        auto uptr = std::unique_ptr<char*[]>(args);
        execute(uptr.get(), envp);
        _print();
    }

    return 0;
}

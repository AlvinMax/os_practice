//
// Created by max on 17.04.18.
//

#include <string>
#include <iostream>
#include <vector>
#include <memory.h>
#include <unistd.h>
#include <wait.h>

std::vector<char*> split(std::string &s) {
    char* ss = strdup(s.c_str());
    std::vector<char*> v;

    char* arg = strtok(ss, " ");
    while (arg != NULL) {
        v.push_back(arg);
        arg = strtok(NULL, " \n\r\t");
    }
    v.push_back('\0');
    return v;
}

void execute(char **args) {
    pid_t pid;
    int status;

    pid = fork();
    if(pid == 0) {
        if(execvp(args[0], args) == -1) {
            perror("Execution is failed.");
            exit(-1);
        }
    } else if(pid < 0) {
        perror("Error forking.");
    } else {
        if(waitpid(pid, &status, 0) == -1) {
            perror("Parent process is failed.");
        }
    }
}

int main(int argc, char ** argv) {
    std::string line;
    printf("$ ");
    while(std::getline(std::cin, line)) {
        if(line == "exit") break;
        std::vector<char*> args = split(line);
        execute(args.data());
        free(args[0]);
        printf("$ ");
    }

    return 0;
}
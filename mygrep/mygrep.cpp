//
// Created by max on 07.04.18.
//

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unordered_map>
#include <set>
#include <cstring>
#include <vector>
#include <cerrno>

std::unordered_map<ino_t, char*> mp;

inline void print_map() {
    for (auto x : mp) {
        char* t = x.second;

        char buf[strlen(t) - 2];
        memcpy(buf, &t[2], strlen(t) - 1);
        buf[strlen(t) - 2] = '\0';
        puts(buf);

        free(x.second);
    }
}

bool cmp(const char* x, const char* y) {
    auto n = strlen(x), m = strlen(y);

    if(n == m) {
        return strcmp(x, y) < 0;
    }

    return n < m;
}

void do_grep(char* filename, char * &pattern) {
    FILE *f;
    f = fopen(filename, "r");
    if(!f) {
        perror(filename);
        exit(1);
    }

    const size_t BUFF_SIZE = 2048;
    const size_t CHECK_SIZE = 2*BUFF_SIZE;

    char buf[BUFF_SIZE];
    char check[CHECK_SIZE];
    memset(check, 255, sizeof(check));

    while(fgets(buf, BUFF_SIZE + 1, f)) {
        strncpy(&check[BUFF_SIZE], buf, sizeof(buf));
        if(strstr(check, pattern) != nullptr) {
              struct stat sb{};
              if(stat(filename, &sb) == -1) {
                  perror("stat error");
                  exit(1);
              }

              ino_t inode = sb.st_ino;

              auto * t = static_cast<char *>(malloc(strlen(filename)));
              strcpy(t, filename);
              if(!mp.count(inode) || cmp(t, mp[inode])) mp[inode] = t;
              return;
        }
        strncpy(check, buf, sizeof(buf));
    }
}


std::set<ino_t> used;
void listdir(char* dir, char * &pattern) {
    DIR *dp;
    struct dirent *entry;

    if((dp = opendir(dir)) == nullptr)
    {
        fprintf(stderr, "Cannot open directory: %s\n", dir);
        return;
    }

    while((entry = readdir(dp)) != nullptr)
    {
        auto t = static_cast<char *>(malloc(strlen(dir) + strlen(entry->d_name) + 2));
        strcpy(t, dir);
        strcat(t, "/");
        strcat(t, entry->d_name);

        struct stat t_stat{};
        stat(t, &t_stat);
        ino_t t_inode = t_stat.st_ino;

        if(S_ISDIR(t_stat.st_mode))
        {
            if(strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0) continue;
            if(!used.count(t_inode)) {
                used.insert(t_inode);
                listdir(t, pattern);
            }
        } else if(S_ISREG(t_stat.st_mode)) {
            do_grep(t, pattern);
        }

        free(t);
    }

    closedir(dp);
}

int main(int argc, char **argv) {
    if(argc != 2) {
        fprintf(stderr, "[WARNING] Usage: mygrep \"PATTERN\".");
        return -1;
    }

    //file listing
    listdir(const_cast<char *>("."), argv[1]);
    print_map();

    return 0;
}

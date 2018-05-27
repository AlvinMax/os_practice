//
// Created by max on 27.05.18.
//

#include <iostream>
#include <sys/mman.h>
#include <cstring>

/*
int function()
{
    return 14; //My(or your?) day of birthday.
}
*/

unsigned char function_code[] = {
        /*0x00000000004004d6 <+0>:*/	0x55,	/*push   %rbp*/
        /*0x00000000004004d7 <+1>:*/	0x48, 0x89, 0xe5,	/*mov    %rsp,%rbp*/
        /*0x00000000004004da <+4>:*/	0xb8, 0x0e, 0x00, 0x00, 0x00,	/*mov    $0xe,%eax*/
        /*0x00000000004004df <+9>:*/	0x5d,	/*pop    %rbp*/
        /*0x00000000004004e0 <+10>:*/	0xc3	/*retq*/
};

const size_t ARGS_COUNT = 11;

void safe_munmap(void* ptr, size_t sz) {
    int res;
    if ((res = munmap(ptr, sz)) != 0) {
        perror("Error clear of mapped memory.");
        exit(-1);
    }
}

int main(int argc, char* argv[]) {
    if(argc < 1 || argc > 2) {
        printf("Expected 0 or 1 arguments\nUsage: jit_compiler [<number>]\n");
        return -1;
    }
    if(argc == 2) {
        //patching code
        int new_arg = std::stoi(std::string(argv[1]));

        unsigned char blocks[] = {
                static_cast<unsigned char>(new_arg & 0xFF),
                static_cast<unsigned char>((new_arg >> 8) & 0xFF),
                static_cast<unsigned char>((new_arg >> 16) & 0xFF),
                static_cast<unsigned char>((new_arg >> 24) & 0xFF)
        };
        for (int i = 5; i < 9; ++i) {
            function_code[i] = blocks[i - 5];
        }
    }

    // 1.Выделить память с помощью mmap(2)
    void* ptr;
    if ((ptr = mmap(nullptr,
                   ARGS_COUNT * sizeof(unsigned char),
                   PROT_READ | PROT_WRITE,
                   MAP_PRIVATE | MAP_ANONYMOUS,
                   -1,
                   0)) == (void*) -1)
    {
        perror("mmap failed.");
        return -1;
    }

    // 2.Записать в выделенную память машинный код, соответсвующий какой-либо функции
    memcpy(ptr, function_code, ARGS_COUNT);

    // 3.Изменить права на выделенную память - чтение и исполнение. See: mprotect(2)
    if (mprotect(ptr, ARGS_COUNT, PROT_READ | PROT_EXEC) != 0)
    {
        perror("Error setting mapped memory as readable and executable");
        // 5.Освободить выделенную память
        safe_munmap(ptr, ARGS_COUNT);
        return -1;
    }

    // 4.Вызвать функцию по указателю на выделенную память
    int (*func) () = (int(*)())ptr;
    printf("%d\n", func());

    // 5.Освободить выделенную память
    safe_munmap(ptr, ARGS_COUNT);

    return 0;
}
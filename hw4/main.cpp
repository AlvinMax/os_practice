#include <iostream>
#include <dlfcn.h>
#include "static/add.h"
#include "shared/sub.h"

using namespace std;

int main() {

    cout << add(14, 6) << '\n';
    cout << sub(14, 6) << '\n';

    void* extern_lib = dlopen("./libs/loadable_lib.so", RTLD_LAZY);
    if (extern_lib == nullptr) {
        cerr << dlerror() << endl;
        return 1;
    }

    int (*mul) (int, int) = (int (*) (int, int)) dlsym(extern_lib, "_Z3mulii");
    if (mul == nullptr) {
        cerr << dlerror() << endl;
        dlclose(extern_lib);
        return 1;
    }

    cout << mul(14, 6) << endl;
    dlclose(extern_lib);
    return 0;
}
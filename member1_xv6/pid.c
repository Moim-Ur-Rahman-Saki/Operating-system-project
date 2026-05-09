#include "types.h"
#include "stat.h"
#include "user.h"

struct pinfo info;

char* getStateName(int state) {
    switch(state) {
        case 0: return "UNUSED";
        case 1: return "EMBRYO";
        case 2: return "SLEEPING";
        case 3: return "RUNNABLE";
        case 4: return "RUNNING";
        case 5: return "ZOMBIE";
        default: return "UNKNOWN";
    }
}

int main() {
    if(getprocinfo(&info) < 0){
        printf(1, "Error\n");
        exit();
    }

    printf(1, "PID: %d\n", info.pid);
    printf(1, "Name: %s\n", info.name);
    printf(1, "State: %s\n", getStateName(info.state));

    exit();
}
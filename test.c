#include "types.h"
#include "stat.h"
#include "user.h"

int main() {
  int pid1 = fork();

  if(pid1 == 0){
    setpriority(getpid(), 1);
    for(int i = 0; i < 10; i++){
      printf(1, "HIGH\n");
    }
    exit();
  }

  int pid2 = fork();

  if(pid2 == 0){
    setpriority(getpid(), 10);
    for(int i = 0; i < 10; i++){
      printf(1, "LOW\n");
    }
    exit();
  }

  wait();
  wait();

  exit();
}
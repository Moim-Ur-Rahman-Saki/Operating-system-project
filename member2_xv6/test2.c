#include "types.h"
#include "stat.h"
#include "user.h"

int main(void)
{
  int pid1 = fork();

  if(pid1 == 0){
    setpriority(getpid(), 1); // HIGH priority

    for(int i = 0; i < 5; i++){
      printf(1, "HIGH\n");
      for(volatile int j = 0; j < 200000000; j++); // BIG delay
    }

    printf(1, "HIGH waiting time: %d\n", getwaitingtime());
    exit();
  }

  int pid2 = fork();

  if(pid2 == 0){
    setpriority(getpid(), 10); // LOW priority

    for(int i = 0; i < 5; i++){
      printf(1, "LOW\n");
      for(volatile int j = 0; j < 200000000; j++); // SAME delay
    }

    printf(1, "LOW waiting time: %d\n", getwaitingtime());
    exit();
  }

  wait();
  wait();

  exit();
}
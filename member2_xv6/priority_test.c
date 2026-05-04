#include "types.h"
#include "stat.h"
#include "user.h"

int main(void)
{
  int pid1 = fork();

  if(pid1 == 0){
    setpriority(getpid(), 1); // HIGH priority
    printf(1, "Process HIGH (priority=1) started\n");

    for(int i = 0; i < 5; i++){
      printf(1, "HIGH running\n");
      for(volatile int j = 0; j < 100000000; j++);
    }

    exit();
  }

  int pid2 = fork();

  if(pid2 == 0){
    setpriority(getpid(), 10); // LOW priority
    printf(1, "Process LOW (priority=10) started\n");

    for(int i = 0; i < 5; i++){
      printf(1, "LOW running\n");
      for(volatile int j = 0; j < 100000000; j++);
    }

    exit();
  }

  wait();
  wait();

  printf(1, "\n Priority scheduling test finished\n");

  exit();
}
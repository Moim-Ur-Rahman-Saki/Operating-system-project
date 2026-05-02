#include "types.h"
#include "stat.h"
#include "user.h"

int main() {
  int pid = getpid();

  printf(1, "Before setting priority\n");
  setpriority(pid, 5);
  printf(1, "Priority set to 5\n");

  exit();
}
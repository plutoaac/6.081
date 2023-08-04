#include "kernel/types.h"
#include "user/user.h"
/*
   fork 出子进程去读素数 然后把他的倍数全部筛了
*/
void primes(int piperead) {
  int pri;
  if (read(piperead, &pri, sizeof(pri)) <= 0) exit(0);
  printf("prime %d\n", pri);
  int pipefd2[2];
  pipe(pipefd2);
  int fd2 = fork();
  if (fd2 == 0) {
    close(pipefd2[1]);
    primes(pipefd2[0]);
    close(pipefd2[0]);
  } else {
    int tmp;
    close(pipefd2[0]);
    while (read(piperead, &tmp, sizeof(tmp))) {
      if (tmp % pri != 0) {
        write(pipefd2[1], &tmp, sizeof(tmp));
      }
    }
    close(pipefd2[1]);
    wait(0);
  }
  exit(0);
}
int main(int argc, char* argv[]) {
  int pipefd[2];
  pipe(pipefd);
  int fd = fork();
  if (fd == 0) {
    close(pipefd[1]);
    primes(pipefd[0]);
    close(pipefd[0]);
  } else {
    close(pipefd[0]);
    for (int i = 2; i <= 35; i++) write(pipefd[1], &i, sizeof(i));
    close(pipefd[1]);
    wait(0);
  }
  exit(0);
}
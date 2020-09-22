#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

/* 管道通信只能在具有两个公共祖先的进程之间使用， 不是任意两个进程都可以 */

#define MAXLINE 1024

int main() {
    int n;
    int fd[2];
    pid_t pid;
    char line[MAXLINE];

    if (pipe(fd) < 0)
        exit(1);
    if ((pid = fork()) < 0) 
        exit(2);
    else if (pid > 0) {  /* parent */
        close(fd[0]);
        write(fd[1], "hello pipe\n", 11);
    }
    else {
        close(fd[1]);
        n = read(fd[0], line, MAXLINE);
        write(STDOUT_FILENO, line, n);
    }
    exit(0);
}
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
/* 将文件映射到内存  */


int main(int argc, char** argv) {
    int fd = open(argv[1], O_RDONLY);
    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        perror("cant get file stat");
    }

    char* file_in_memory = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    srand(45);

    for (int i=0; i<500000; i++) {
        int index = rand() % sb.st_size;
        printf("%c", file_in_memory[index]);
    }
    close(fd);
}
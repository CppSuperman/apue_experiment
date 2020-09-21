#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

/* 将文件映射到内存  */


void main(int argc, char** argv) {
    FILE* fp = fopen(argv[1], "r");
    struct stat sb;
    if (stat(argv[1], &sb) == -1) {
        perror("cant get file stat");
        return;
    }

    
    srand(45);

    for (int i=0; i<500000; i++) {
        int index = rand() % sb.st_size;
        fseek(fp, index, SEEK_SET);
        printf("%c", fgetc(fp));
    }
    fclose(fp);
}
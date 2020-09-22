#include <stdio.h>
#include <stdlib.h>

#define PAGER "${PAGER:-more}"
#define MAXLINE 1024

int main(int argc, char** argv) {
    char line[MAXLINE];
    FILE *fpin, *fpout;
    if (argc != 2)
        exit(-1);
        
    if ((fpin = fopen(argv[1], "r")) == NULL) {
        exit(-2);
    }
    if ((fpout = popen(PAGER, "w")) == NULL) {
        exit(-3);
    }
    while (fgets(line, MAXLINE, fpin) != NULL)
    {
        if (fputs(line, fpout) == EOF) {
            exit(-4);
        }
    }
    if (ferror(fpin))
        exit(-5);
    if (pclose(fpout) == -1)
        exit(-6);
    exit(0);
    
}
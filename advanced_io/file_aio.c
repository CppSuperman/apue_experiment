#include <aio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*
posix 标准 文件异步io
*/

#define BSZ 4096

unsigned char translate(unsigned char c) {
    if (isalpha(c)) {
        if (c >= 'n')
            c -= 13;
        else if (c >= 'a')
            c += 13;
        else if (c >= 'N')
            c -= 13;
        else
            c += 13;
    }
    return c;
}


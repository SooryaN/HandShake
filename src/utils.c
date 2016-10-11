#include "stdheaders.h"
#include "utils.h"

int run_cmd(char *cmd, ...)
{
    va_list ap;
    char buf[CMDBUFLEN];
    va_start(ap, cmd);
    vsnprintf(buf, CMDBUFLEN, cmd, ap);

    va_end(ap);

    printf("%s\n", buf);

    return system(buf);
}

void print_hex(char *str, int len){
    printf("\nHex dumping:");
    for(int i=0; i<len; i++){
        if (i%8 == 0) printf("\n");
        printf("%02x ",(unsigned char)str[i]);
    }
    printf("\n");
}

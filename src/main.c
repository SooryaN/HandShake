#include "stdheaders.h"
#include "utils.h"
#include "tuntap.h"

int main(int argc, char const *argv[]){
    int tun_fd;
    char buf[100];
    char *dev = calloc(10, 1);
    CLEAR(buf);
    tun_fd = tun_alloc(dev);

    if (set_if_up(dev) != 0)
        printf("ERROR when setting up if\n");

    if (set_if_address(dev, "10.0.0.5/24") != 0)
        printf("ERROR when setting address for if\n");

    if (set_if_route(dev, "10.0.0.0/24") != 0)
        printf("ERROR when setting route for if\n");

    for(;;){
        read(tun_fd, buf, 100);
        print_hex(buf, 100);
    }

    free(dev);

    return 0;
}

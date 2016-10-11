#include "stdheaders.h"

#define BUFFERLEN 100

int run_cmd(char *cmd, ...)
{
    va_list ap;
    char buf[BUFFERLEN];

    va_start(ap, cmd);
    vsnprintf(buf, BUFFERLEN, cmd, ap);
    va_end(ap);

    printf("%s\n", buf);
    return system(buf);
}

int set_if_up(char *dev){
    return run_cmd("ip link set dev %s up", dev);
}

int set_if_address(char *dev, char *cidr){
    return run_cmd("ip address add dev %s local %s", dev, cidr);
}

int set_if_route(char *dev, char *cidr){
    return run_cmd("ip route add dev %s %s", dev, cidr);
}

/*
 * Taken directly from the kernel Documentation
 * (https://www.kernel.org/doc/Documentation/networking/tuntap.txt)
 */
int tun_alloc(char *dev){
    struct ifreq ifr;
    int fd, err;

    if( (fd = open("/dev/net/tun", O_RDWR)) < 0 ) {
        printf("Cannot open TUN/TAP dev");
        exit(1);
    }

    memset(&ifr, 0, sizeof(ifr));

    /* Flags: IFF_TUN   - TUN device (no Ethernet headers)
     *        IFF_TAP   - TAP device
     *
     *        IFF_NO_PI - Do not provide packet information
     */
    ifr.ifr_flags = IFF_TUN;
    if( *dev ) {
        strncpy(ifr.ifr_name, dev, IFNAMSIZ);
    }

    if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 ){
        printf("ERR: Could not ioctl tun: %s\n", strerror(errno));
        close(fd);
        return err;
    }

    strcpy(dev, ifr.ifr_name);
    return fd;
}

int main(int argc, char const *argv[]){
    int tun_fd;
    char buf[100];
    char *dev = calloc(10, 1);
    CLEAR(buf);

    // to create TUN device
    // $ mknod /dev/net/tun c 10 200

    tun_fd = tun_alloc(dev);

    if (set_if_up(dev) != 0)
        printf("ERROR when setting up if\n");

    if (set_if_address(dev, "10.0.0.5/24") != 0)
        printf("ERROR when setting address for if\n");

    if (set_if_route(dev, "10.0.0.0/24") != 0)
        printf("ERROR when setting route for if\n");

    read(tun_fd, buf, 100);

    free(dev);
    return 0;
}

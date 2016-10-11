#include "stdheaders.h"
#include "utils.h"

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

    if( (fd = open("/dev/net/tap", O_RDWR)) < 0 ) {
        print_err("Cannot open TUN/TAP dev");
        exit(1);
    }

    CLEAR(ifr);

    /* Flags: IFF_TUN   - TUN device (no Ethernet headers)
     *        IFF_TAP   - TAP device
     *
     *        IFF_NO_PI - Do not provide packet information
     */
    ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
    if( *dev ) {
        strncpy(ifr.ifr_name, dev, IFNAMSIZ);
    }

    if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 ){
        print_err("ERR: Could not ioctl tun: %s\n", strerror(errno));
        close(fd);
        return err;
    }

    strcpy(dev, ifr.ifr_name);
    return fd;
}

#include "netdev.h"
#include "stdheaders.h"

void netdev_init(struct netdev* dev, char* addr, char* hwaddr)
{
    CLEAR(*dev);

    if (inet_pton(AF_INET, addr, &dev->addr) != 1) {
        perror("ERR: Parsing inet address failed");
        exit(1);
    }

    sscanf(hwaddr, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &dev->hw_addr[0]
                                                  , &dev->hw_addr[1]
                                                  , &dev->hw_addr[2]
                                                  , &dev->hw_addr[3]
                                                  , &dev->hw_addr[4]
                                                  , &dev->hw_addr[5]);
}

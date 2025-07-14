/*
 * network.c
 * C bindings to obtain network interface information.
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <net/if.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

void
IPv4(const char *if_name)
{
    int dummy_socket;
    struct ifreq interface;

    dummy_socket = socket(PF_INET, SOCK_DGRAM, 0);  /* dummy socket */
    strncpy(interface.ifr_name, if_name, IFNAMSIZ); /* get IPv4 address */

    int err = ioctl(dummy_socket, SIOCGIFADDR, &interface);

    if (err) exit(-1);

    struct sockaddr_in ip_addr = *((struct sockaddr_in*) &interface.ifr_addr);

    char *ip = inet_ntoa(ip_addr.sin_addr);
    printf("IP: %s\n", ip);

    close(dummy_socket);
    return;
}

int
main(void)
{
    size_t size = 128;
    char hostname[size];
    int err = gethostname(hostname, size);

    if (err) return -1;

    printf("%s\n", hostname);

    IPv4("wlan0");
    return 0;
}

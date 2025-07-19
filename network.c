/* network.c
 * C bindings to obtain network interface information.
 *
 * Copyright (C) 2025 by Abhigyan <314abh@gmail.com>
 */

/*
 * TODO: Implement C bindings.
 * TODO: Implement IPv6 support.
 * TODO: Implement any extra things required to handle ethernet
 *       interfaces.
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include <net/if.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#define HOSTNAMSIZ 256
#define ERROR(v)   if (v) exit(-1);

struct Host
{
  char *ip;
  char *ip_remote;
  char *ip_broadcast;
  char hostname[HOSTNAMSIZ];
};

/*
 * Read man pages for `sockaddr_in', `socket' and `netdevice' if you
 * want to really understand what's going on below.
 */
void
get_host(struct Host* host, const char *if_name)
{
  if (gethostname(host->hostname, HOSTNAMSIZ)) exit(-1); /* assign hostname */

  struct ifreq interface;                         	 /* get network interface */
  strncpy(interface.ifr_name, if_name, IFNAMSIZ);	 /* set interface name */
  int dummy_socket = socket(PF_INET, SOCK_DGRAM, 0);	 /* create dummy socket */

  ERROR(ioctl(dummy_socket, SIOCGIFADDR, &interface));
  ERROR(ioctl(dummy_socket, SIOCGIFDSTADDR, &interface));
  ERROR(ioctl(dummy_socket, SIOCGIFBRDADDR, &interface));

  close(dummy_socket);
  
  struct sockaddr_in ip_addr = *((struct sockaddr_in*) &interface.ifr_addr);
  struct sockaddr_in ip_remote_addr = *((struct sockaddr_in*) &interface.ifr_dstaddr);
  struct sockaddr_in ip_broadcast_addr = *((struct sockaddr_in*) &interface.ifr_broadaddr);
  
  host->ip = inet_ntoa(ip_addr.sin_addr);
  host->ip_remote = inet_ntoa(ip_remote_addr.sin_addr);
  host->ip_broadcast = inet_ntoa(ip_broadcast_addr.sin_addr);
}

/* int */
/* main(void) */
/* { */
/*   struct Host host = { 0 }; */
/*   get_host(&host, "wlan0"); */

/*   printf("%s\n", host.hostname); */
/*   printf("%s\n", host.ip); */
/*   printf("%s\n", host.ip_remote); */
/*   printf("%s\n", host.ip_broadcast); */
/*   return 0; */
/* } */

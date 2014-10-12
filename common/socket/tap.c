/*
 *
 *	This is free software. You can redistribute it and/or modify under
 *	the terms of the GNU General Public License version 2.
 *
 * 	Copyright (C) 1998 by kra
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/ioctl.h>
#ifdef _WITH_LINUX_KERNEL_HDR
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if.h>
extern int socket(int domain, int type, int protocol);
#define HL_MODE_NR 0
extern char *host_lookup(unsigned int in, int use_mode);
extern int verbose;
#else
#include <net/ethernet.h>
#include <net/if.h>
#if defined __APPLE__ && defined __MACH__
#define ETH_ALEN 6
#define SOCK_PACKET SOCK_RAW
#else
#include <net/if_packet.h>
#endif
#include <sys/socket.h>
#include <netinet/in.h>
int verbose = 1;
#define HL_MODE_NR 0
#endif



unsigned char my_eth_mac[ETH_ALEN];
unsigned int my_eth_ip;

int tap(const char *device, int promisc_mode)
{
	
	int fd;				
	struct ifreq ifr;   /* Link-layer interface request structure */
        	            /* Ethernet code for IP 0x0800==ETH_P_IP */
	/*
	 * here ETH_P_IP  - je to jedno, stejne pres nej poslu ARP packet
	 * 		    nevim ale jestli ho prijmu?
	 */
	/*
	 * look at tcpdump
	 */
	if ((fd = socket(AF_INET, SOCK_PACKET, 
			/*htons(ETH_P_IP)*/ htons(ETH_P_ALL))) < 0) {
	        if (verbose)
			perror("(tap) SOCK_PACKET allocation problems [fatal]");
	        return -1;					           
	}
	strncpy(ifr.ifr_name, device, IFNAMSIZ);
	ifr.ifr_name[IFNAMSIZ - 1] = 0;
	if ((ioctl(fd, SIOCGIFFLAGS, &ifr)) < 0) {    /* Get the device info */
	        if (verbose)
			perror("(tap) Can't get device flags [fatal]");
	        close(fd);
      		return -1;
	}
	if (!promisc_mode)
		ifr.ifr_flags &= ~IFF_PROMISC;    /* Unset promiscuous mode */
	else
		ifr.ifr_flags |= IFF_PROMISC;        /* Set promiscuous mode */
#if 0
	if (ifr.ifr_flags & IFF_SOFTHEADERS) {
		printf("had softheaders\n");
		ifr.ifr_flags ^= IFF_SOFTHEADERS;
	}
#endif
	if ((ioctl(fd, SIOCSIFFLAGS, &ifr)) < 0) {    /* Set flags */
        	if (verbose)
			perror("(tap) Can't set/unset promiscuous mode [fatal]");
		close(fd);
		return -1;
	}
	if(!promisc_mode){
        	close(fd);
	        return 0;
	} else {
		if (ioctl(fd, SIOCGIFADDR, &ifr) < 0) {
			if (verbose)
				perror("(tap) Can't get interface IP address");
			tap(device, 0);
			return -1;
		}
		my_eth_ip = *(unsigned int *) (ifr.ifr_addr.sa_data + 2);
		if (ioctl(fd, SIOCGIFHWADDR, &ifr) < 0) {
			if (verbose)
				perror("(tap) Can't get interface HW address");
			tap(device, 0);
			return -1;
		}
		memcpy(my_eth_mac, ifr.ifr_hwaddr.sa_data, ETH_ALEN);
		if (verbose) {
			printf("listeining on %s %s ", device,
			       "localhost");
			       //host_lookup(my_eth_ip, HL_MODE_NR));
			//print_eth_mac(my_eth_mac);
			printf("\n");
		}
		return fd;
	}
}

int get_ifc_info(char *ifc_name, unsigned int *ip, char *mac)
{
	int fd;				
	struct ifreq ifr;
	
	if ((fd = socket(AF_INET, SOCK_RAW, /*htons(ETH_P_IP)*/ htons(ETH_P_ALL))) < 0) {
		return -1;
	}
	strncpy(ifr.ifr_name, ifc_name, IFNAMSIZ);
	ifr.ifr_name[IFNAMSIZ - 1] = 0;
	
	if (ip) {
		if (ioctl(fd, SIOCGIFADDR, &ifr) < 0) {
			close(fd);
			return -1;
		}
		*ip = *(unsigned int *) (ifr.ifr_addr.sa_data + 2);
	}
	if (mac) {
		if (ioctl(fd, SIOCGIFHWADDR, &ifr) < 0) {
			close(fd);
			return -1;
		}
		memcpy(mac, ifr.ifr_hwaddr.sa_data, ETH_ALEN);
		printf("mac on %s ", ifc_name);
		//print_eth_mac(mac);
		printf("\n");
	}
	close(fd);
	return 0;
}


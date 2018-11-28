#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <string.h>
#include <netinet/in.h>
#include <linux/udp.h>
#include <linux/tcp.h>
#include <signal.h>
#include <time.h>
#define ERROR 1
#define OK 0

void handle(int nsignal);

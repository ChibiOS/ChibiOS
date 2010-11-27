# List of the required lwIP files.
LWNETIFSRC = \
        ${CHIBIOS}/ext/lwip/src/netif/etharp.c \
        ${CHIBIOS}/ext/lwip/src/netif/loopif.c

LWCORESRC = \
        ${CHIBIOS}/ext/lwip/src/core/dhcp.c \
        ${CHIBIOS}/ext/lwip/src/core/dns.c \
        ${CHIBIOS}/ext/lwip/src/core/init.c \
        ${CHIBIOS}/ext/lwip/src/core/mem.c \
        ${CHIBIOS}/ext/lwip/src/core/memp.c \
        ${CHIBIOS}/ext/lwip/src/core/netif.c \
        ${CHIBIOS}/ext/lwip/src/core/pbuf.c \
        ${CHIBIOS}/ext/lwip/src/core/raw.c \
        ${CHIBIOS}/ext/lwip/src/core/stats.c \
        ${CHIBIOS}/ext/lwip/src/core/sys.c \
        ${CHIBIOS}/ext/lwip/src/core/tcp.c \
        ${CHIBIOS}/ext/lwip/src/core/tcp_in.c \
        ${CHIBIOS}/ext/lwip/src/core/tcp_out.c \
        ${CHIBIOS}/ext/lwip/src/core/udp.c

LWIPV4SRC = \
        ${CHIBIOS}/ext/lwip/src/core/ipv4/autoip.c \
        ${CHIBIOS}/ext/lwip/src/core/ipv4/icmp.c \
        ${CHIBIOS}/ext/lwip/src/core/ipv4/igmp.c \
        ${CHIBIOS}/ext/lwip/src/core/ipv4/inet.c \
        ${CHIBIOS}/ext/lwip/src/core/ipv4/inet_chksum.c \
        ${CHIBIOS}/ext/lwip/src/core/ipv4/ip.c \
        ${CHIBIOS}/ext/lwip/src/core/ipv4/ip_addr.c \
        ${CHIBIOS}/ext/lwip/src/core/ipv4/ip_frag.c

LWAPISRC = \
        ${CHIBIOS}/ext/lwip/src/api/api_lib.c \
        ${CHIBIOS}/ext/lwip/src/api/api_msg.c \
        ${CHIBIOS}/ext/lwip/src/api/err.c \
        ${CHIBIOS}/ext/lwip/src/api/netbuf.c \
        ${CHIBIOS}/ext/lwip/src/api/netdb.c \
        ${CHIBIOS}/ext/lwip/src/api/netifapi.c \
        ${CHIBIOS}/ext/lwip/src/api/sockets.c \
        ${CHIBIOS}/ext/lwip/src/api/tcpip.c

LWINC = \
        ${CHIBIOS}/ext/lwip/src/include \
        ${CHIBIOS}/ext/lwip/src/include/ipv4

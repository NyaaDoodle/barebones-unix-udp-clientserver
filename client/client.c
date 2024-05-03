#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

int main(void) {
    int m_socket, status;
    struct addrinfo hints;
    struct addrinfo *serverinfo;
    char send_buf[256];
    char recv_buf[256];
    int bytes_sent = 0;
    int bytes_recv = 0;
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    if ((status = getaddrinfo("127.0.0.1", "3490", &hints, &serverinfo)) != 0) {
        fprintf(stderr, "getaddrinfo() error: %s\n", gai_strerror(status));
        exit(-1);
    }
    
    m_socket = socket(serverinfo->ai_family, serverinfo->ai_socktype, serverinfo->ai_protocol);
    if (m_socket == -1) {
        perror("socket");
        exit(-1);
    }

    strcpy(send_buf, "Testing uwu");
    bytes_sent = sendto(m_socket, send_buf, strlen(send_buf), 0, serverinfo->ai_addr, serverinfo->ai_addrlen);
    if (bytes_sent == -1) {
        perror("sendto");
        exit(-1);
    }
    bytes_recv = recv(m_socket, recv_buf, 256, 0);
    if (bytes_recv == -1) {
        perror("recv");
        exit(-1);
    }
    printf("Recieved server response: %s\n", recv_buf);
    close(m_socket);
    freeaddrinfo(serverinfo);
    return 0; 
}

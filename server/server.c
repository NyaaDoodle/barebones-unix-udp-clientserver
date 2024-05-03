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
    struct sockaddr clientinfo;
    int clientinfo_len = (int)sizeof(clientinfo);
    char send_buf[256];
    char recv_buf[256];
    int bytes_sent = 0;
    int bytes_recv = 0;
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;
    if ((status = getaddrinfo(NULL, "3490", &hints, &serverinfo)) != 0) {
        fprintf(stderr, "getaddrinfo() error: %s\n", gai_strerror(status));
        exit(-1);
    }
    
    m_socket = socket(serverinfo->ai_family, serverinfo->ai_socktype, serverinfo->ai_protocol);
    if (m_socket == -1) {
        perror("socket");
        exit(-1);
    }
    
    if (bind(m_socket, serverinfo->ai_addr, serverinfo->ai_addrlen) == -1) {
        perror("bind");
        exit(-1);
    }

    while (1) {
        bytes_recv = recvfrom(m_socket, recv_buf, 256, 0, &clientinfo, &clientinfo_len);
        if (bytes_recv == -1) {
            perror("recvfrom");
            exit(-1);
        }
        strcpy(send_buf, "I got the message owo)/ It was: ");
        strcat(send_buf, recv_buf);
        strcat(send_buf, "\n");
        bytes_sent = sendto(m_socket, send_buf, strlen(send_buf), 0, &clientinfo, clientinfo_len);
        if (bytes_sent == -1) {
            perror("sendto");
            exit(-1);
        }
    }
    
    close(m_socket);
    freeaddrinfo(serverinfo);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>

#define PORT 8084

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[1024] = {0};

    printf("Client Zarif bermula...\n");
    fflush(stdout);

    while(1) {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        
        
        server = gethostbyname("server_zarif");
        if (server == NULL) {
            printf("Mencari server_zarif...\n");
            fflush(stdout);
            sleep(2);
            continue;
        }

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(PORT);
        bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);

        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            printf("Gagal connect. Cuba lagi...\n");
        } else {
            read(sock, buffer, 1024);
            printf("Respon Server C -> %s\n", buffer);
        }

        fflush(stdout);
        close(sock);
        sleep(5); 
    }
    return 0;
}
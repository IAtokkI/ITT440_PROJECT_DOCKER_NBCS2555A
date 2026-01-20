#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <mysql/mysql.h>
#include <pthread.h>

#define PORT 8082


void *auto_update(void *arg) {
    MYSQL *conn;
    while(1) {
        conn = mysql_init(NULL);
        if (mysql_real_connect(conn, "my_db", "root", "pass123", "project_db", 0, NULL, 0)) {
            
            mysql_query(conn, "UPDATE points_table SET points = points + 1, datetime_stamp = NOW() WHERE user = 'fad'");
            mysql_close(conn);
        }
        sleep(30); 
    }
    return NULL;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[2048] = {0};

    
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, auto_update, NULL);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT); 
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    printf("Server Fad (C) sedia di port %d...\n", PORT);
    fflush(stdout);

    while(1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        MYSQL *conn = mysql_init(NULL);
        if (mysql_real_connect(conn, "my_db", "root", "pass123", "project_db", 0, NULL, 0)) {
            
            mysql_query(conn, "SELECT points, datetime_stamp FROM points_table WHERE user = 'fad'");
            MYSQL_RES *res = mysql_store_result(conn);
            MYSQL_ROW row = mysql_fetch_row(res);

            if(row) {
                sprintf(buffer, "User: fad | Points: %s | Last Update: %s", row[0], row[1]);
            }
            send(new_socket, buffer, strlen(buffer), 0);
            mysql_free_result(res);
        }
        mysql_close(conn);
        close(new_socket);
    }
    return 0;
}
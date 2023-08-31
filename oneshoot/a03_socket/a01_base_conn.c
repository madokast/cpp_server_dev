#include <sys/types.h> // 定义了一些类型
#include <sys/socket.h>
#include <arpa/inet.h> // IP 地址转换
#include <unistd.h> // POSIX 操作系统 API 的访问功能
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define bufsize 1024

int initserver() {
    // 服务端监听 socket。AF_INET 表示 IPv4 套接字类型。SOCK_STREAM 流式传输
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1) {
        printf("[server] error craete listen socket\n");
        return -1;
    }

    // bind 服务地址
    struct sockaddr_in bindaddr;
    bindaddr.sin_family = AF_INET;
    bindaddr.sin_addr.s_addr = htons(INADDR_ANY);
    bindaddr.sin_port = htons(80);
    if (bind(listenfd, (struct sockaddr *)&bindaddr, sizeof(bindaddr)) == -1) {
        printf("[server] error bind listen socket\n");
        goto closefd;
    }

    // 启动监听
    if (listen(listenfd, SOMAXCONN) == -1) {
        printf("[server] error socket listen\n");
        goto closefd;
    }

    return listenfd;
closefd:
    close(listenfd);
    return -1;
}

void closeserver(int listenfd) {
    if (close(listenfd) == -1) {
        printf("[server] error close server\n");
    } else {
        printf("[server] close server\n");
    }
}

void* serveloop(void* listenfd) {
    while(1) {
        struct sockaddr_in clientaddr;
        socklen_t clientaddrlen = sizeof(clientaddr);
        int clientfd = accept(*(int*)(listenfd), (struct sockaddr *)&clientaddr, &clientaddrlen);
        if (clientfd == -1) {
            printf("[server] error accept\n");
            continue;
        }
        char buffer[bufsize] = {0};
        int recvlen = recv(clientfd, buffer, bufsize, 0);
        if (recvlen <= 0) {
            printf("[server] error recv\n");
            goto closeclient;
        }
        printf("[server] receive %s\n", buffer);
        printf("[server] send %s\n", buffer);
        int sendlen = send(clientfd, buffer, strlen(buffer), 0);
        if (sendlen != strlen(buffer)) {
            printf("[server] error send\n");
            goto closeclient;
        }

        closeclient:
        printf("[server] close client\n");
        close(clientfd);
    }
}

void* doaccess(void* _) {
    // 创建 socket
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if (clientfd == -1) {
        printf("[client] error craete socket\n");
        return;
    }

    //  连接服务器
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serveraddr.sin_port = htons(80);
    if (connect(clientfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == -1) {
        printf("[client] error connect server\n");
        return;
    }

    // 发送数据
    char data[] = "hello, world!";
    printf("[client] send %s\n", data);
    int sendlen = send(clientfd, data, strlen(data), 0);
    if (sendlen != strlen(data)) {
        printf("[client] error send data\n");
        return;
    }

    // 接收数据
    char buffer[bufsize] = {0};
    int recvlen = recv(clientfd, buffer, bufsize, 0);
    if (recvlen > 0) {
        printf("[client] receive %s\n", buffer);
    } else {
        printf("[client] error receive data\n");
    }

    close(clientfd);
}

int main(void) {
    int listenfd = initserver();
    if (listenfd == -1) exit(1);

    pthread_t serverid, clientid;
    printf("[system] start server...\n");
    pthread_create(&serverid, NULL, serveloop, &listenfd);
    sleep(1);

    printf("[system] start client...\n");
    pthread_create(&clientid, NULL, doaccess, NULL);
    pthread_join(clientid, NULL);

    closeserver(listenfd);
}

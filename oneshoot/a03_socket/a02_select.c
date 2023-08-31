#include <sys/types.h> // 定义了一些类型
#include <sys/socket.h>
#include <arpa/inet.h> // IP 地址转换
#include <unistd.h> // POSIX 操作系统 API 的访问功能
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <vector>
#define INVALID_FD -1
#define MAX(a, b) (a) > (b) ? (a) : (b);
#define bufsize 1024


int main() {
    // 存储客户 fd
    std::vector<int> clientfds;
    // 最大 fd
    int maxfd;
    // fd 集合
    fd_set readset;
    // selct 计时
    timeval tm;
    // select 返回值
    int selectret;
    // 读 buffer
    char buffer[bufsize];
    // 读长度
    int recvlen;

    // 监听 socket
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == INVALID_FD) {
        printf("[server] error craete listen socket\n");
        return -1;
    }

    // 初始化服务器地址
    struct sockaddr_in bindaddr;
    bindaddr.sin_family = AF_INET;
    bindaddr.sin_addr.s_addr = htonl(INADDR_ANY);
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

    while (1) {
        FD_ZERO(&readset); // 清除监听集合
        FD_SET(listenfd, &readset); // 将 listenfd 加入 readset
        maxfd = listenfd;
        for (int clientfd : clientfds) {
            if (clientfd == INVALID_FD) continue;
            FD_SET(clientfd, &readset);
            maxfd = MAX(maxfd, clientfd);
        }
        tm.tv_sec = 1; // 1 秒等待
        tm.tv_usec = 0;

        selectret = select(maxfd + 1, &readset, NULL, NULL, &tm); // 返回就绪的 fd 数目
        if (selectret == -1) {
            printf("[server] error select %d\n", errno);
            if (errno != EINTR) break; // 不是中断异常，则退出循环
        } else if (selectret == 0) {
            printf("[server] no ready fd\n");
        } else {
            if (FD_ISSET(listenfd, &readset)) { // listen 有新连接
                struct sockaddr_in clientaddr;
                socklen_t clientaddrlen = sizeof(clientaddr);
                int clientfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientaddrlen);
                if (clientfd == INVALID_FD) {
                    printf("[server] error accept\n");
                } else {
                    printf("[server] accept fd %d\n", clientfd);
                    clientfds.push_back(clientfd);
                }
            }
            for (int& clientfd : clientfds) {
                if (clientfd != INVALID_FD && FD_ISSET(clientfd, &readset)) {
                    memset(buffer, 0, bufsize); // clear buffer
                    recvlen = recv(clientfd, buffer, bufsize, 0);
                    if (recvlen <= 0 ) { // 读不到，关闭，将 fd 改为 -1
                        printf("[server] recv fd %d error\n", clientfd);
                        close(clientfd);
                        clientfd = INVALID_FD;
                    } else {
                        printf("[server] recv fd %d %s\n", clientfd, buffer);
                    }
                }
            }

        }


    }

closefd:
    for (int clientfd : clientfds) {
        if (clientfd != INVALID_FD) close(clientfd);
    }
    close(listenfd);
    return -1;
}
/*
 * @Author: OBKoro1
 * @Date: 2022-04-08 16:36:12
 * @LastEditors: OBKoro1
 * @LastEditTime: 2022-04-08 22:37:34
 * @FilePath: /epoll/webserver.cpp
 * @Description: 
 * 
 * Copyright (c) 2022 by 用户/公司名, All Rights Reserved. 
 */

#include "webserver.h"



websever::websever(const int& port_): port(port_)
{
    this->InitSocket();
}

bool websever::InitSocket(){
    int ret;
    struct sockaddr_in addr;
    if(port > 65535 || port < 1024) {
        //std::cout<<"Port number error!"<<std::endl;
        return false;
    }
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);
    // struct linger optLinger = { 0 };
    // if(openLinger) {
    //     /* 优雅关闭: 直到所剩数据发送完毕或超时 */
    //     optLinger.l_onoff = 1;
    //     optLinger.l_linger = 1;
    // }

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_fd < 0) {
        //std::cout<<"Create socket error!"<<std::endl;
        return false;
    }

    // ret = setsockopt(listen_fd, SOL_SOCKET, SO_LINGER, &optLinger, sizeof(optLinger));
    // if(ret < 0) {
    //     close(listen_fd);
    //     //std::cout<<"Init linger error!"<<std::endl;
    //     return false;
    // }

    // int optval = 1;
    // /* 端口复用 */
    // /* 只有最后一个套接字会正常接收数据。 */
    // ret = setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, (const void*)&optval, sizeof(int));
    // if(ret == -1) {
    //     //std::cout<<"set socket setsockopt error !"<<std::endl;
    //     close(listen_fd);
    //     return false;
    // }

    ret = bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr));
    if(ret < 0) {
        //std::cout<<"Bind Port"<<port_<<" error!"<<std::endl;
        close(listen_fd);
        return false;
    }

    ret = listen(listen_fd, 6);
    if(ret < 0) {
        //printf("Listen port:%d error!\n", port_);
        close(listen_fd);
        return false;
    }
    std::cout<<"listen...."<<std::endl;
    // ret = epoll->AddFd(listen_fd, EPOLLIN);
    // if(ret == 0) {
    //     //printf("Add listen error!\n");
    //     close(listen_fd);
    //     return false;
    // }
    // setFdNonblock(listen_fd);
    //printf("Server port:%d\n", port_);
    return true;
}

void websever::start(){
    HandleListen();
    HandleRead(fd_array[0]);
    HandleWrite(fd_array[0]);
}

bool websever::HandleListen(){
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    std::cout<<"hello"<<std::endl;
    int fd = accept(listen_fd, (struct sockaddr *)&addr, &len);
    if(fd<=0){
        std::cout<<"失败"<<std::endl;
    }
    fd_array.push_back(fd);
    // epoll->AddFd(fd, EPOLLIN);
}

bool websever::HandleRead(int Fd){
    char buff[1024];
    int ret =  read(Fd, buff, 1024);
    if(ret==-1){
        std::cout<<"读缓存区为空"<<std::endl;
        return false;
    }
    else{
        std::cout<<buff<<std::endl;
    }
    return true;
}

bool websever::HandleWrite(int Fd){
    char buff[]="I am sever";
    int ret = write(Fd, buff, strlen(buff));
    if(ret==-1){
        std::cout<<"写失败"<<std::endl;
        return false;
    }
    return true;
}

websever::~websever(){
    close(listen_fd);
}


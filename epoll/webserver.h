/*
 * @Author: OBKoro1
 * @Date: 2022-04-08 16:36:21
 * @LastEditors: OBKoro1
 * @LastEditTime: 2022-04-08 22:30:33
 * @FilePath: /epoll/webserver.h
 * @Description: 
 * 
 * Copyright (c) 2022 by 用户/公司名, All Rights Reserved. 
 */
#ifndef __WEBSEVER__H__
#define __WEBSEVER__H__

#include <unordered_map>
#include <fcntl.h>      
#include <unistd.h>     
#include <assert.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include "epoll.hpp"
#include <memory>
#include <cstring>
#include <vector>


class websever{
public:
    websever(const int& port_);
    ~websever();
    bool InitSocket();
    void start();
    bool HandleListen();
    bool HandleRead(int Fd);
    bool HandleWrite(int Fd);



private:
    int listen_fd=-1;
    int port;
    std::vector<int> fd_array;
    std::shared_ptr<myepoll> epoll = std::make_shared<myepoll>();
};

#endif
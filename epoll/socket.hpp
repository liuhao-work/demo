/*
 * @Author: 刘浩
 * @Date: 2022-04-08 16:22:24
 * @LastEditors: OBKoro1
 * @LastEditTime: 2022-04-09 10:53:22
 * @FilePath: /epoll/socket.hpp
 * @Description: 
 * 
 * Copyright (c) 2022 by 杭州电子科技大学, All Rights Reserved. 
 */

#ifndef __SOCKET_H_
#define __SOCKET_H_

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <sys/types.h> 
#include <sys/stat.h>  
#include <fcntl.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <functional>

void error(std::string warn){
    perror(warn.c_str());
    exit(1);
}
int setFdNonblock(int fd) {
    assert(fd > 0);
    return fcntl(fd, F_SETFL, fcntl(fd, F_GETFD, 0) | O_NONBLOCK);
}

class build_socket
{
public:
    build_socket(std::string port_t)
    :port(port_t) {};
    ~build_socket(){};
    build_socket(const build_socket&)=delete;
    build_socket& operator=(const build_socket&)=delete;

    void init(){
        struct sockaddr_in serv_addr;
        lis_socket = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
        if(lis_socket==-1)
        {
            error("socket() error");
        }
        int optval = 1;
        if(setsockopt(lis_socket, SOL_SOCKET,  SO_REUSEADDR, &optval, sizeof(optval)) == -1)
            error("setsocket() eror");
        memset(&serv_addr,0,sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        serv_addr.sin_port = htons(atoi(port.c_str()));
        if(bind(lis_socket,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
        {
            error("bind() error");
        }
        if(listen(lis_socket,5)==-1)
        {
            error("listen() error");
        }
        struct linger optLinger = { 0 };
        if(true) {
        /* 优雅关闭: 直到所剩数据发送完毕或超时 */
            optLinger.l_onoff = 1;
            optLinger.l_linger = 1;
        }
        int ret = setsockopt(lis_socket, SOL_SOCKET, SO_LINGER, &optLinger, sizeof(optLinger));
        if(ret < 0) {
            close(lis_socket);
            //std::cout<<"Init linger error!"<<std::endl;
        }
        // setFdNonblock(lis_socket);
        std::cout<<"begin listen information form the port "<<port<<" ...."<<std::endl;
    }
    void create_thread(){
        init();
        while(1)
        {
            int clnt_sock = wait_accept();
            execute_transfer(clnt_sock);
        }
    }
    int wait_accept(){
        int clnt_sock;
        socklen_t clnt_addr_size;
        struct sockaddr_in clnt_addr;
        clnt_addr_size = sizeof(clnt_addr);
        clnt_sock = accept(lis_socket,(struct sockaddr*)&clnt_addr,&clnt_addr_size);

        if(clnt_sock==-1)
        {
            error("accept() error");
        }
        // setFdNonblock(clnt_sock);
        std::cout<<"connect success"<<std::endl;
        return clnt_sock;
    }
    void execute_transfer(int clnt_sock){
        char mess[1024]={0};
        if(read(clnt_sock,mess,sizeof(mess))<=0)
        {
            error("read() error");
        }
        std::cout<<mess<<std::endl;
        // sleep(100);
        // send_client(clnt_sock);
        std::string message = "this is a message from server!";
        if(write(clnt_sock,message.c_str(),message.length())<=0)
            error("write() error");
        
        std::cout<<"send success"<<std::endl;
        close(clnt_sock);
    }

private:
    std::string port;
    int lis_socket;
};




#endif

    


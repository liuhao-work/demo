/*
 * @Author: 刘浩
 * @Date: 2022-04-08 10:16:58
 * @LastEditTime: 2022-04-08 22:26:19
 * @LastEditors: OBKoro1
 * @Description: io复用的epoll类
 * @FilePath: /epoll/epoll.hpp
 */

#ifndef __EPOLL__HPP__
#define __EPOLL__HPP__

#include <sys/epoll.h>
#include <iostream>
#include <unistd.h>


class myepoll{
public:
    
    myepoll(size_t fd_size = 512){
        bool is_create = CreateEpollInstance(fd_size);
    }

    /**
     * @description: 创建epoll一个新的实例对象。
     * @param {size_t} fd_size: epoll可以添加的socket的文件描述符的个数，默认512。
     * @return {bool} 成功返回true，失败返回false。
     */
    bool CreateEpollInstance(size_t fd_size = 512){
        if(epoll_fd!=-1) return true;
        epoll_fd = epoll_create(fd_size);
        return epoll_fd!=-1;
    }

    /**
     * @description: 向epoll实例中添加socket文件描述符。
     * @param {int} &Fd: 需要添加的socket文件描述符
     * @param {uint32_t} &events: epoll实例需要检测的对象事件;
     *          {
     *              EPOLLIN: 读事件，检测读缓存区， 
     *              EPOLLOUT: 写事件，检测写缓存区，
     *              EPOLLERR: 检测异常事件,
     *          }
     * @return {bool} 添加成功则返回true，失败返回false。
     */
    bool AddFd(int &Fd, const uint32_t &events){
        struct epoll_event ev;
        ev.events = events;
        ev.data.fd = Fd;
        return 0==epoll_ctl(epoll_fd, EPOLL_CTL_ADD, Fd, &ev);
    }

    /**
     * @description: 向epoll实例中删除socket文件描述符。
     * @param {int} Fd: 需要删除的socket文件描述符。
     * @return {bool} 删除成功返回true，失败返回false。
     */
    bool DelFd(int Fd){
        return 0==epoll_ctl(epoll_fd, EPOLL_CTL_DEL, Fd, NULL);
    }

    /**
     * @description: 析构函数调用close()关闭epoll的文件描述符。
     */
    ~myepoll(){
        close(epoll_fd);
    }
private:
    int epoll_fd=-1;

};
#endif
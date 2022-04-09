/*
 * @Author: OBKoro1
 * @Date: 2022-04-07 16:28:53
 * @LastEditors: OBKoro1
 * @LastEditTime: 2022-04-09 09:22:21
 * @FilePath: /epoll/main.cpp
 * @Description: 
 * 
 * Copyright (c) 2022 by 用户/公司名, All Rights Reserved. 
 */
#include <iostream>
#include "webserver.h"
#include "socket.hpp"
#include <cstring>

const static std::string PORT = "8080";

int main(){
    build_socket instance(PORT);
    instance.create_thread();
}
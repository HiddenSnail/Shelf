#pragma once

#include <queue>
#include <string>
#include <functional>
#include <iostream>
#include "connection_pool.h"

namespace Shelf {

class Connection {
public:
    Connection() {
        std::cout << "Connection::Create!" << std::endl;
    }
    void print() {
        std::cout << "Connection::Op!" << std::endl;
    }
    ~Connection() {
        std::cout << "Connection::Delete!" << std::endl;
    }
};

}


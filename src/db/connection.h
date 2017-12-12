#pragma once

#include <queue>
#include <string>
#include <functional>
#include <iostream>
#include "connection_pool.h"

namespace Shelf {

class Connection {
protected:
    std::string _uri;
public:
    Connection(std::string uri = "") : _uri(uri) {
        std::cout << "Connection::Create!" << std::endl;
    }

    std::string getUri() { return _uri; }

    void print() {
        std::cout << "Connection::Op!" << std::endl;
    }

    virtual ~Connection() {
        std::cout << "Connection::Delete!" << std::endl;
    }
};

}


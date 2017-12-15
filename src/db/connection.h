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
protected:
    Connection(std::string uri = "") : _uri(uri) {}
public:
    std::string getUri() { return _uri; }
    virtual ~Connection() {}
};

}


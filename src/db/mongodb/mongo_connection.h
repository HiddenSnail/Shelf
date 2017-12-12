#pragma once

#include "../connection.h"
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

namespace Shelf {

class MongoConnection : public Connection, public mongocxx::client {
private:
    static mongocxx::instance _instance;
public:
     MongoConnection(std::string uri);
    ~MongoConnection();
};
}
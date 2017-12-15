#pragma once

#include "../connection.h"
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

namespace Shelf {

class MongoConnection : public Connection, public mongocxx::client {
private:
    static mongocxx::instance _instance;
public:
     MongoConnection(std::string uri);
    ~MongoConnection();
};
}
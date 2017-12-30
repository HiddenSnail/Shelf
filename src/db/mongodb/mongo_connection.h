#pragma once

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

#include "db/connection.h"

namespace Shelf {

    class MongoConnection : public Connection, public mongocxx::client {
    private:
        static mongocxx::instance _instance;
    public:
         MongoConnection(std::string uri);
        ~MongoConnection();
    };
}
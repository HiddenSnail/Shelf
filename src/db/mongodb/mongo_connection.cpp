
#include "db/mongodb/mongo_connection.h"

namespace Shelf {

    mongocxx::instance MongoConnection::_instance{};

    MongoConnection::MongoConnection(std::string uri)
        :Connection(uri), mongocxx::client(mongocxx::uri(uri))
    {
        std::cout << "Create a mongo connetion!" << std::endl;
    }

    MongoConnection::~MongoConnection() {
        std::cout << "Delete a mongo connetion!" << std::endl;
    }

}
#include "./db/connection.h"
#include "./db/mongodb/mongo_connection.h"
#include "./db/connection_pool.h"
#include "./objs/user.h"
#include "./access/mongodb/access.h"
#include "./access/access_proxy.h"
#include <iostream>

using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;

using namespace Shelf;
using namespace Shelf::Mongodb;

int main() {
    std::cout << "hello world" << std::endl;
    ConnectionPool<MongoConnection> *pool = ConnectionPool<MongoConnection>::getInstance("mongodb://localhost:27017");
    AccessProxy<UserAccess> user_proxy(pool->getConnectionHolder());
//    std::cout <<  << std::endl;
    user_proxy.get("5a2ba1312f0d5b697b727632");
    return 0;
}
#include <iostream>

#include "libs/crow_all.h"

#include "db/connection.h"
#include "db/mongodb/mongo_connection.h"
#include "db/connection_pool.h"
#include "objs/user.h"
#include "access/mongodb/all_access.h"

using namespace Shelf;
using namespace Shelf::Mongodb;
using namespace crow;

int main() {
    std::cout << "hello world" << std::endl;
//    SimpleApp app;
//
//    CROW_ROUTE(app, "/")([](){
//       return "Hello Corw";
//    });
//
//    app.port(18080).multithreaded().run();
    ConnectionPool<MongoConnection> *pool = ConnectionPool<MongoConnection>::getInstance("mongodb://localhost:27017");
    UserAccess userAccess(pool->getConnectionHolder());
    userAccess.getByEmail("773829@124.com").print();
    return 0;
}
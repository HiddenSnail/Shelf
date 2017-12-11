#include "./db/connection.h"
#include <iostream>

using namespace Shelf;

int main() {
    // {
        std::cout << "hello world" << std::endl;
        ConnectionPool<Connection> *pool = ConnectionPool<Connection>::getInstance();
        std::cout << pool->count() << std::endl;
        {
            ConnectionPool<Connection>::ConnectionHandler p = pool->getConnectionHandler();
            std::cout << "Busy:" << pool->countBusyConnections() << std::endl;
            std::cout << "Free:" << pool->countFreeConnections() << std::endl;
            std::cout << p->getConnection() << std::endl;
         }
        std::cout << "Busy:" << pool->countBusyConnections() << std::endl;
        std::cout << "Free:" << pool->countFreeConnections() << std::endl;
    // }
    return 0;
}
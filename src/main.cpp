#include "./db/connection.h"
#include "./db/mongodb/mongo_connection.h"
#include "./db/connection_pool.h"
#include <iostream>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;

using namespace Shelf;

int main() {
    {
        std::cout << "hello world" << std::endl;
        ConnectionPool<MongoConnection> *pool = ConnectionPool<MongoConnection>::getInstance("mongodb://localhost:27017");
        std::cout << pool->count() << std::endl;
        {
            auto p = pool->getConnectionHolder();
            std::cout << "Busy:" << pool->countBusyConnections() << std::endl;
            std::cout << "Free:" << pool->countFreeConnections() << std::endl;
            
            auto t = p->getConnection();
            
            auto collection = (*t)["colorbook"]["test_col"];

            std::vector<bsoncxx::document::value> docs;
            for (int i = 0; i < 5; i++) {
                bsoncxx::builder::stream::document doc{};
                docs.push_back(doc << "index" << i 
                << "value" << std::string("value") + std::to_string(i)
                << finalize);
            }
            collection.insert_many(docs);

            collection.update_many(
                document{} << "index" << open_document
                << "$lte" << 2
                << close_document << finalize,
                document{} << "$set" << open_document
                << "index" << 4
                << close_document << finalize
            );
            
            mongocxx::cursor cursor = collection.find(
                document{} << "index" << open_document
                << "$gt" << 2
                << "$lte" << 5
                << close_document
                << finalize);
            for (auto data: cursor) {
                std::cout << bsoncxx::to_json(data) << std::endl;
            }

            bsoncxx::stdx::optional<mongocxx::result::delete_result> result = collection.delete_many(document{} << finalize);
            if (result) {
                std::cout << "delete " << result->deleted_count() << " documents" << std::endl;
            }
        }
        std::cout << "Busy:" << pool->countBusyConnections() << std::endl;
        std::cout << "Free:" << pool->countFreeConnections() << std::endl;
    }
    return 0;
}
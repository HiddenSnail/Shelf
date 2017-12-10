#include <iostream>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <vector>
#include <string>

using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;

int main(int, char**) {
    mongocxx::instance inst{};
    mongocxx::client conn{mongocxx::uri{"mongodb://localhost:27017"}};
    auto collection = conn["colorbook"]["test_col"];

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
    return 0;
}
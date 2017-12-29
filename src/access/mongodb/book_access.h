#pragma once

#include "../../objs/book.h"
#include "../../db/mongodb/mongo_connection.h"
#include "../../db/connection_pool.h"
#include "../access.h"

namespace Shelf {
namespace Mongodb {

    using bsoncxx::builder::stream::document;
    using bsoncxx::builder::stream::open_array;
    using bsoncxx::builder::stream::open_document;
    using bsoncxx::builder::stream::close_array;
    using bsoncxx::builder::stream::close_document;
    using bsoncxx::builder::stream::finalize;

    class BookAccess: public Access<MongoConnection> {
    public:
        typedef Book DataType;
    protected:
        mongocxx::collection _collection;
    public:
        BookAccess(Access::ConnectionHolder holder = nullptr);
        ~BookAccess();

        bool updateBaseInfo(const DataType &);
        bool updatePictures(const std::string id, const std::vector<Picture> pictures, UpdateOption option = Replace);
        bool updatePictures(const DataType &book, UpdateOption option = Replace);

        bool insert(DataType &book);
        bool insert(std::vector<DataType> &bookVec);

        void erase(const std::string id);
        void eraseByBookName(const std::string name);

        DataType get(const std::string id);
        DataType getByBookName(const std::string name);

    private:
        bsoncxx::document::value convertToDbValue(const DataType &book);
        DataType convertFromDbValue(const bsoncxx::document::value &value);

    };
}
}
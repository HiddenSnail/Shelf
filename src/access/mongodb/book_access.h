#pragma once

#include <vector>
#include "objs/book.h"
#include "db/connection_pool.h"
#include "db/mongodb/mongo_connection.h"
#include "access/access.h"

namespace Shelf {
namespace Mongodb {

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
        void erase(const std::vector<std::string> ids);
        void eraseByBookName(const std::string name);

        DataType get(const std::string id);
        DataType getByBookName(const std::string name);

    private:
        bsoncxx::document::value convertToDbValue(const DataType &book);
        DataType convertFromDbValue(const bsoncxx::document::value &value);

    };
}
}
#pragma once

#include "../../objs/user.h"
#include "../../db/mongodb/mongo_connection.h"
#include "../../db/connection_pool.h"

namespace Shelf {
namespace Mongodb {

    using bsoncxx::builder::stream::open_array;
    using bsoncxx::builder::stream::open_document;
    using bsoncxx::builder::stream::close_array;
    using bsoncxx::builder::stream::close_document;
    using bsoncxx::builder::stream::finalize;

    class UserAccess {
    public:
        typedef User DataType;
        typedef ConnectionPool<MongoConnection>::ConnectionHolder ConnectionHolder;
    protected:
        ConnectionHolder _holder;
        mongocxx::collection _colletion;
        UserAccess () { }
        UserAccess(UserAccess::ConnectionHolder holder = nullptr);
    public:
        ~UserAccess();
        void setConnectionHolder(UserAccess::ConnectionHolder holder);
        UserAccess::ConnectionHolder getConnectionHolder() const;

        bool isAccountExist(std::string email);

        /**
         *  @note: should implement functions below, so that this class can be agented by AccessProxy.
         *
         */
        void save(const DataType &data);
        void update(const DataType &data);
        void erase(const DataType &data);
        DataType get(std::string id);
        std::vector<DataType> get(const DataType &filter);
    };
}
}
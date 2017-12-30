#pragma once

#include "objs/user.h"
#include "db/mongodb/mongo_connection.h"
#include "db/connection_pool.h"
#include "access/access.h"

namespace Shelf {
namespace Mongodb {

    class UserAccess : public Access<MongoConnection> {
    public:
        typedef User DataType;
    protected:
        mongocxx::collection _collection;
    public:
        UserAccess(ConnectionHolder holder = nullptr);
        ~UserAccess();

        /**
         * The base info include: _username, _email, _password
         * @param user
         * @return
         */
        bool updateBaseInfo(const UserAccess::DataType &user);
        bool updateFriendIds(const std::string id, const std::vector<std::string> friendIds, UpdateOption option = Replace);
        bool updateFriendIds(const UserAccess::DataType &user, UpdateOption option = Replace);
        bool updateBookIds(const std::string id, const std::vector<std::string> bookIds, UpdateOption option = Replace);
        bool updateBookIds(const UserAccess::DataType &user, UpdateOption option = Replace);

        bool insert(UserAccess::DataType &user);
        bool insert(std::vector<UserAccess::DataType> &userVec);

        void erase(const std::string id);

        UserAccess::DataType get(const std::string id);
        UserAccess::DataType getByEmail(const std::string email);
    private:
        bsoncxx::document::value convertToDbValue(const UserAccess::DataType &user);
        UserAccess::DataType convertFromDbValue(const bsoncxx::document::value &value);
    };
}
}
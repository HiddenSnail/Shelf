#include "user_access.h"

namespace Shelf {
namespace Mongodb {
    UserAccess::UserAccess(ConnectionHolder holder) : _holder(holder) {
        auto connection = holder->getConnection();
        _colletion = (*connection)["colorbook"]["user"];
    }

    UserAccess::~UserAccess() { _holder = nullptr; }

    void UserAccess::setConnectionHolder(UserAccess::ConnectionHolder holder) { _holder = holder; }

    UserAccess::ConnectionHolder UserAccess::getConnectionHolder() const { return _holder; }

    bool UserAccess::isAccountExist(std::string email) {
        bsoncxx::builder::stream::document doc{};
        auto result = _colletion.find_one(std::move(doc << "email" << email << finalize));
        return result ? true : false;
    }

    void UserAccess::save(const UserAccess::DataType &data) {
        bsoncxx::builder::stream::document doc{};
        bsoncxx::document::value val = doc
                << "username" << data._username
                << "email" << data._email
                << "password" << data._password
                << finalize;
        _colletion.insert_one(std::move(val));
    }

    void UserAccess::update(const UserAccess::DataType &data) {

    }

    void UserAccess::erase(const UserAccess::DataType &data) {

    }

    UserAccess::DataType UserAccess::get(const std::string id) {
        User user;
        bsoncxx::builder::stream::document doc{};
        auto result = _colletion.find_one(std::move(doc << "_id" << bsoncxx::oid(id) << finalize));
        if (result) {
            std::cout << bsoncxx::to_json(result->view()) << std::endl;
        }
        return user;
    }

    std::vector<UserAccess::DataType> UserAccess::get(const UserAccess::DataType &filter) {
        std::vector<UserAccess::DataType> vec;
        User user1;
        user1._email = "58934932";
        vec.push_back(user1);
        return vec;
    }
}
}
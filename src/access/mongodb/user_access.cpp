#include "user_access.h"

namespace Shelf {
namespace Mongodb {
    UserAccess::UserAccess(Access::ConnectionHolder holder): Access(holder)
    {
        auto connection = holder->getConnection();
        _collection = (*connection)["colorbook"]["user"];
    }

    UserAccess::~UserAccess()
    {
        // do nothing
    }

    bool UserAccess::updateBaseInfo(const UserAccess::DataType &user)
    {
        if (user._id.length() != 24 || get(user._id)._id.empty()) {
            return false;
        }

        auto result = this->_collection.update_one(
                document{} << "_id" << bsoncxx::oid(user._id) << finalize,
                document{} << "$set" << open_document
                           << "username" << user._username
                           << "email" << user._email
                           << "password" << user._password
                           << close_document << finalize
        );

        if (result) {
            return true;
        } else {
            return false;
        }
    }

    bool UserAccess::updateFriendIds(const std::string id, const std::vector<std::string> friendIds, UpdateOption option)
    {
        User user;
        user._id = id;
        user._friendIds = friendIds;
        return updateBookIds(user, option);
    }

    bool UserAccess::updateFriendIds(const UserAccess::DataType &user, UpdateOption option)
    {
        if (user._id.length() != 24 || get(user._id)._id.empty()) {
            return false;
        }
        document doc{};
        bsoncxx::stdx::optional<mongocxx::result::update> result;
        std::string opStr;
        switch (option) {
            case Add: {
                opStr = "$addToSet";
                auto friendIdArray = doc << opStr << open_document << "friend_ids" << open_document
                                         << "$each" << open_array;
                for (int i = 0; i < user._friendIds.size(); i++) {
                    friendIdArray << user._friendIds[i];
                }
                auto updateValue = friendIdArray << close_array << close_document << close_document << finalize;
                result = this->_collection.update_one(
                        document{} << "_id" << bsoncxx::oid(user._id) << finalize,
                        std::move(updateValue)
                );
                break;
            }
            case Delete: {
                opStr = "$pullAll";
                auto friendIdArray = doc << opStr << open_document << "friend_ids" << open_array;
                for (int i = 0; i < user._friendIds.size(); i++) {
                    friendIdArray << user._friendIds[i];
                }
                auto updateValue = friendIdArray << close_array << close_document << finalize;
                result = this->_collection.update_one(
                        document{} << "_id" << bsoncxx::oid(user._id) << finalize,
                        std::move(updateValue)
                );
                break;
            }
            default: {
                opStr = "$set";
                auto friendIdArray = doc << opStr << open_document << "friend_ids" << open_array;
                for (int i = 0; i < user._friendIds.size(); i++) {
                    friendIdArray << user._friendIds[i];
                }
                auto updateValue = friendIdArray << close_array << close_document << finalize;
                result = this->_collection.update_one(
                        document{} << "_id" << bsoncxx::oid(user._id) << finalize,
                        std::move(updateValue)
                );
            }

        }

        if (result) {
            return true;
        } else {
            return false;
        }
    }

    bool UserAccess::updateBookIds(const std::string id, const std::vector<std::string> bookIds, UpdateOption option)
    {
        User user;
        user._id = id;
        user._bookIds = bookIds;
        return updateBookIds(user, option);
    }

    bool UserAccess::updateBookIds(const UserAccess::DataType &user, UpdateOption option)
    {
        if (user._id.length() != 24 || get(user._id)._id.empty()) {
            return false;
        }
        document doc{};
        bsoncxx::stdx::optional<mongocxx::result::update> result;
        std::string opStr;
        switch (option) {
            case Add: {
                opStr = "$addToSet";
                auto bookIdArray = doc << opStr << open_document << "book_ids" << open_document
                                       << "$each" << open_array;
                for (int i = 0; i < user._bookIds.size(); i++) {
                    bookIdArray << user._bookIds[i];
                }
                auto updateValue = bookIdArray << close_array << close_document << close_document << finalize;
                result = this->_collection.update_one(
                        document{} << "_id" << bsoncxx::oid(user._id) << finalize,
                        std::move(updateValue)
                );
                break;
            }
            case Delete: {
                opStr = "$pullAll";
                auto bookIdArray = doc << "$set" << open_document << "book_ids" << open_array;
                for (int i = 0; i < user._bookIds.size(); i++) {
                    bookIdArray << user._bookIds[i];
                }
                auto updateValue = bookIdArray << close_array << close_document << finalize;
                result = this->_collection.update_one(
                        document{} << "_id" << bsoncxx::oid(user._id) << finalize,
                        std::move(updateValue)
                );
                break;
            }
            default: {
                opStr = "$set";
                auto bookIdArray = doc << "$set" << open_document << "book_ids" << open_array;
                for (int i = 0; i < user._bookIds.size(); i++) {
                    bookIdArray << user._bookIds[i];
                }
                auto updateValue = bookIdArray << close_array << close_document << finalize;
                result = this->_collection.update_one(
                        document{} << "_id" << bsoncxx::oid(user._id) << finalize,
                        std::move(updateValue)
                );
            }

        }

        if (result) {
            return true;
        } else {
            return false;
        }
    }

    bool UserAccess::insert(UserAccess::DataType &user)
    {
        bsoncxx::document::value val = this->convertToDbValue(user);
        bsoncxx::stdx::optional<mongocxx::result::insert_one> result = this->_collection.insert_one(std::move(val));
        user._id = "";
        if (result) {
            user._id = result->inserted_id().get_oid().value.to_string();
            return true;
        } else {
            return false;
        }
    }

    bool UserAccess::insert(std::vector<UserAccess::DataType> &userVec)
    {
        std::vector<bsoncxx::document::value> valueVec;
        for (const auto &user: userVec) {
            valueVec.push_back(this->convertToDbValue(user));
        }
        bsoncxx::stdx::optional<mongocxx::result::insert_many> results = _collection.insert_many(valueVec);
        for (auto &user : userVec) {
            user._id = "";
        }
        if (results) {
            auto ids = results->inserted_ids();
            for (int i = 0; i < userVec.size(); i++) {
                userVec[i]._id = ids[i].get_oid().value.to_string();
            }
            return true;
        } else {
            return false;
        }

    }

    void UserAccess::erase(const std::string id)
    {
        if (id.length() != 24) {
            return;
        }
        this->_collection.delete_one(document{} << "_id" << bsoncxx::oid(id) << finalize);
    }

    UserAccess::DataType UserAccess::get(const std::string id)
    {
        User user;
        if (id.length() == 24) {
            bsoncxx::builder::stream::document doc{};
            auto result = _collection.find_one(std::move(doc << "_id" << bsoncxx::oid(id) << finalize));
            if (result) {
                user = this->convertFromDbValue(*result);
            }
        }
        return user;
    }

    UserAccess::DataType UserAccess::getByEmail(const std::string email)
    {
        User user;
        auto result = this->_collection.find_one(document{} << "email" << email << finalize);
        if (result) {
            user = this->convertFromDbValue(*result);
        }
        return user;
    }

    bsoncxx::document::value UserAccess::convertToDbValue(const DataType &user)
    {
        bsoncxx::builder::stream::document doc{};
        doc << "username" << user._username
         << "email" << user._email
         << "password" << user._password;

        auto bookIdArray = doc << "book_ids" << open_array;
        for (int i = 0; i < user._bookIds.size(); i++) {
            bookIdArray << user._bookIds[i];
        }

        auto friendIdArray = bookIdArray << close_array << "friend_ids" << open_array;
        for (int i = 0; i < user._friendIds.size(); i++) {
            friendIdArray << user._friendIds[i];
        }

        bsoncxx::document::value value = friendIdArray << close_array << finalize;
        return value;
    }

    UserAccess::DataType UserAccess::convertFromDbValue(const bsoncxx::document::value &value)
    {
        User user;
        auto view = value.view();
        user._id = view["_id"].get_oid().value.to_string();
        user._username = view["username"].get_utf8().value.to_string();
        user._password = view["password"].get_utf8().value.to_string();
        user._email = view["email"].get_utf8().value.to_string();
        for (auto &&ele: view["book_ids"].get_array().value) {
            user._bookIds.push_back(ele.get_utf8().value.to_string());
        }
        for (auto &&ele: view["friend_ids"].get_array().value) {
            user._friendIds.push_back(ele.get_utf8().value.to_string());
        }
        return user;
    }


}
}
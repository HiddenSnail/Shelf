#include "access/mongodb/book_access.h"

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

namespace Shelf {
namespace Mongodb{

    using bsoncxx::builder::stream::document;
    using bsoncxx::builder::stream::open_array;
    using bsoncxx::builder::stream::open_document;
    using bsoncxx::builder::stream::close_array;
    using bsoncxx::builder::stream::close_document;
    using bsoncxx::builder::stream::finalize;

    BookAccess::BookAccess(ConnectionHolder holder): Access(holder)
    {
        auto connection = holder->getConnection();
        _collection = (*connection)["colorbook"]["book"];

    }

    BookAccess::~BookAccess()
    {
        //do nothing
    }

    bool BookAccess::updateBaseInfo(const DataType &book)
    {
        if (book._id.length() != 24 || get(book._id)._id.empty()) {
            return false;
        }

        auto updateValue = this->convertToDbValue(book);

        auto result = this->_collection.update_one(
                document{} << "_id" << bsoncxx::oid(book._id) << finalize,
                document{} << "$set" << open_document
                           << "name" << book._name
                           << "tag" << book._tag
                           << close_document << finalize
        );

        if (result) {
            return true;
        } else {
            return false;
        }
    }

    bool BookAccess::updatePictures(const std::string id, const std::vector<Picture> pictures, UpdateOption option)
    {
        Book book;
        book._id = id;
        book._pictures = pictures;
        return updatePictures(book, option);
    }

    bool BookAccess::updatePictures(const DataType &book, UpdateOption option)
    {
        if (book._id.length() != 24 || get(book._id)._id.empty()) {
            return false;
        }
        document doc{};
        bsoncxx::stdx::optional<mongocxx::result::update> result;
        std::string opStr;
        switch (option) {
            case Add: {
                opStr = "$addToSet";
                auto pictureArray = doc << opStr << open_document << "pictures" << open_document
                                        << "$each" << open_array;
                for (int i = 0; i < book._pictures.size(); i++) {
                    pictureArray << open_document
                                 << "title" << book._pictures[i]._title
                                 << "url" << book._pictures[i]._url
                                 << close_document;
                }

                auto updateValue = pictureArray << close_array << close_document << close_document << finalize;
                result = this->_collection.update_one(
                        document{} << "_id" << bsoncxx::oid(book._id) << finalize,
                        std::move(updateValue)
                );
                break;
            }
            case Delete: {
                opStr = "$pullAll";
                auto pictureArray = doc << opStr << open_document << "pictures" << open_array;
                for (int i = 0; i < book._pictures.size(); i++) {
                    pictureArray << open_document
                                 << "title" << book._pictures[i]._title
                                 << "url" << book._pictures[i]._url
                                 << close_document;
                }
                auto updateValue = pictureArray << close_array << close_document << finalize;
                result = this->_collection.update_one(
                        document{} << "_id" << bsoncxx::oid(book._id) << finalize,
                        std::move(updateValue)
                );
                break;
            }
            default: {
                opStr = "$set";
                auto pictureArray = doc << opStr << open_document << "pictures" << open_array;
                for (int i = 0; i < book._pictures.size(); i++) {
                    pictureArray << open_document
                                 << "title" << book._pictures[i]._title
                                 << "url" << book._pictures[i]._url
                                 << close_document;
                }
                auto updateValue = pictureArray << close_array << close_document << finalize;
                result = this->_collection.update_one(
                        document{} << "_id" << bsoncxx::oid(book._id) << finalize,
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

    bool BookAccess::insert(DataType &book)
    {
        bsoncxx::document::value val = this->convertToDbValue(book);
        auto result = this->_collection.insert_one(std::move(val));
        if (result) {
            book._id = result->inserted_id().get_oid().value.to_string();
            return true;
        } else {
            return false;
        }
    }

    bool BookAccess::insert(std::vector<DataType> &bookVec)
    {
        std::vector<bsoncxx::document::value> valueVec;
        for (const auto &book: bookVec) {
            valueVec.push_back(this->convertToDbValue(book));
        }
        bsoncxx::stdx::optional<mongocxx::result::insert_many> results = _collection.insert_many(valueVec);
        for (auto &book : bookVec) {
            book._id = "";
        }
        // TODO: can't make sure all insert success
        if (results) {
            auto ids = results->inserted_ids();
            for (int i = 0; i < bookVec.size(); i++) {
                bookVec[i]._id = ids[i].get_oid().value.to_string();
            }
            return true;
        } else {
            return false;
        }
    }

    void BookAccess::erase(const std::string id)
    {
        if (id.length() != 24) {
            return;
        }
        this->_collection.delete_one(document{} << "_id" << bsoncxx::oid(id) << finalize);
    }

    void BookAccess::erase(const std::vector<std::string> ids)
    {
        for (auto &ele: ids)
        {
            erase(ele);
        }
    }

    void BookAccess::eraseByBookName(const std::string name)
    {
        this->_collection.delete_one(document{} << "name" << name << finalize);
    }


    BookAccess::DataType BookAccess::get(const std::string id)
    {
        Book book;
        if (id.length() == 24) {
            bsoncxx::builder::stream::document doc{};
            auto result = _collection.find_one(std::move(doc << "_id" << bsoncxx::oid(id) << finalize));
            if (result) {
                book = this->convertFromDbValue(*result);
            }
        }
        return book;
    }

    BookAccess::DataType BookAccess::getByBookName(const std::string name)
    {
        Book book;
        bsoncxx::builder::stream::document doc{};
        auto result = _collection.find_one(std::move(doc << "name" << name << finalize));
        if (result) {
            book = this->convertFromDbValue(*result);
        }
        return book;
    }

    bsoncxx::document::value BookAccess::convertToDbValue(const DataType &book)
    {
        document doc{};
        doc << "name" << book._name
        << "tag" << book._tag;

        auto pictureArray = doc << "pictures" << open_array;
        for (int i = 0; i < book._pictures.size(); i++) {
            pictureArray << open_document
                         << "title" << book._pictures[i]._title
                         << "url" << book._pictures[i]._url
                         << close_document;
        }

        bsoncxx::document::value value = pictureArray << close_array << finalize;
        return value;
    }

    BookAccess::DataType BookAccess::convertFromDbValue(const bsoncxx::document::value &value)
    {
        Book book;
        auto view = value.view();
        book._id = view["_id"].get_oid().value.to_string();
        book._name = view["name"].get_utf8().value.to_string();
        book._tag = view["tag"].get_utf8().value.to_string();

        for (auto &&ele: view["pictures"].get_array().value) {
            Picture picture;
            picture._title = ele["title"].get_utf8().value.to_string();
            picture._url = ele["url"].get_utf8().value.to_string();
            book._pictures.push_back(picture);
        }

        return book;
    }
}
}
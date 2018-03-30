#include <vector>

#include "service/service.h"
#include "access/mongodb/all_access.h"

#include "../libs/crow_all.h"
#include "service.h"
#include "../objs/user.h"
#include "../objs/book.h"
#include "../access/mongodb/book_access.h"
#include "../access/access.h"

namespace Shelf {

    crow::response Service::registerAccount(const crow::request& req)
    {
        auto body = crow::json::load(req.body);
        crow::response res;
        if (body) {
            User newUser;
            newUser._username = body["username"].s();
            newUser._password = body["password"].s();
            newUser._email = body["email"].s();
            if (newUser.isCanSave()) {
                Mongodb::UserAccess userAccess(_pool->getConnectionHolder());
                // TODO: should use md5 convert password
                if (userAccess.getByEmail(newUser._email)._id.empty()) {
                    if(userAccess.insert(newUser)) {
                        res.code = 200;
                    } else {
                        res.code = 500;
                        res.body = "Database error";
                    }
                } else {
                    res.code = 400;
                    res.body = "The email already be registed";
                }
            }
        } else {
            res.code = 400;
            res.body = "Body format error";
        }
        return res;
    }

    crow::response Service::login(const crow::request& req)
    {
        auto body = crow::json::load(req.body);
        crow::response res;
        if (body) {
            User  loginUser;
            loginUser._email = body["email"].s();
            loginUser._password = body["password"].s();
            Mongodb::UserAccess userAccess(_pool->getConnectionHolder());
            User realUser = userAccess.getByEmail(loginUser._email);
            if (realUser._id.empty()) {
                res.code = 400;
                res.body = "The account not exsit";
            } else {
                // TODO: should use md5 convert password
                if (loginUser._password == realUser._password) {
                    res.code = 200;
                    // TODO: the session is static
                    res.json_value["session"] = realUser._id;
                    res.json_value["username"] = realUser._username;
                    res.json_value["email"] = realUser._email;
                } else {
                    res.code = 400;
                    res.body = "Password error";
                }
            }
        }
        return res;
    }

    crow::response Service::uploadBook(const crow::request& req)
    {
        auto body = crow::json::load(req.body);
        crow::response res;
        std::string session = req.get_header_value("session");
        if (session.empty()) return crow::response{400, "The header no session"};
        if (body) {
            // TODO
            std::string userId = session;
            Mongodb::UserAccess userAccess(_pool->getConnectionHolder());
            if (userAccess.get(userId)._id.empty()) {
                res.code = 400;
                res.body = "Not login";
            } else {
                Mongodb::BookAccess bookAccess(_pool->getConnectionHolder());
                std::vector<Book> bookArray;
                for (int i = 0; i < body["books"].size(); i++) {
                    Book book;
                    book._name = body["books"][i]["name"].s();
                    book._tag = body["books"][i]["tag"].s();

                    for (int j = 0; j < body["books"][i]["pictures"].size(); j++) {
                        Picture picture;
                        picture._title = body["books"][i]["pictures"][j]["title"].s();
                        std::string base64 = body["books"][i]["pictures"][j]["base64"].s();
                        // TODO: use qiniu get url
                        std::string url = "http://sinastorage.com/storage.zone.photo.sina.com.cn/zone/1000_0/20171102/dc585690e55569e7f0335665b84ebba0_1000_562.jpg?&ssig=qZTqCoqjzz&KID=sina,slidenews&Expires=1514673034";
                        picture._url = url;
                        book._pictures.push_back(picture);
                    }
                    bookArray.push_back(book);
                }
                if (bookAccess.insert(bookArray)) {
                    std::vector<std::string> bookIds;
                    for (int k = 0; k < bookArray.size(); k++) {
                        bookIds.push_back(bookArray[k]._id);
                    }
                    userAccess.updateBookIds(userId, bookIds, UpdateOption::Add);
                    res.code = 200;
                } else {
                    res.code = 500;
                    res.body = "Database error";
                }
            }
        } else {
            res.code = 400;
            res.body = "Json format error";
        }
        return res;
    }

    crow::response Service::deleteBook(const crow::request& req)
    {
        auto body = crow::json::load(req.body);
        std::string session = req.get_header_value("session");
        if (session.empty()) return crow::response{400, "The header no session"};
        crow::response res;
        if (body) {
            // TODO
            std::string userId = session;
            Mongodb::UserAccess userAccess(_pool->getConnectionHolder());
            if (userAccess.get(userId)._id.empty()) {
                res.code = 400;
                res.body = "Not login";
            } else {
                Mongodb::BookAccess bookAccess(_pool->getConnectionHolder());
                std::vector<std::string> bookIds;
                for (int i = 0; i < body["bookIds"].size(); i++) {
                    bookIds.push_back(body["bookIds"][i].s());
                }
                userAccess.updateBookIds(userId, bookIds, UpdateOption::Delete);
                bookAccess.erase(bookIds);
                res.code = 200;
            }
        } else {
            res.code = 400;
            res.body = "Json format error";
        }
        return res;
    }

    crow::response Service::downloadBook(const crow::request& req)
    {
        auto session = req.get_header_value("session");
        crow::response res;
        if (!session.empty()) {
            // TODO
            std::string userId = session;
            Mongodb::UserAccess userAccess(_pool->getConnectionHolder());
            if (userAccess.get(userId)._id.empty()) {
                res.code = 400;
                res.body = "Not login";
            } else {
                Mongodb::BookAccess bookAccess(_pool->getConnectionHolder());
                User user = userAccess.get(userId);
                crow::json::wvalue data;
                for (int i = 0; i < user._bookIds.size(); i++) {
                    Book book = bookAccess.get(user._bookIds[i]);
                    data["books"][i]["id"] = book._id;
                    data["books"][i]["name"] = book._name;
                    data["books"][i]["tag"] = book._tag;
                    for (int j = 0; j < book._pictures.size(); j++) {
                        data["books"][i]["pictures"][j]["title"] = book._pictures[j]._title;
                        data["books"][i]["pictures"][j]["url"] = book._pictures[j]._url;
                    }
                }
                res.body = crow::json::dump(data);
                res.code = 200;
            }
        } else {
            res.code = 400;
            res.body = "The header no session";
        }
        return res;
    }

    // 响应测试
    crow::response Service::testResponse(const crow::request& req)
    {
        crow::response res;
        res.body = "Test success!";
        res.code = 200;
        return res;
    }

    void Service::exec(const unsigned int port)
    {
        static Service service;
        CROW_ROUTE(service._app, "/user/register").methods("POST"_method)
                ([](const crow::request &req){
                    auto res = service.registerAccount(req);
                    return res;
                });

        CROW_ROUTE(service._app, "/user/login").methods("POST"_method)
                ([](const crow::request &req){
                    auto res = service.login(req);
                    return res;
                });

        CROW_ROUTE(service._app, "/user/book").methods("GET"_method, "PUT"_method, "DELETE"_method)
                ([](const crow::request &req){
                    crow::response res;
                    switch (req.method) {
                        case crow::HTTPMethod::GET:
                            res = service.downloadBook(req);
                            break;
                        case crow::HTTPMethod::PUT:
                            res = service.uploadBook(req);
                            break;
                        case crow::HTTPMethod::DELETE:
                            res = service.deleteBook(req);
                            break;
                        default:
                            break;
                    };
                    return res;
                });

        CROW_ROUTE(service._app, "/test").methods("GET"_method)([](const crow::request &req) {
            return service.testResponse(req);
        });

        service._app.port(port)
                .multithreaded()
                .run();
    }
}

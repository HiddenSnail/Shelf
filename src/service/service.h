#pragma once

#include <set>
#include <string>

#include "libs/crow_all.h"
#include "db/connection_pool.h"
#include "db/mongodb/mongo_connection.h"

namespace Shelf {
    class Service {
    private:
        crow::App<> _app;
        ConnectionPool<MongoConnection> *_pool;
        std::set<std::string> _sessionSet;

        Service() {
            _pool = ConnectionPool<MongoConnection>::getInstance("mongodb://localhost:27017");
        }
    protected:
        // 注册账户
        crow::response registerAccount(const crow::request& req);
        // 登录
        crow::response login(const crow::request& req);
        // 上传画册
        crow::response uploadBook(const crow::request& req);
        // 删除画册
        crow::response deleteBook(const crow::request& req);
        // 下载画册
        crow::response downloadBook(const crow::request& req);
        // 响应测试
        crow::response testResponse(const crow::request& req);
    public:
        static void exec(const unsigned int port);
    };
}

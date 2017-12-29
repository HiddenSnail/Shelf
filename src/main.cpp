#include "./db/connection.h"
#include "./db/mongodb/mongo_connection.h"
#include "./db/connection_pool.h"
#include "./objs/user.h"
#include "./access/mongodb/all_access.h"
#include "./access/access_proxy.h"
#include <iostream>

using namespace Shelf;
using namespace Shelf::Mongodb;

int main() {
    std::cout << "hello world" << std::endl;
    ConnectionPool<MongoConnection> *pool = ConnectionPool<MongoConnection>::getInstance("mongodb://localhost:27017");
    UserAccess userAccess(pool->getConnectionHolder());
    BookAccess bookAccess(pool->getConnectionHolder());
    Book book;
    book._name = "Antusheng";
    book._tag = "story";
    Picture pic;
    pic._title = "myfirst picture";
    pic._url = "http://www.baidu.com";

    Picture p1;
    p1._title = "p1", p1._url = "url1";
    Picture p2;
    p2._title = "p2", p2._url = "url2";
    std::vector<Picture> pics = { p1, p2 };
    std::vector<Book> books = { book, book };
    bookAccess.eraseByBookName("Antusheng");
    return 0;
}
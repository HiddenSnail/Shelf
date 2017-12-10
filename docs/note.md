+ server: root@139.196.94.117
+ compile: c++ --std=c++11 test.cpp $(pkg-config --cflags --libs libmongocxx) -Wl,-rpath,/usr/local/lib
+ MongDB BSON结构设计如下  
    ```
    user
    {
        _id: #ObjectID
        account: #string,
        password: #string,
        username: #string,
        books_id: [#ObjectID, ...],
        friends_id: [#ObjectID, ...]
    }

    book
    {
        _id: #ObjecctID,
        tag: #string,
        name: #string,
        pictures: [{
            title: #string,
            content: #binData
        }, ...]
    }

    ```
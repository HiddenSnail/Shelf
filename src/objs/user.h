#pragma once

#include "object.h"
#include <string>
#include <vector>
#include <iostream>

namespace Shelf {
class User : public Object {
public:
    std::string _username;
    std::string _email;
    std::string _password;
    std::vector<std::string> _bookIds;
    std::vector<std::string> _friendIds;

public:
    User(): Object(), _username(""), _email(""), _password("") {
        _bookIds.clear();
        _friendIds.clear();
    }

    virtual std::string type() { return "User"; }

    bool isCanSave() {
        return !_username.empty() && !_email.empty() && !_password.empty();
    }

    bool isEmpty() {
        return _id.empty() && _username.empty() && _email.empty() && _password.empty()
               && _bookIds.empty() && _friendIds.empty();
    }

    void print() {
        std::cout << "id: " << _id << std::endl;
        std::cout << "username: " << _username << std::endl;
        std::cout << "password: " << _password << std::endl;
        std::cout << "email: " << _email << std::endl;
        std::cout << "book_ids: [";
        if (!_bookIds.empty()) {
            for (int i = 0; i < _bookIds.size() - 1; i++) {
                std::cout << _bookIds[i] << ",";
            }
            std::cout << _bookIds[_bookIds.size()-1];
        }
        std::cout << "]" << std::endl;
        std::cout << "friend_ids: [";
        if (!_friendIds.empty()) {
            for (int i = 0; i < _friendIds.size() - 1; i++) {
                std::cout << _friendIds[i] << ",";
            }
            std::cout << _friendIds[_friendIds.size()-1];
        }
        std::cout << "]" << std::endl;
    }
};
}
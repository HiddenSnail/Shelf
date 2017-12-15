#pragma once

#include "object.h"
#include <string>
#include <vector>

namespace Shelf {
class User : public Object {
public:
    std::string _username;
    std::string _email;
    std::string _password;
    std::vector<std::string> _booksId;
    std::vector<std::string> _friendsId;
public:
    bool isCanSave() {
        return !_username.empty() && !_email.empty() && !_password.empty();
    }
    bool isEmpty() {
        return _id.empty() && _username.empty() && _email.empty() && _password.empty()
                && _booksId.empty() && _friendsId.empty();
    }
};
}
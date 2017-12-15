#pragma once

#include "object.h"

namespace Shelf {
class Book : public Object {
public:
    std::string _tag;
    std::string _name;
    Book() {}
    ~Book() {}
};
}
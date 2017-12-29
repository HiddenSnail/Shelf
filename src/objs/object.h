#pragma once

#include <string>

namespace Shelf {
class Object {
public:
    std::string _id;
    virtual std::string type() = 0;

public:
    Object(): _id("") {}
};
}
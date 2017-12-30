#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "object.h"

namespace Shelf {

    struct Picture {
        std::string _title;
        std::string _url;
        Picture(): _title(""), _url("") {}

    };

    class Book : public Object {
    public:
        std::string _tag;
        std::string _name;
        std::vector<Picture> _pictures;
    public:
        Book(): Object(), _tag(""), _name("") {
            _pictures.clear();
        }

        virtual std::string type() { return "Book"; }

        void print() {
            std::cout << "id: " << _id << std::endl;
            std::cout << "tag: " << _tag << std::endl;
            std::cout << "name: " << _name << std::endl;
            std::cout << "pictures: [" << std::endl;
            for (auto ele: _pictures) {
                std::cout << "  { title: " << ele._title << ", url: "
                          << ele._url << " }" << std::endl;
            }
            std::cout << "]" << std::endl;
        }
    };
}
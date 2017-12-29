#pragma once 

#include <vector>

namespace Shelf {

template<class T>
class AccessProxy : public T {
public:
    AccessProxy(): T() {}
    
    AccessProxy(typename T::ConnectionHolder holder) : T(holder) {}

    // update data by id
    bool update(typename T::DataType &data) {
        return T::update(data);
    }

    bool insert(typename T::DataType &data) {
        return T::insert(data);
    }

    bool insert(std::vector<typename T::DataType> &dataVec) {
        return T::insert(dataVec);
    }
    
    typename T::DataType get(const std::string id) {
        return T::get(id);
    }


};

}
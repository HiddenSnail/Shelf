#pragma once 

#include <vector>

namespace Shelf {

template<class T>
class AccessProxy : public T {
public:
    AccessProxy(): T() {}
    
    AccessProxy(typename T::ConnectionHolder holder) : T(holder) {}
    
    void save(typename T::DataType &data) {
        T::save(data);
    }
    
    void update(typename T::DataType &data) {
        T::update(data);
    }
    
    void erase(typename T::DataType &data) {
        T::erase(data);
    }
    
    typename T::DataType get(std::string id) {
        return T::get(id);
    }
    
    std::vector<typename T::DataType> get(typename T::DataType &filter) {
        return T::get(filter);
    }

};

}
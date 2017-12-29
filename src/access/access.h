#ifndef SRC_ACCESS_H_H
#define SRC_ACCESS_H_H

namespace Shelf {

    enum UpdateOption {
        Replace = 0,
        Add,
        Delete
    };

    template <class ConnectionType>
    class Access {
    public:
        typedef typename ConnectionPool<ConnectionType>::ConnectionHolder ConnectionHolder;
    protected:
        ConnectionHolder _holder;
    protected:
        Access(Access::ConnectionHolder holder = nullptr): _holder(holder) {}
    public:
        virtual ~Access() { this->_holder = nullptr; }
        void setConnectionHolder(ConnectionHolder holder) { this->_holder = holder; }
        ConnectionHolder getConnectionHolder() const { return this->_holder; }
    };

}


#endif //SRC_ACCESS_H_H

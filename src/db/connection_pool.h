#pragma once 

#include <string>
#include <memory>
#include <vector>
#include <mutex>

namespace Shelf {

template <class ContType>
class ConnectionPool {
private:
    class Holder;
public:
    typedef std::shared_ptr<Holder> ConnectionHolder;
private:
    static const int MAX_CONTS = 100;
    static const int MIN_CONTS = 10;
    std::vector<ContType*> _conts;
    int _busyHead; // first busy connetion index;
    std::string _uri;
    std::mutex _mutex;

private:
    class Holder : public std::enable_shared_from_this<Holder> {
    private:
        ContType *_cont;
        ConnectionPool<ContType> *_pool;
    public:
        Holder(ContType *cont, ConnectionPool<ContType> *pool) : _cont(cont), _pool(pool) {}
        ContType* getConnection() {
            return _cont;
        }

        // 闲置的链接会放置在数组开头，一直碰到_busyHead才是被使用的连接
        ~Holder() {
            std::lock_guard<std::mutex> guard(_pool->_mutex);
            for (int i = _pool->_busyHead; i < _pool->_conts.size(); i++) {
                if (_pool->_conts[i] == _cont) {
                    // 将即将被回收的连接与第一个(_busyHead)的连接调换位置，然后将_busyHead往后移动
                    _pool->_conts[i] = _pool->_conts[_pool->_busyHead];
                    _pool->_conts[_pool->_busyHead] = _cont;
                    _pool->_busyHead++;
                    break; 
                }
            }
        }
    };

private:
    ConnectionPool(std::string uri): _busyHead(MIN_CONTS), _uri(uri) {
        for (int i = 0; i < MIN_CONTS; i++) {
            _conts.emplace_back(new ContType(uri));
        }
    }

    void extendPool() {
        const int extendNum = 10; 
        if (_conts.size() < MAX_CONTS) {
            int realNum = 0;
            if (_conts.size() + extendNum <= MAX_CONTS) {
                realNum = extendNum;
            } else {
                realNum = MAX_CONTS - _conts.size();
            }
            _conts.insert(_conts.begin(), realNum, new ContType(_uri));
            _busyHead += realNum;
        }
    }
public:
    static ConnectionPool* getInstance(std::string uri) {
        static ConnectionPool *p = new ConnectionPool(uri);
        return p;
    }

    ConnectionHolder getConnectionHolder() {
        std::lock_guard<std::mutex> guard(_mutex);
        ConnectionHolder handler = nullptr;
        if (_busyHead == 0) {
            extendPool();
        }
        if (_busyHead != 0) {
            int freeContIndex = _busyHead - 1;
            handler = ConnectionHolder(new Holder(_conts[freeContIndex], this));
            _busyHead--;
        }
        return handler;
    }

    ~ConnectionPool() {
        for (auto ele : _conts) {
            delete ele;
        }
    }
    
    const int count() {
        std::lock_guard<std::mutex> guard(_mutex);
        return _conts.size();
    }

    const int countFreeConnections() {
        std::lock_guard<std::mutex> guard(_mutex);
        return _busyHead;
    }

    const int countBusyConnections() {
        std::lock_guard<std::mutex> guard(_mutex);
        return _conts.size() - _busyHead;
    }

    const int getMaxSize() {
        std::lock_guard<std::mutex> guard(_mutex);
        return MAX_CONTS;
    }

    const int getMinSize() {
        std::lock_guard<std::mutex> guard(_mutex);
        return MIN_CONTS;
    }
};
}
+ server: root@139.196.94.117
+ compile: c++ --std=c++11 test.cpp $(pkg-config --cflags --libs libmongocxx) -Wl,-rpath,/usr/local/lib
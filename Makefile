PKG_NAME = code
PKG_SOURCE = ./docs ./include ./src Makefile

SERVER_USR = root
SERVER_IP = 139.196.94.117

CXX = g++
CXX_HEAD_FLAGS = --std=c++11
CXX_END_FLAGS = `pkg-config --cflags --libs libmongocxx` -Wl,-rpath,/usr/local/lib

shelf : ./src/testdb.cpp
	$(CXX) $(CXX_HEAD_FLAGS) -o shelf.out ./src/testdb.cpp $(CXX_END_FLAGS)

.PHONY : run
run : 
	./shelf.out

.PHONY : pkg
pkg :
	-rm $(PKG_NAME).tar
	tar -cvf $(PKG_NAME).tar $(PKG_SOURCE)

.PHONY : dpkg
dpkg:
	tar -xf $(PKG_NAME).tar

.PHONY : push
push :
	scp ./$(PKG_NAME).tar $(SERVER_USR)@$(SERVER_IP):/$(SERVER_USR)/workspace/Shelf
	scp ./Makefile $(SERVER_USR)@$(SERVER_IP):/$(SERVER_USR)/workspace/Shelf
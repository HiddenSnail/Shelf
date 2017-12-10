PKG_NAME = code
PKG_SOURCE = ./docs ./include ./src Makefile

SERVER_USR = root
SERVER_IP = 139.196.94.117
SERVER_PROJECT_DIR = workspace/Shelf

CXX = g++
CXX_HEAD_FLAGS = --std=c++11
CXX_END_FLAGS = `pkg-config --cflags --libs libmongocxx` -Wl,-rpath,/usr/local/lib

shelf : ./src/testdb.cpp
	@$(CXX) $(CXX_HEAD_FLAGS) -o shelf.out ./src/testdb.cpp $(CXX_END_FLAGS)




.PHONY : pkg
pkg :
	@-rm $(PKG_NAME).tar
	@tar -cf $(PKG_NAME).tar $(PKG_SOURCE)

.PHONY : dpkg
dpkg:
	@tar -xf $(PKG_NAME).tar

.PHONY : push
push :
	@scp ./$(PKG_NAME).tar $(SERVER_USR)@$(SERVER_IP):/$(SERVER_USR)/$(SERVER_PROJECT_DIR)
	@scp ./Makefile $(SERVER_USR)@$(SERVER_IP):/$(SERVER_USR)/$(SERVER_PROJECT_DIR)

.PHONY : compile
compile :
	@echo compiling source code ...
	@make pkg
	@make push
	@ssh $(SERVER_USR)@$(SERVER_IP) "cd $(SERVER_PROJECT_DIR) && make dpkg && make"
	@echo compiling finish!

.PHONY : run
run : 
	@ssh $(SERVER_USR)@$(SERVER_IP) "cd $(SERVER_PROJECT_DIR) && ./shelf.out"
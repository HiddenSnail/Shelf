PKG_NAME = code
PKG_SOURCE = ./docs ./src Makefile

SERVER_USR = root
SERVER_IP = 139.196.94.117
SERVER_PROJECT_DIR = workspace/Shelf

CXX = g++
CXX_HEAD_FLAGS = --std=c++11
CXX_END_FLAGS = `pkg-config --cflags --libs libmongocxx` -Wl,-rpath,/usr/local/lib

OBJECT_S = connection.o main.o

shelf : $(OBJECT_S)
	$(CXX) -o shelf.out $(OBJECT_S)

main.o : ./src/main.cpp
	$(CXX) $(CXX_HEAD_FLAGS) -c -o main.o ./src/main.cpp $(CXX_END_FLAGS)

connection.o : ./src/db/connection.h ./src/db/connection.cpp ./src/db/connection_pool.h
	$(CXX) $(CXX_HEAD_FLAGS) -c -o connection.o ./src/db/connection.cpp $(CXX_END_FLAGS)

# connection_pool.o : ./src/db/connection_pool.h
# 	$(CXX) $(CXX_HEAD_FLAGS) -c -o connection_pool.o ./src/db/connection_pool.h $(CXX_END_FLAGS)

# debug.o : ./src/utils/debug.h
# 	$(CXX) $(CXX_HEAD_FLAGS) -c -o debug.o ./src/utils/debug.h $(CXX_END_FLAGS)

.PHONY : pkg
pkg :
	@-rm $(PKG_NAME).tar
	@tar -cf $(PKG_NAME).tar $(PKG_SOURCE)

.PHONY : dpkg
dpkg :
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

.PHONY : clean
clean : 
	@ssh $(SERVER_USR)@$(SERVER_IP) "cd $(SERVER_PROJECT_DIR) && rm *.o"
	@echo clean finish!
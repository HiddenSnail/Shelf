PKG_NAME = code
PKG_SOURCE = ./docs ./src Makefile

SERVER_USR = root
SERVER_IP = 139.196.94.117
SERVER_PROJECT_DIR = workspace/Shelf

CXX = g++
CXX_INC = -I./src
CXX_HEAD_FLAGS = --std=c++11 $(CXX_INC)
CXX_END_FLAGS = `pkg-config --cflags --libs libmongocxx` -lboost_system -lpthread -Wl,-rpath,/usr/local/lib

TARGET = shelf.out
SRC = $(shell find ./src -path "*debug" -prune -o -name "*.cpp" -print)
OBJ = $(patsubst %.cpp, %.o, $(SRC)) 

$(TARGET) : $(OBJ)
	$(CXX) $(CXX_HEAD_FLAGS) -o $@ $^ $(CXX_END_FLAGS)

.cpp.o:
	$(CXX) $(CXX_HEAD_FLAGS) -c -o $@ $< $(CXX_END_FLAGS)

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
	ssh $(SERVER_USR)@$(SERVER_IP) "cd $(SERVER_PROJECT_DIR) && make dpkg && make"
	@echo compiling finish!

.PHONY : run
run : 
	@ssh $(SERVER_USR)@$(SERVER_IP) "cd $(SERVER_PROJECT_DIR) && ./$(TARGET)"

.PHONY : clean
clean : 
	@ssh $(SERVER_USR)@$(SERVER_IP) "cd $(SERVER_PROJECT_DIR) && rm $(OBJ)"
	@echo clean finish!

CXX = g++
CC = gcc
CXXFLAGS = -std=c++11  -Wall -Wextra -pthread
CCFLAGS  = -g

OPENSSL_DIR = 
ZLIB_DIR = 
OPENSSL_SUPPORT = -DCPPHTTPLIB_OPENSSL_SUPPORT -I$(OPENSSL_DIR)/include -L$(OPENSSL_DIR)/lib -lssl -lcrypto
ZLIB_SUPPORT = -DCPPHTTPLIB_ZLIB_SUPPORT -I${ZLIB_DIR}/include -L${ZLIB_DIR}/lib -lz
INCLUDE_DIR = -I. -Iinclude -Iinclude/sqlite3
LD_LIBRARY = -Llib -lsqlite3

all: hello

hello : hello.o Makefile
	$(CXX) -o $@ hello.o $(CXXFLAGS) $(OPENSSL_SUPPORT) $(ZLIB_SUPPORT) ${INCLUDE_DIR}

%.o : %.cpp
    $(CXX) -c $(CXXFLAGS) $<

%.o : %.c
    $(CC) -c $(CCFLAGS) $<

clean:
	rm server client hello simplesvr upload redirect *.pem

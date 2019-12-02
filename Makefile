
#CXX = clang++
CXXFLAGS = -std=c++11  -Wall -Wextra -Wmissing-field-initializers -pthread
OPENSSL_DIR = .
ZLIB_DIR = .
OPENSSL_SUPPORT = -DCPPHTTPLIB_OPENSSL_SUPPORT -I$(OPENSSL_DIR)/include -L$(OPENSSL_DIR)/lib -lssl -lcrypto
ZLIB_SUPPORT = -DCPPHTTPLIB_ZLIB_SUPPORT -I${ZLIB_DIR}/include -L${ZLIB_DIR}/lib -lz
INCLUDE_DIR = -I. -Iinclude

all: hello

hello : hello.cc Makefile
	$(CXX) -o hello $(CXXFLAGS) hello.cc $(OPENSSL_SUPPORT) $(ZLIB_SUPPORT) ${INCLUDE_DIR}

clean:
	rm server client hello simplesvr upload redirect *.pem

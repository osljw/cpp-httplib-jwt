//
//  hello.cc
//
//  Copyright (c) 2019 Yuji Hirose. All rights reserved.
//  MIT License
//
#include <iostream>

#include "httplib.h"
#include "jwt-cpp/jwt.h"
#include "sql.h"
#include "SQLiteCpp/SQLiteCpp.h"
#include "server_handler.h"

using namespace httplib;
using namespace sql;

int main(void) {
  std::cout << "SQlite3 version " << SQLite::VERSION << " (" << SQLite::getLibVersion() << ")" << std::endl;
  ServerHandler svr;
  svr.Start();

  return 0;
}

#ifndef SERVER_HANDLER_H
#define SERVER_HANDLER_H

#include "httplib.h"
#include "jwt-cpp/jwt.h"
#include "sql.h"
#include "SQLiteCpp/SQLiteCpp.h"

using namespace httplib;
using namespace sql;

class ServerHandler {
  public:
    ServerHandler();

    void Start();
    void Home(const Request& req, Response& res);
    void Register(const Request& req, Response& res);
    void Login(const Request& req, Response& res);
    void Show(const Request& req, Response& res);

  private:
    Server svr; 
    std::unique_ptr<SQLite::Database> db = nullptr;
};

#endif

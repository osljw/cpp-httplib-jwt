#include "server_handler.h"
#include "absl/strings/str_split.h"

using namespace std::placeholders;
ServerHandler::ServerHandler() {
    try {
      db = std::unique_ptr<SQLite::Database>(new SQLite::Database("test.db", SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE));
      std::cout << "SQLite database file '" << db->getFilename() << "' opened successfully\n";

      // Create a new table with an explicit "id" column aliasing the underlying rowid
      //db->exec("DROP TABLE IF EXISTS user");
      db->exec("CREATE TABLE if not exists user (name TEXT PRIMARY KEY, passwd TEXT)");

      // first row
      int nb = db->exec("INSERT or replace INTO user VALUES (\"test\", \"test\")");
      std::cout << "INSERT OR REPLACE INTO user VALUES (\"test\", \"test\")\", returned " << nb << std::endl;

      // Check the results : expect two row of result
      SQLite::Statement query(*db, "SELECT * FROM user");
      std::cout << "SELECT * FROM user :" << std::endl;
      while (query.executeStep())
      {
          std::cout << "row (" << query.getColumn(0) << ", " << query.getColumn(1) << ")" << std::endl;
      }
    } catch (std::exception& e) {
      std::cout << "SQLite exception: " << e.what() << std::endl;
    }
    
    svr.Get("/", std::bind(&ServerHandler::Home, this, _1, _2));
    svr.Get("/register", std::bind(&ServerHandler::Register, this, _1, _2));
    svr.Get("/login", std::bind(&ServerHandler::Login, this, _1, _2));
    svr.Get("/show", std::bind(&ServerHandler::Show, this, _1, _2));
    //svr.Get("/login", [&](const Request & req, Response &res) { this->Login(req, res); } );
}

void ServerHandler::Start() {
    svr.listen("10.130.47.187", 1234);
}

void ServerHandler::Home(const Request& req, Response& res) {
    Show(req, res);
}

void ServerHandler::Register(const Request& req, Response& res) {
    std::cout << "====== register =====" << std::endl;
    if(req.has_param("name") && req.has_param("passwd")) {
        std::string name = req.get_param_value("name");
        std::string passwd = req.get_param_value("passwd");

        sql::InsertModel i;
        i.insert("name", name)
                ("passwd", passwd)
            .replace(true)
            .into("user");
        std::cout << i.str() << std::endl;
        int nb = db->exec(i.str());
        std::cout << "nb=:" << nb << std::endl;
        //try {
        //    int nb = db->exec(i.str());
        //} catch (std::exception& e) {
        //    std::cout << "SQLite exception: " << e.what() << std::endl;
        //}
        res.set_content(i.str(), "text/plain");
        return;
    }

    res.set_content("register failed", "text/plain");
}

void ServerHandler::Login(const Request& req, Response& res) {
    std::cout << "====== login =====" << std::endl;
    if(req.has_param("name") && req.has_param("passwd")) {
        std::string name = req.get_param_value("name");
        std::string passwd = req.get_param_value("passwd");
        std::string content = std::string("login success") + std::string("name:") + name +
            "passwd:" + passwd;

        bool valid_user = false;
        sql::SelectModel s;
        s.select("name", "passwd")
            .from("user")
            .where((column("name") == name) and (column("passwd") == passwd));
        std::cout << s.str() << std::endl;
        try {
            SQLite::Statement query(*db, s.str());
            while (query.executeStep()) {
                valid_user = true;
                std::cout << "row (" << query.getColumn(0) << ", " << query.getColumn(1) << ")" << std::endl;
                
            }
        } catch (std::exception& e) {
            std::cout << "SQLite exception: " << e.what() << std::endl;
        }
        if(!valid_user) {
            res.set_content("login failed!", "text/plain");
            std::cout << "login failed" << std::endl;
            return;
        }

        // creat token
        auto token = jwt::create()
            .set_issuer("auth0")
            .set_type("JWS")
            .set_issued_at(std::chrono::system_clock::now())
            .set_expires_at(std::chrono::system_clock::now() + std::chrono::seconds{3600})
            .set_payload_claim("name", jwt::claim(name))
            .sign(jwt::algorithm::hs256{"secret_only_you_know"});

        // decode token for print
        auto decoded = jwt::decode(token);
        for(auto& e : decoded.get_payload_claims())
            std::cout << e.first << " = " << e.second.to_json() << std::endl;

        res.set_header("Set-Cookie", std::string("name=") + token);
        //res.set_header("Authorization", std::string("Bearer ") + token);
        res.set_content(content.c_str(), "text/plain");
        std::cout << "login success" << "name:" << name
            << "passwd:" << passwd << std::endl;
    } else
        res.set_content("login failed!", "text/plain");

}

void ServerHandler::Show(const Request& req, Response& res) {
    std::cout << "====== show =====" << std::endl;
    //if (req.has_header("Authorization")) {
    //    auto token = req.get_header_value("Authorization");
    if (req.has_header("Cookie")) {
        auto token = req.get_header_value("Cookie");
        std::cout << "token:" << token << std::endl;
        // decode token for print
        std::vector<std::string> v = absl::StrSplit(token, '=');
        std::cout << "token split:" << v[1] << std::endl;
        try {
            auto decoded = jwt::decode(v[1]);
            for(auto& e : decoded.get_payload_claims())
                std::cout << e.first << " = " << e.second.to_json() << std::endl;
        } catch (std::exception ex) {
            std::cout << "exception:" << ex.what() << std::endl;
        }
        res.set_content(token, "text/plain");
    }
}

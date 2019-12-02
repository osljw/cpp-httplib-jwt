//
//  hello.cc
//
//  Copyright (c) 2019 Yuji Hirose. All rights reserved.
//  MIT License
//
#include <iostream>

#include "httplib.h"
#include "jwt-cpp/jwt.h"

using namespace httplib;

int main(void) {
  Server svr;

  svr.Get("/login", [](const Request & req, Response &res) {

    std::cout << "login" << std::endl;
    if(req.has_param("name") && req.has_param("passwd")) {
        std::string name = req.get_param_value("name");
        std::string passwd = req.get_param_value("passwd");
        std::string content = std::string("login success") + std::string("name:") + name +
            "passwd:" + passwd;

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
  });

  svr.Get("/show", [](const Request & req, Response &res) {
    //if (req.has_header("Authorization")) {
    //    auto token = req.get_header_value("Authorization");
    if (req.has_header("Cookie")) {
        auto token = req.get_header_value("Cookie");
        std::cout << "token:" << token << std::endl;
        // decode token for print
        //auto decoded = jwt::decode(token);
        //for(auto& e : decoded.get_payload_claims())
        //    std::cout << e.first << " = " << e.second.to_json() << std::endl;
        res.set_content(token, "text/plain");
    }
  });

  svr.listen("10.130.47.187", 1234);
}

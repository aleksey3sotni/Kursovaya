#pragma once
#include <string>
#include <map>

class Logger;

class Communicate
{
public:
    int connection(int port, std::map<std::string, std::string> database, Logger* l1);
    static std::string md5(std::string input_str); 
    static std::string generate_salt(); 
};

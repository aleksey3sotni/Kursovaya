#pragma once
#include <map>
#include <string>
#include "Errors.h"

class Connector
{
private:
    std::map<std::string, std::string> data_base;
public:
    int connect(std::string base_file = "/home/stud/kyrsach/test/auth.txt");
    std::map<std::string, std::string> get_data();
};

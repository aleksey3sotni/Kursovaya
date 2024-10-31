#include "Connector.h"
#include "Errors.h"

#include <fstream>
#include <stdexcept>
#include <string>

int Connector::connect(std::string base_file)
{
    if(base_file.find('.') == std::string::npos) {
        throw crit_err("invalid_base_path");
    }

    std::ifstream file_read(base_file);
    if (!file_read.is_open()) {
        throw crit_err("invalid_base_path");
    }

    std::string line;
    while (getline(file_read, line)) {
        std::string temp_login;
        std::string temp_pass;
        bool flag = true;

        for (char s : line) {
            if (s == ':') {
                flag = false;
                continue;
            }
            if (flag) {
                temp_login.push_back(s);
            } else {
                temp_pass.push_back(s);
            }
        }

        data_base[temp_login] = temp_pass;
    }

    if (data_base.empty()) {
        throw crit_err("invalid_base_path");
    }

    return 0;
}

std::map<std::string, std::string> Connector::get_data()
{
    return data_base;
}

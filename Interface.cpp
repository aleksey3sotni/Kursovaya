#include "Interface.h"
#include "Logger.h"
#include "Communicate.h"
#include "Connector.h"
#include "Calculator.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

// ... (другие include) ...

// Функция для разбиения строки по разделителю
std::vector<std::string> split(const std::string& str, char delim) {
    std::vector<std::string> parts;
    std::stringstream ss(str);
    std::string part;
    while (std::getline(ss, part, delim)) {
        parts.push_back(part);
    }
    return parts;
}

int Interface::comm_proc(int argc, const char* argv) {
    Logger log("server.log"); 
    log.writelog("Server started.");

    Connector connector; 
    try {
        connector.connect("/home/stud/kyrsach/test/auth.txt"); 
    } catch (const crit_err& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        log.writelog("Error: " + std::string(e.what()));
        return 1;
    }

    auto database = connector.get_data();

    Communicate comm;
    int port = 8080; 
    try {
        comm.connection(port, database, &log);
    } catch (const crit_err& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        log.writelog("Error: " + std::string(e.what()));
        return 1;
    }

    return 0;
}

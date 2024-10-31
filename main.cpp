#include <iostream>
#include "Connector.h"
#include "Interface.h"
#include "Communicate.h"
#include "Calculator.h"
#include "Errors.h"
#include "Logger.h"

int main(int argc,const char** argv)
{
    Interface UI;
    UI.comm_proc(argc,argv);
    return 0;
}

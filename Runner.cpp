//
// Created by 神奇bug在哪里 on 2023/6/5.
//

#include "Runner.h"
#include "errors.h"
#include <cassert>
namespace Comments
{
     DeviceData* deviceData = nullptr;
     UserInterface* userInterface = nullptr;
}
using namespace Comments;
Runner::Runner() {
    Comments::deviceData = new DeviceData();
    Comments::userInterface = new UserInterface();
    assert(deviceData != nullptr);
    assert(userInterface != nullptr);
}

void Runner::run() {
    assert(userInterface != nullptr);
    try {
        Comments::deviceData->readFile();
    }
    catch (FileError & e){
        //only file IO error could be caught here. If there is any other error, it will be thrown to main().
        std::cout<<e.what()<<std::endl;
    }
    //UI started here.
    while (true) {
        try {
#if defined(__APPLE__) || defined(__linux__)
            system("clear");
#elif defined(_WIN32)
            system("cls");
#endif
            userInterface->picker();
        }catch (ParamError &e) {
            // there must be a param error
            std::cout << e.what() << std::endl;
#ifdef _WIN32
            system("pause");
#else
            system("read -n 1 -p \"Press any key to continue...\"");
#endif
        }
        catch (std::exception &e) {
            // there must be any hard error which not be handled
            this->errorCode = errno;
            std::cout << e.what() << std::endl;
            printError();
#ifdef _WIN32
            system("pause");
#else
            system("read -n 1 -p \"Press any key to continue...\"");
#endif
            exit(errorCode);
        }
    }

}

void Runner::printError() const {
    std::cout<<"Error: "<<this->errorCode<<std::endl;
}

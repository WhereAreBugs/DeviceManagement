//
// Created by 神奇bug在哪里 on 2023/6/5.
//

#ifndef DEVICEMANAGEMENT_RUNNER_H
#define DEVICEMANAGEMENT_RUNNER_H
#include "UserInterface.h"
#include "DeviceData.h"
namespace Comments
{
    extern DeviceData* deviceData;
    extern UserInterface* userInterface;
}
//the namespace Comments is used to avoid the global variables.
//the global variables are used to avoid the parameters passing between functions.

class Runner {
private:
    //this function is used to print the error message when an unhandled error occurs.
    void printError() const;
    int errorCode = 0;
public:
    //the constructor is used to initialize the global variables.
    Runner();
    //entry of the program
    void run();
};


#endif //DEVICEMANAGEMENT_RUNNER_H

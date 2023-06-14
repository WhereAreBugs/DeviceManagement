//
// Created by 神奇bug在哪里 on 2023/6/4.
//
#include "Runner.h"
#include "csignal"
/** @brief this function used to proceed the signal
 *  @aim save the data when the program is interrupted
 *  @param sig: the signal
 */
void signalHandler(int sig)
{
    std::cout << "Interrupt signal (" << sig << ") received.\n";
    Comments::deviceData->writeFile();
    exit(sig);
}
/**
 * @brief this function used to register the signal handler
 */
void sigReg()
{
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    signal(SIGABRT, signalHandler);
    signal(SIGSEGV, signalHandler);
    signal(SIGKILL, signalHandler);
    //register signal handler
}


int main()
{
    sigReg();
    //Register of the signal handler
    Runner runner;
    runner.run();
    return 0;
}
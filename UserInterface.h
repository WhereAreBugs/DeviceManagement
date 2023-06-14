//
// Created by 神奇bug在哪里 on 2023/6/5.
//

#ifndef DEVICEMANAGEMENT_USERINTERFACE_H
#define DEVICEMANAGEMENT_USERINTERFACE_H


#include "DeviceData.h"

class UserInterface {
private:
    int choice=0;
    static void print(DeviceData::Device *target);
    static void addDevice();
    void removeDevice();
    void updateDevice();
    void getOneDevice();
    static void printAllDevice();
    static void searchDevice();
    static void filterExpDevice();
    static void sortAndShowDevice();
public:
    UserInterface();
    void picker();

};


#endif //DEVICEMANAGEMENT_USERINTERFACE_H

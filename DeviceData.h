//
// Created by 神奇bug在哪里 on 2023/6/4.
//

#ifndef DEVICEMANAGEMENT_DEVICEDATA_H
#define DEVICEMANAGEMENT_DEVICEDATA_H
#include <iostream>
#include <vector>
#include <ctime>
class DeviceData {
public:
    typedef enum {
        UNUSED,
        ON,
        OFF,
        BROKEN,
        RECYCLED
    } Status;
    typedef union {
        unsigned int fullDate : 21;
        struct {
#ifdef __AARCH64EL__
            unsigned int year : 12;
            unsigned int month : 4;
            unsigned int day : 5;
#else
            unsigned int day : 5;
            unsigned int month : 4;
            unsigned int year : 12;
#endif

        };
    } Dates;
    typedef enum {
        byID = 1,
        byPrice = 3, //due to my mistake, the enum value is not continuous.
        byExpDate = 2,
        byBuyDate = 4
    } SortType;
    typedef struct {
        int id;
        std::string name;
        std::string type;
        Dates expDate;
        Status status;
        std::string location;
        std::string description;
        Dates buyDate;
        double price;
    } Device;
    typedef std::vector<Device*> searchType;
private:
    typedef struct deviceList {
        Device * data = nullptr;
        deviceList * next = nullptr;
    } DeviceList;
    DeviceList * head = nullptr;
    DeviceList * tail = nullptr;
    int size;
    bool checkIdPresent(int id);
public:
    DeviceData();
    ~DeviceData();
    void add(int id, std::string name, std::string type, Status status, std::string location, std::string description,
             DeviceData::Dates expDate, DeviceData::Dates buyDate, double price);

    static bool isDateValid(Dates expDate);
    static bool isExp(Dates expDate);
    static std::string tostr(Status status);
    void remove(int id);
    void remove(const std::string& name);

    void
    update(int id, std::string name, std::string type, Status status, std::string location, std::string description,
           Dates expDate, Dates buyDate, double price);

    searchType search(const std::string& name);
    [[maybe_unused]] int getID(const std::string& name);
    searchType findExp();
    void sortDevice(SortType st);
    Device *get(int id);
    Device *get(const std::string& name);
    searchType get(); //get all devices

    void readFile();
    void writeFile();
};
#endif //DEVICEMANAGEMENT_DEVICEDATA_H

//
// Created by 神奇bug在哪里 on 2023/6/4.
//

#include "DeviceData.h"
#include "errors.h"
#include <sstream>
#include <fstream>
#ifdef __APPLE__
#include <utility>

#else
#include <cassert>
#include <cstring>
#endif
DeviceData::DeviceData() {
    head = new DeviceList;
    tail = head;
    assert(head != nullptr);
    size = 0;

}

DeviceData::~DeviceData()
{
    assert(head != nullptr);
    DeviceList * currentPoint = head;
    while (currentPoint != nullptr) {
        DeviceList * nextPoint = currentPoint->next;
        assert(currentPoint->data != nullptr);
        delete currentPoint->data;
        delete currentPoint;
        currentPoint = nextPoint;
    }
}
void DeviceData::add(int id, std::string name, std::string type, Status status, std::string location,
                     std::string description,
                     DeviceData::Dates expDate, DeviceData::Dates buyDate, double price) {
    if (!checkIdPresent(id)) {
        throw IdAlreadyExistError(id);
    }
    assert(tail != nullptr);
    DeviceList * currentPoint = tail;
    currentPoint->next = new DeviceList;
    assert(currentPoint->next != nullptr);
    currentPoint->next->data = new Device;
    assert(currentPoint->next->data != nullptr);
    currentPoint->next->data->id = id;
    currentPoint->next->data->name = std::move(name);
    currentPoint->next->data->type = std::move(type);
    currentPoint->next->data->status = status;
    currentPoint->next->data->location = std::move(location);
    currentPoint->next->data->description = std::move(description);
    currentPoint->next->data->expDate = expDate;
    currentPoint->next->data->buyDate = buyDate;
    currentPoint->next->data->price = price;
    currentPoint->next->next = nullptr;
    tail = currentPoint->next;
    size++;
}

void DeviceData::remove(int id) {
    assert(head != nullptr);
    DeviceList * currentPoint = head;
    while (currentPoint->next != nullptr) {
        if (currentPoint->next->data->id == id) {
            DeviceList * temp = currentPoint->next;
            currentPoint->next = currentPoint->next->next;
            delete temp->data;
            delete temp;
            size--;
            return;
        }
        currentPoint = currentPoint->next;
    }
    throw IdNotFoundError(id);
}

void DeviceData::remove(const std::string& name)
{
    assert(head != nullptr);
    DeviceList * currentPoint = head;
    while (currentPoint->next != nullptr) {
        if (currentPoint->next->data->name == name) {
            DeviceList * temp = currentPoint->next;
            currentPoint->next = currentPoint->next->next;
            delete temp->data;
            delete temp;
            size--;
            return;
        }
        currentPoint = currentPoint->next;
    }
    throw NameNotFoundError(name);
}

void DeviceData::update(int id, std::string name, std::string type, Status status, std::string location,
                        std::string description,
                        Dates expDate, Dates buyDate, double price) {
    if (!checkIdPresent(id)) {
        throw IdNotFoundError(id);
    }
    assert(head != nullptr);
    DeviceList * currentPoint = head;
    while (currentPoint->next != nullptr) {
        if (currentPoint->next->data->id == id) {
            currentPoint->next->data->name = std::move(name);
            currentPoint->next->data->type = std::move(type);
            currentPoint->next->data->status = status;
            currentPoint->next->data->location = std::move(location);
            currentPoint->next->data->expDate = expDate;
            currentPoint->next->data->buyDate = buyDate;
            currentPoint->next->data->price = price;
            currentPoint->next->data->description = std::move(description);
            return;
        }
        currentPoint = currentPoint->next;
    }
    throw IdNotFoundError(id);
}

[[maybe_unused]] int DeviceData::getID(const std::string &name) {
    assert(head != nullptr);
    DeviceList * currentPoint = head;
    while (currentPoint->next != nullptr) {
        if (currentPoint->next->data->name == name) {
            return currentPoint->next->data->id;
        }
        currentPoint = currentPoint->next;
    }
    throw NameNotFoundError(name);
}

DeviceData::searchType DeviceData::search(const std::string &name) {
    assert(head != nullptr);
    DeviceList * currentPoint = head;
    searchType result;
    while (currentPoint->next != nullptr) {
        if (strstr(currentPoint->next->data->name.c_str(),name.c_str()) != nullptr) {
            result.emplace_back(currentPoint->next->data);
        }
        currentPoint = currentPoint->next;
    }
    if (result.empty())
         throw NameNotFoundError(name);
    else
        return result;
}

DeviceData::Device *DeviceData::get(int id) {
    assert(head != nullptr);
    DeviceList * currentPoint = head;
    while (currentPoint->next != nullptr) {
        if (currentPoint->next->data->id == id) {
            return currentPoint->next->data;
        }
        currentPoint = currentPoint->next;
    }
    throw IdNotFoundError(id);
}

DeviceData::Device *DeviceData::get(const std::string &name) {
    assert(head != nullptr);
    DeviceList * currentPoint = head;
    while (currentPoint->next != nullptr) {
        if (currentPoint->next->data->name == name) {
            return currentPoint->next->data;
        }
        currentPoint = currentPoint->next;
    }
    throw NameNotFoundError(name);
}

DeviceData::searchType DeviceData::get() {
    assert(head != nullptr);
    DeviceList * currentPoint = head;
    searchType result;
    while (currentPoint->next != nullptr) {
        result.emplace_back(currentPoint->next->data);
        currentPoint = currentPoint->next;
    }
    return result;
}

DeviceData::searchType DeviceData::findExp() {
    DeviceList * currentPoint = head;
    searchType result;
    while (currentPoint->next != nullptr) {
        if (isExp(currentPoint->next->data->expDate))
            result.emplace_back(currentPoint->next->data);
        currentPoint = currentPoint->next;
    }
    return result;
}



bool DeviceData::isExp(DeviceData::Dates expDate) {
    //get system time
    time_t now = time(nullptr);
    tm *ltm = localtime(&now);
    //the year is 1900 + tm_year, due to the API.
    unsigned int year = 1900 + ltm->tm_year;
    unsigned int month = 1 + ltm->tm_mon;
    unsigned int day = ltm->tm_mday;
    DeviceData::Dates nowDate = {.year = year,.month = month,.day = day};
    //temperately use this method to compare, it supposed to be a better way
    if (expDate.year < nowDate.year)
        return true;
    else if (expDate.year == nowDate.year) {
        if (expDate.month < nowDate.month)
            return true;
        else if (expDate.month == nowDate.month) {
            if (expDate.day < nowDate.day)
                return true;
        }
    }
    return false;
}

void DeviceData::readFile() {
    std::ifstream file;
    file.open("data.csv");
    if (!file.is_open())
        throw FileOpenError();
    std::string line;
    while (getline(file,line)) {
        std::stringstream ss(line);
        std::string temp;
        std::vector<std::string> result;
        //used to store the result of each line
        while (getline(ss,temp,',')) {
            result.push_back(temp);
        }
        if (result.size() != 9)
            throw FileFormatError();
        int id = std::stoi(result[0]);
        std::string name = result[1];
        std::string type = result[2];
        Status status;
        if (result[3] == "UNUSED")
            status = UNUSED;
        else if (result[3] == "ON")
            status = ON;
        else if (result[3] == "OFF")
            status = OFF;
        else if (result[3] == "BROKEN")
            status = BROKEN;
        else
            throw FileFormatError();
        std::string location = result[4];
        std::string description = result[5];
        std::string date = result[6];
        std::stringstream ss_expDate(date);
        std::vector<std::string> result2;
        while (getline(ss_expDate, temp, '/')) {
            result2.push_back(temp);
        }
        if (result2.size() != 3)
            throw FileFormatError();
        Dates expDate = {.year = static_cast<unsigned int>(std::stoi(result2[0])), .month = static_cast<unsigned int>(std::stoi(result2[1])), .day = static_cast<unsigned int>(std::stoi(result2[2]))};
        std::string date2 = result[7];
        std::stringstream ss_buyDate(date2);
        std::vector<std::string> result3;
        while (getline(ss_buyDate, temp, '/')) {
            result3.push_back(temp);
        }
        if (result3.size() != 3)
            throw FileFormatError();
        Dates buyDate = {.year = static_cast<unsigned int>(std::stoi(result3[0])), .month = static_cast<unsigned int>(std::stoi(result3[1])), .day = static_cast<unsigned int>(std::stoi(result3[2]))};
        double price = std::stod(result[8]);
        //add to the list.
        add(id, name, type, status, location, description, expDate, buyDate, price);
    }
    //all datas should be read into the list, close the file.
    file.close();
}

void DeviceData::writeFile() {
    std::ofstream file;
    file.open("data.csv");
    if (!file.is_open())
        throw FileWriteError();
    DeviceList * currentPoint = head;
    while (currentPoint->next != nullptr) {
        file << currentPoint->next->data->id << "," << currentPoint->next->data->name << "," << currentPoint->next->data->type << ",";
        if (currentPoint->next->data->status == UNUSED)
            file << "UNUSED,";
        else if (currentPoint->next->data->status == ON)
            file << "ON,";
        else if (currentPoint->next->data->status == OFF)
            file << "OFF,";
        else if (currentPoint->next->data->status == BROKEN)
            file << "BROKEN,";
        else
            throw FileFormatError();
        file << currentPoint->next->data->location << "," << currentPoint->next->data->description << "," << currentPoint->next->data->expDate.year << "/" << currentPoint->next->data->expDate.month << "/" << currentPoint->next->data->expDate.day << "," << currentPoint->next->data->buyDate.year << "/" << currentPoint->next->data->buyDate.month << "/" << currentPoint->next->data->buyDate.day << "," << currentPoint->next->data->price << std::endl;
        currentPoint = currentPoint->next;
    }
    file.close();

}

bool DeviceData::isDateValid(DeviceData::Dates expDate) {
    if (expDate.year < 0 || expDate.month < 0 || expDate.day < 0)
        return false;
    if (expDate.month > 12)
        return false;
    if (expDate.month == 2) {
        if (expDate.year % 4 == 0) {
            if (expDate.day > 29)
                return false;
        }
        else {
            if (expDate.day > 28)
                return false;
        }
    }
    else if (expDate.month == 1 || expDate.month == 3 || expDate.month == 5 || expDate.month == 7 || expDate.month == 8 || expDate.month == 10 || expDate.month == 12) {
        if (expDate.day > 31)
            return false;
    }
    else {
        if (expDate.day > 30)
            return false;
    }
    return true;
}

void DeviceData::sortDevice(DeviceData::SortType st) {
    switch (st) {
        case byID:{
            DeviceList * currentPoint = head;
            while (currentPoint->next != nullptr) {
                DeviceList * currentPoint2 = currentPoint->next;
                while (currentPoint2->next != nullptr) {
                    if (currentPoint2->next->data->id < currentPoint->next->data->id) {
                        Device * temp = currentPoint2->next->data;
                        currentPoint2->next->data = currentPoint->next->data;
                        currentPoint->next->data = temp;
                    }
                    currentPoint2 = currentPoint2->next;
                }
                currentPoint = currentPoint->next;
            }
            break;
        }
        case byPrice:{
            DeviceList * currentPoint = head;
            while (currentPoint->next != nullptr) {
                DeviceList * currentPoint2 = currentPoint->next;
                while (currentPoint2->next != nullptr) {
                    if (currentPoint2->next->data->price < currentPoint->next->data->price) {
                        Device * temp = currentPoint2->next->data;
                        currentPoint2->next->data = currentPoint->next->data;
                        currentPoint->next->data = temp;
                    }
                    currentPoint2 = currentPoint2->next;
                }
                currentPoint = currentPoint->next;
            }
            break;
        }
        case byExpDate:{
            DeviceList * currentPoint = head;
            while (currentPoint->next != nullptr) {
                DeviceList * currentPoint2 = currentPoint->next;
                while (currentPoint2->next != nullptr) {
                    if (currentPoint2->next->data->expDate.year < currentPoint->next->data->expDate.year) {
                        Device * temp = currentPoint2->next->data;
                        currentPoint2->next->data = currentPoint->next->data;
                        currentPoint->next->data = temp;
                    }
                    else if (currentPoint2->next->data->expDate.year == currentPoint->next->data->expDate.year) {
                        if (currentPoint2->next->data->expDate.month < currentPoint->next->data->expDate.month) {
                            Device * temp = currentPoint2->next->data;
                            currentPoint2->next->data = currentPoint->next->data;
                            currentPoint->next->data = temp;
                        }
                        else if (currentPoint2->next->data->expDate.month == currentPoint->next->data->expDate.month) {
                            if (currentPoint2->next->data->expDate.day < currentPoint->next->data->expDate.day) {
                                Device * temp = currentPoint2->next->data;
                                currentPoint2->next->data = currentPoint->next->data;
                                currentPoint->next->data = temp;
                            }
                        }
                    }
                    currentPoint2 = currentPoint2->next;
                }
                currentPoint = currentPoint->next;
            }
            break;
        }
        case byBuyDate:{
            DeviceList * currentPoint = head;
            while (currentPoint->next != nullptr) {
                DeviceList * currentPoint2 = currentPoint->next;
                while (currentPoint2->next != nullptr) {
                    if (currentPoint2->next->data->buyDate.year < currentPoint->next->data->buyDate.year) {
                        Device * temp = currentPoint2->next->data;
                        currentPoint2->next->data = currentPoint->next->data;
                        currentPoint->next->data = temp;
                    }
                    else if (currentPoint2->next->data->buyDate.year == currentPoint->next->data->buyDate.year) {
                        if (currentPoint2->next->data->buyDate.month < currentPoint->next->data->buyDate.month) {
                            Device * temp = currentPoint2->next->data;
                            currentPoint2->next->data = currentPoint->next->data;
                            currentPoint->next->data = temp;
                        }
                        else if (currentPoint2->next->data->buyDate.month == currentPoint->next->data->buyDate.month) {
                            if (currentPoint2->next->data->buyDate.day < currentPoint->next->data->buyDate.day) {
                                Device * temp = currentPoint2->next->data;
                                currentPoint2->next->data = currentPoint->next->data;
                                currentPoint->next->data = temp;
                            }
                        }
                    }
                    currentPoint2 = currentPoint2->next;
                }
                currentPoint = currentPoint->next;
            }
            break;
        }
    }
}

std::string DeviceData::tostr(DeviceData::Status status) {
    if (status == UNUSED)
        return "In Repository";
    else if (status == ON)
        return "In Use";
    else if (status == BROKEN)
        return "Broken";
    else if (status == OFF)
        return "Used but not running";
    else if (status == RECYCLED)
        return "Has been recycled";
    else
        return "Unknown";

}

bool DeviceData::checkIdPresent(int id) {
    try {
        get(id);
    }
    catch (IdNotFoundError & e) {
        return true;
    }
    return false;
}








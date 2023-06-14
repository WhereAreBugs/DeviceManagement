//
// Created by 神奇bug在哪里 on 2023/6/5.
//

#include <iostream>
#include "UserInterface.h"
#include "errors.h"
#include "Runner.h"

UserInterface::UserInterface() {
    choice = 0;
    //Due to the lack of time to deal with the codec problem, so all the Chinese characters are commented out.
    //MacOS and most Linux uses UTF-8 as the default, but Windows uses GBK as the default.
    std::cout << "Welcome to Device Management System!" << std::endl;
    //Just for now, the following code is commented out.
}

/**
 *  @brief This function is used to pick the function that user want to use.
 *  @param None
 *  @attention Please use it as a loop.
 */
void UserInterface::picker() {
    //to avoid the stdin buffer exists some trash
    std::cout << "Please choose the function you want to use:" << std::endl;
    std::cout << "1. Add a device." << std::endl;
    std::cout << "2. Remove a device." << std::endl;
    std::cout << "3. get a device's Info." << std::endl;
    std::cout << "4. Show all devices." << std::endl;
    std::cout << "5. Search from all devices." << std::endl;
    std::cout << "6. Filter all the expired device" << std::endl;
    std::cout << "7. Update a device's Info" << std::endl;
    std::cout << "8. show sorted devices' info" << std::endl;
    std::cout << "9. Save and exit." << std::endl;
    std::cin >> choice;
    switch (choice) {
        case 1:
            try {
                addDevice();
            } catch (ParamError &e) {
                std::cout << e.what() << std::endl;
#ifdef _WIN32
                system("pause");
#else
                system("read -n 1 -p \"Press any key to continue...\"");
#endif
            }
            break;
        case 2:
            try {
                removeDevice();
            } catch (ParamError &e) {
                std::cout << e.what() << std::endl;
#ifdef _WIN32
                system("pause");
#else
                system("read -n 1 -p \"Press any key to continue...\"");
#endif
            }
            break;
        case 3:
            try {
                getOneDevice();
            } catch (ParamError &e) {
                std::cout << e.what() << std::endl;
#ifdef _WIN32
                system("pause");
#else
                system("read -n 1 -p \"Press any key to continue...\"");
#endif
            }
            break;
        case 4:
            try {
                printAllDevice();
            } catch (ParamError &e) {
                std::cout << e.what() << std::endl;
#ifdef _WIN32
                system("pause");
#else
                system("read -n 1 -p \"Press any key to continue...\"");
#endif
            }
            break;
        case 5:
            try {
                searchDevice();
            } catch (ParamError &e) {
                std::cout << e.what() << std::endl;
#ifdef _WIN32
                system("pause");
#else
                system("read -n 1 -p \"Press any key to continue...\"");
#endif
            }
            break;

        case 6: {
            try {
                filterExpDevice();
            }
            catch (ParamError &e) {
                std::cout << e.what() << std::endl;
#ifdef _WIN32
                system("pause");
#else
                system("read -n 1 -p \"Press any key to continue...\"");
#endif
            }
            break;
        }
        case 7: {
            try {
                updateDevice();
            }
            catch (ParamError &e) {
                std::cout << e.what() << std::endl;
#ifdef _WIN32
                system("pause");
#else
                system("read -n 1 -p \"Press any key to continue...\"");
#endif
            }
            break;
        }
        case 8:{
            try {
                sortAndShowDevice();
            }
            catch (ParamError &e) {
                std::cout << e.what() << std::endl;
            }
            catch (NullError &e){
                std::cout << e.what() << std::endl;
            }
            // due to the code reuse, the pause has been called inside the function, so we don't need to call it again.
            break;
        }
        case 9: {
            std::cout << "Thanks for using!" << std::endl;
            Comments::deviceData->writeFile();
            exit(0);
        }
        default:
            if (std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            throw InvalidChoiceError(choice);
    }
}

/**
 * @brief This function is used to add a device.
 * @param None
 * @attention This function will throw an exception if the input is invalid.
 */
void UserInterface::addDevice() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    std::cout << "Please input the device's id:" << std::endl;
    int id;
    std::cin >> id;
    std::cout << "Please input the device's name:" << std::endl;
    std::string name;
    std::cin >> name;
    std::cout << "Please input the device's type:" << std::endl;
    std::string type;
    std::cin >> type;
    std::cout << "Please choose the device's status:" << std::endl;
    std::cout << "0. UNUSED" << std::endl;
    std::cout << "1. ON" << std::endl;
    std::cout << "2. OFF" << std::endl;
    std::cout << "3. BROKEN" << std::endl;
    DeviceData::Status status;
    std::cin >> reinterpret_cast<int &>(status);
    std::cout << "Please input the device's location:" << std::endl;
    std::string location;
    std::cin >> location;
    std::cout << "Please input the device's description:" << std::endl;
    std::string description;
    std::cin >> description;
    std::cout << "Please input the device's expire Date:(format: years/month/days)" << std::endl;
    unsigned int years, month, days;
    std::cin >> years >> month >> days;
#ifdef __APPLE__
    DeviceData::Dates result = {.year = years, .month = month, .day = days};
#else
    DeviceData::Dates result = {.day = days , .month = month, .year = years};
#endif
    if (!DeviceData::isDateValid(result)) {
        throw InputDataInvalidError("Date is invalid.");
    }
    std::cout << "Please input the device's buy Date:" << std::endl;
    std::cin >> years >> month >> days;
#ifdef __APPLE__
    DeviceData::Dates buyDate = {.year = years, .month = month, .day = days};
#else
    DeviceData::Dates buyDate = {.day = days , .month = month, .year = years};
#endif
    if (!DeviceData::isDateValid(buyDate)) {
        throw InputDataInvalidError("Date is invalid.");
    }
    
    std::cout << "Please input the device's price:" << std::endl;
    double price;
    std::cin >> price;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw InputDataInvalidError("Input is invalid.");
    }

    Comments::deviceData->add(id, name, type, status, location, description, result, buyDate, price);
}

void UserInterface::removeDevice() {
    std::cout << "WARNING: This operation will remove the device from the database." << std::endl;
    std::cout << "authentication is required." << std::endl;
    std::cout << "Please input the password:" << std::endl;
    std::string password;
    std::cin >> password;
    if (password != "password") {
        throw PasswordError();
    }
    std::cout << "Please choose the method you want to use:" << std::endl;
    std::cout << "1. Search by id." << std::endl;
    std::cout << "2. Search by name." << std::endl;
    std::cin >> choice;
    switch (choice) {
        case 1: {
            std::cout << "Please input the device's id:" << std::endl;
            int id;
            std::cin >> id;
            Comments::deviceData->remove(id);
            break;
        }
        case 2: {
            std::cout << "Please input the device's name:" << std::endl;
            std::string name;
            std::cin >> name;
            Comments::deviceData->remove(name);
            break;
        }
        default:
            throw InvalidChoiceError(choice);
    }

}

void UserInterface::updateDevice() {
    std::cout << "Please choose the method you want to use:" << std::endl;
    std::cout << "1. Update by id." << std::endl;
    std::cout << "2. Update by name." << std::endl;
    std::cin >> choice;
    switch (choice) {
        case 1: {
            std::cout << "Please input the device's id:" << std::endl;
            int id;
            std::cin >> id;
            std::cout << "Please input the device's name:" << std::endl;
            std::string name;
            std::cin >> name;
            std::cout << "Please input the device's type:" << std::endl;
            std::string type;
            std::cin >> type;
            std::cout << "Please choose the device's status:" << std::endl;
            std::cout << "0. UNUSED" << std::endl;
            std::cout << "1. ON" << std::endl;
            std::cout << "2. OFF" << std::endl;
            std::cout << "3. BROKEN" << std::endl;
            DeviceData::Status status;
            std::cin >> reinterpret_cast<int &>(status);
            std::cout << "Please input the device's location:" << std::endl;
            std::string location;
            std::cin >> location;
            std::cout << "Please input the device's description:" << std::endl;
            std::string description;
            std::cin >> description;
            std::cout << "Please input the device's expire Date:(format: years/month/days)" << std::endl;
            unsigned int years, month, days;
            std::cin >> years >> month >> days;
#ifdef __APPLE__
             DeviceData::Dates result = {.year = years, .month = month, .day = days};
#else
            DeviceData::Dates result = {.day = days , .month = month, .year = years};
#endif
            if (!DeviceData::isDateValid(result)) {
                throw InputDataInvalidError("Date is invalid.");
            }
            std::cout << "Please input the device's buy Date:" << std::endl;
            std::cin >> years >> month >> days;
#ifdef __APPLE__
            DeviceData::Dates buyDate = {.year = years, .month = month, .day = days};
#else
            DeviceData::Dates buyDate = {.day = days , .month = month, .year = years};
#endif
            if (!DeviceData::isDateValid(buyDate)) {
                throw InputDataInvalidError("Date is invalid.");
            }
            std::cout << "Please input the device's price:" << std::endl;
            double price;
            std::cin >> price;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw InputDataInvalidError("Input is invalid.");
            }
            Comments::deviceData->update(id, name, type, status, location, description, result, buyDate, price);
            break;
        }
        case 2: {
            std::cout << "Please input the device's name:" << std::endl;
            std::string name;
            std::cin >> name;
            std::cout << "Please input the device's id:" << std::endl;
            int id;
            std::cin >> id;
            std::cout << "Please input the device's type:" << std::endl;
            std::string type;
            std::cin >> type;
            std::cout << "Please choose the device's status:" << std::endl;
            std::cout << "0. UNUSED" << std::endl;
            std::cout << "1. ON" << std::endl;
            std::cout << "2. OFF" << std::endl;
            std::cout << "3. BROKEN" << std::endl;
            DeviceData::Status status;
            std::cin >> reinterpret_cast<int &>(status);
            std::cout << "Please input the device's location:" << std::endl;
            std::string location;
            std::cin >> location;
            std::cout << "Please input the device's description:" << std::endl;
            std::string description;
            std::cin >> description;
            std::cout << "Please input the device's expire Date:(format: years/month/days)" << std::endl;
            unsigned int years, month, days;
            std::cin >> years >> month >> days;
#ifdef  __AARCH64EL__
            DeviceData::Dates result = {.year = years, .month = month, .day = days};
#else
            DeviceData::Dates result = {.day = days , .month = month, .year = years};
#endif
            if (!DeviceData::isDateValid(result)) {
                throw InputDataInvalidError("Date is invalid.");
            }
            std::cout << "Please input the device's buy Date:" << std::endl;
            std::cin >> years >> month >> days;
#ifdef __APPLE__
            DeviceData::Dates buyDate = {.year = years, .month = month, .day = days};
#else
            DeviceData::Dates buyDate = {.day = days , .month = month, .year = years};
#endif
            if (!DeviceData::isDateValid(buyDate)) {
                throw InputDataInvalidError("Date is invalid.");
            }
            std::cout << "Please input the device's price:" << std::endl;
            double price;
            std::cin >> price;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw InputDataInvalidError("Input is invalid.");
            }
            Comments::deviceData->update(id, name, type, status, location, description, result, buyDate, price);
            break;
        }
        default:
            throw InvalidChoiceError(choice);
    }
}

void UserInterface::getOneDevice() {
    std::cout << "Please choose the method you want to use:" << std::endl;
    std::cout << "1. Get by id." << std::endl;
    std::cout << "2. Get by name." << std::endl;
    std::cin >> choice;
    switch (choice) {
        case 1: {
            std::cout << "Please input the device's id:" << std::endl;
            int id;
            std::cin >> id;
            auto data = Comments::deviceData->get(id);
            print(data);
            break;
        }
        case 2: {
            std::cout << "Please input the device's name:" << std::endl;
            std::string name;
            std::cin >> name;
            auto data = Comments::deviceData->get(name);
            print(data);
            break;
        }
        default:
            throw InvalidChoiceError(choice);
    }
#ifdef _WIN32
    system("pause");
#else
    system("read -n 1 -p \"Press any key to continue...\" INP");
#endif
}

/**
 * @brief This function is used to get a device Info.
 * @param None
 * @attention This function will throw an exception if the input is invalid.
 *
 */

void UserInterface::printAllDevice() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    auto data = Comments::deviceData->get();
    int count = 0;
    for (auto &i: data) {
        std::cout << "Data" << count++ << ":" << std::endl;
        print(i);
    }
#ifdef _WIN32
    system("pause");
#else
    system("read -n 1 -p \"Press any key to continue...\" INP");
#endif
}

void UserInterface::print(DeviceData::Device *target) {
    std::cout << "Device's id: " << target->id << "\t\t" << "Device's name: " << target->name << "\t\t" << std::endl
              << "Device's type: " << target->type << "\t\t" << "Device's status: ";
    if (!DeviceData::isExp(target->expDate)&&(target->status!=DeviceData::RECYCLED)){
        std::cout << DeviceData::tostr(target->status) << "\t\t" << std::endl;
    } else {
        std::cout << "EOL" << "\t\t" << std::endl;
    }
        std::cout << "Device's location: " << target->location << std::endl << "Device's description: "
              << target->description << "\t" << std::endl << "Device's expire date: " << target->expDate.year
              << "/" << target->expDate.month << "/"  << target->expDate.day << std::endl << "Device's buy date: "
              << target->buyDate.year << "/" << target->buyDate.month << "/" << target->buyDate.day << std::endl
              << "Device's price: " << target->price << std::endl << std::endl;
}

void UserInterface::searchDevice() {
    std::cout << "Please input the device's name:" << std::endl;
    std::string name;
    std::cin >> name;
    auto data = Comments::deviceData->search(name);
    int count = 0;
    for(auto &i: data) {
        std::cout << "SearchResult" << count++ << ":" << std::endl;
        print(i);
    }
#ifdef _WIN32
    system("pause");
#else
    system("read -n 1 -p \"Press any key to continue...\" INP");
#endif
}

void UserInterface::filterExpDevice() {
    auto data = Comments::deviceData->findExp();
    int count = 0;
    for(auto &i: data) {
        std::cout << "SearchResult" << count++ << ":" << std::endl;
        print(i);
    }
#ifdef _WIN32
    system("pause");
#else
    system("read -n 1 -p \"Press any key to continue...\" INP");
#endif
}

void UserInterface::sortAndShowDevice() {
    int choice;
    DeviceData::SortType sortType;
    std::cout << "Please choose the method you want to use:" << std::endl;
    std::cout << "1. Sort by id." << std::endl;
    std::cout << "2. Sort by expDate." << std::endl;
    std::cout << "3. Sort by price." << std::endl;
    std::cout << "4. Sort by buyDate." << std::endl;
    std::cin >> choice;
    sortType = choice == 1 ? DeviceData::SortType::byID : choice == 2 ? DeviceData::SortType::byExpDate
                                                                   : choice == 3 ? DeviceData::SortType::byPrice
                                                                                 : DeviceData::SortType::byBuyDate;
    Comments::deviceData->sortDevice(sortType);
    printAllDevice();
}

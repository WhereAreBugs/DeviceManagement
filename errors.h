//
// Created by 神奇bug在哪里 on 2023/6/4.
//

#ifndef DEVICEMANAGEMENT_ERRORS_H
#define DEVICEMANAGEMENT_ERRORS_H
#include <exception>
#include <string>
#include <utility>
class ParamError : public std::exception {
public:
    [[nodiscard]] virtual const char * what() const noexcept  { // NOLINT(modernize-use-override)
        return "Param error.";
    }
};
class FileError : public std::exception {
public:
    [[nodiscard]] virtual const char * what() const noexcept  { // NOLINT(modernize-use-override)
        return "File error.";
    }
};
class NullError : public std::exception {
public:
    [[nodiscard]] virtual const char * what() const noexcept  { // NOLINT(modernize-use-override)
        return "Database Null error.";
    }
};
class FileOpenError : public FileError {
public:
    [[nodiscard]] const char * what() const noexcept override {
        return "File open error.";
    }
};
class FileFormatError : public FileError {
public:
    [[nodiscard]] const char * what() const noexcept override {
        return "File Format error.";
    }
};
class  FileWriteError : public FileError {
public:
    [[nodiscard]] const char * what() const noexcept override {
        return "File write error.";
    }
};
class PasswordError : public ParamError {
public:
    [[nodiscard]] const char * what() const noexcept override {
        return "Password error.";
    }
};

class IdNotFoundError : public ParamError {
private:
    std::string error_msg;
public:
    explicit IdNotFoundError(int id) {
        error_msg = "Id " + std::to_string(id) + " not found.";
    }
    [[nodiscard]] const char * what() const noexcept override {
        return error_msg.c_str();
    }
};
class NameNotFoundError : public ParamError {
private:
    std::string error_msg;
public:
    //此处可能发生调用堆栈的切换，不应该使用引用
    explicit NameNotFoundError(std::string name) { // NOLINT(performance-unnecessary-value-param)
        error_msg = "Name " + name + " not found.";
    }
    [[nodiscard]] const char * what() const noexcept override {
        return error_msg.c_str();
    }
};
class InputDataInvalidError : public ParamError {
private:
    typedef enum{
        ID,
        NAME,
        TYPE,
        STATUS,
        LOCATION,
        DESCRIPTION
    } InputType;
    std::string error_msg;
public:
    explicit InputDataInvalidError(std::string type) { // NOLINT(performance-unnecessary-value-param)
        error_msg = std::move(type);
    }

    explicit InputDataInvalidError(InputType type) {
        switch (type) {
            case ID:
                error_msg = "Id must be a positive integer.";
                break;
            case NAME:
                error_msg = "Name must be a string.";
                break;
            case TYPE:
                error_msg = "Type must be a string.";
                break;
            case STATUS:
                error_msg = "Status must be one of the following: UNUSED, ON, OFF, BROKEN.";
                break;
            case LOCATION:
                error_msg = "Location must be a string.";
                break;
            case DESCRIPTION:
                error_msg = "Description must be a string.";
                break;
        }
    }
    [[nodiscard]] const char * what() const noexcept override {
        return error_msg.c_str();
    }
};
class InvalidChoiceError : public ParamError {
private:
    std::string error_msg;
public:
    explicit InvalidChoiceError(int choice) {
        error_msg = "Choice " + std::to_string(choice) + " is invalid.";
    }
    [[nodiscard]] const char * what() const noexcept override {
        return error_msg.c_str();
    }
};
class IdAlreadyExistError : public ParamError {
private:
    std::string error_msg;
public:
    explicit IdAlreadyExistError(int id) {
        error_msg = "Id " + std::to_string(id) + " already exists.";
    }
    [[nodiscard]] const char * what() const noexcept override {
        return error_msg.c_str();
    }
};
#endif //DEVICEMANAGEMENT_ERRORS_H

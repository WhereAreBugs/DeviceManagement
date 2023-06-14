cmake_minimum_required(VERSION 3.25)
project(DeviceManagement)



set(CMAKE_CXX_STANDARD 17)

add_executable(DeviceManagement main.cpp  DeviceData.cpp DeviceData.h errors.h  UserInterface.cpp UserInterface.h Runner.cpp Runner.h)


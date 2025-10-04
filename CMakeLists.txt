cmake_minimum_required(VERSION 3.29)
project(BDSMaksym_OOP2025)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(BDSMaksym_OOP2025
    anekdoty.txt
        mood.cpp
    mood.h
    mood_log.csv)

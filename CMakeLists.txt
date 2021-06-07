cmake_minimum_required(VERSION 3.16)
project(TicketSystem)

set(CMAKE_CXX_STANDARD 14)

# add_compile_options(-std=c++11 -Ofast)

add_executable(code src/main.cpp src/includes/L_time.cpp
        src/includes/parser.cpp src/includes/ticketSystem.cpp
        src/includes/user.cpp src/includes/ticketSystem.cpp
        src/includes/Algorithm.cpp src/includes/train.cpp
        src/includes/commandManager.cpp)
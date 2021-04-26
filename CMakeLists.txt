cmake_minimum_required(VERSION 3.19)
project(TicketSystem)

set(CMAKE_CXX_STANDARD 14)

add_executable(TicketSystem src/main.cpp src/includes/parser.cpp src/includes/ticketSystem.cpp src/includes/user.cpp src/includes/ticketSystem.cpp src/includes/Algorithm.cpp)
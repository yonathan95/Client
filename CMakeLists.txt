cmake_minimum_required(VERSION 3.17)
project(Client)

set(CMAKE_CXX_STANDARD 11)

add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/Client.cpp src/KeyboardReader.cpp include/KeyboardReader.h src/SocketReader.cpp include/SocketReader.h)

include_directories(~/CLionProjects/Client/include)
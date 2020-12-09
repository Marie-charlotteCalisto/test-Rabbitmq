cmake_minimum_required(VERSION 3.19)

project(PocRabbitmq)

######## Set Variable ##########
Set(SOURCES src/main.cpp src/ConnectionHandler.cpp src/parse.cpp)

##### exe #####
add_executable(amqpcpp_example ${SOURCES})


#### lib ####
target_link_libraries(amqpcpp_example amqpcpp)


#### dir ####
target_include_directories(amqpcpp_example PRIVATE include)

cmake_minimum_required(VERSION 3.19)

project(PocRabbitmq)

######## Set Variable ##########
Set(SOURCES src/parse.cpp)

##### exe #####
add_executable(amqpcpp1 src/exe1.cpp ${SOURCES})
add_executable(amqpcpp2 src/exe2.cpp ${SOURCES})


#### lib ####
target_link_libraries(amqpcpp1 amqpcpp)
target_link_libraries(amqpcpp2 amqpcpp)


#### dir ####
target_include_directories(amqpcpp1 PRIVATE include)
target_include_directories(amqpcpp2 PRIVATE include)

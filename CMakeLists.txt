cmake_minimum_required(VERSION 3.19)

project(PocRabbitmq)

######## Set Variable ##########
#Set(SOURCES src/parse.cpp)

##### exe #####


add_executable(superviser src/superviser.cpp)
add_executable(client1 src/client1.cpp)
add_executable(client2 src/client2.cpp)



#### lib ####
target_link_libraries(superviser amqpcpp ev pthread dl ssl curl jsoncpp)
target_link_libraries(client1 amqpcpp ev pthread dl ssl)
target_link_libraries(client2 amqpcpp ev pthread dl ssl)


#### dir ####
#target_include_directories(client1 PRIVATE include)
#target_include_directories(client2 PRIVATE include)

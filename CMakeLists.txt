cmake_minimum_required(VERSION 3.19)

project(PocRabbitmq)

######## Set Variable ##########
Set(SOURCES src/callbacks.cpp)


##### exe #####
add_executable(superviser src/superviser.cpp)

add_executable(client1 src/client1.cpp ${SOURCES})
add_executable(client2 src/client2.cpp ${SOURCES})

add_executable(client3 src/client3.cpp ${SOURCES})
add_executable(client4 src/client4.cpp ${SOURCES})




#### lib ####
target_link_libraries(superviser amqpcpp ev pthread dl ssl curl jsoncpp)

target_link_libraries(client1 amqpcpp ev pthread dl ssl)
target_link_libraries(client2 amqpcpp ev pthread dl ssl)

target_link_libraries(client3 amqpcpp ev pthread dl ssl)
target_link_libraries(client4 amqpcpp ev pthread dl ssl)



#### dir ####
target_include_directories(client1 PRIVATE include)
target_include_directories(client2 PRIVATE include)

target_include_directories(client3 PRIVATE include)
target_include_directories(client4 PRIVATE include)

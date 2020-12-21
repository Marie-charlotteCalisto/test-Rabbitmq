cmake_minimum_required(VERSION 3.19)

project(PocRabbitmq)

######## Set Variable ##########
Set(SOURCES src/parse.cpp)

##### exe #####
#add_executable(amqpcpp1 exemple/Test1NoTCP.cpp ${SOURCES})
#add_executable(amqpcpp2 exemple/Test2NoTCP.cpp ${SOURCES})



#add_executable(sendTCP src/samequeuefrist.cpp)
#add_executable(respondTCP src/samequeueSecond.cpp)
#add_executable(superviserTCP src/superviser.cpp)
add_executable(client1 src/client1.cpp)
add_executable(client2 src/client2.cpp)
add_executable(server src/server.cpp)



#### lib ####
#target_link_libraries(amqpcpp1 amqpcpp ev pthread dl ssl)
#target_link_libraries(amqpcpp2 amqpcpp)
#target_link_libraries(superviserTCP amqpcpp ev pthread dl ssl)
target_link_libraries(server amqpcpp ev pthread dl ssl)
target_link_libraries(client1 amqpcpp ev pthread dl ssl)
target_link_libraries(client2 amqpcpp ev pthread dl ssl)


#### dir ####
#target_include_directories(amqpcpp1 PRIVATE include)
#target_include_directories(amqpcpp2 PRIVATE include)
#target_include_directories(sendTCP PRIVATE include)
#target_include_directories(respondTCP PRIVATE include)

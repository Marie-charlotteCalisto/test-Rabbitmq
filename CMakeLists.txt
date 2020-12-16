cmake_minimum_required(VERSION 3.19)

project(PocRabbitmq)

######## Set Variable ##########
Set(SOURCES src/parse.cpp)

##### exe #####
#add_executable(amqpcpp1 exemple/Test1NoTCP.cpp ${SOURCES})
#add_executable(amqpcpp2 exemple/Test2NoTCP.cpp ${SOURCES})



#add_executable(sendTCP src/samequeuefrist.cpp)
#add_executable(respondTCP src/samequeueSecond.cpp)
add_executable(sendTCP src/diffqueuefirst.cpp)
add_executable(respondTCP src/diffqueueSecond.cpp)



#### lib ####
#target_link_libraries(amqpcpp1 amqpcpp ev pthread dl ssl)
#target_link_libraries(amqpcpp2 amqpcpp)
target_link_libraries(sendTCP amqpcpp ev pthread dl ssl)
target_link_libraries(respondTCP amqpcpp ev pthread dl ssl)


#### dir ####
#target_include_directories(amqpcpp1 PRIVATE include)
#target_include_directories(amqpcpp2 PRIVATE include)
target_include_directories(sendTCP PRIVATE include)
target_include_directories(respondTCP PRIVATE include)

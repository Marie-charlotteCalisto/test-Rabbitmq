cmake_minimum_required(VERSION 3.19)

project(PocRabbitmq)

######## Set Variable ##########
Set(SOURCES src/callbacks.cpp src/client.cpp)


##### exe #####
add_executable(discuss src/superviser.cpp ${SOURCES})

#add_executable(http old_implementations/http_request.cpp ${SOURCES})

#add_executable(client1 old_implementations/client1.cpp ${SOURCES})
#add_executable(client2 old_implementations/client2.cpp ${SOURCES})

#add_executable(client3 old_implementations/client3.cpp ${SOURCES})
#add_executable(client4 old_implementations/client4.cpp ${SOURCES})

#add_executable(thread old_implementations/testthread.cpp)




#### lib ####
target_link_libraries(discuss amqpcpp ev pthread dl ssl curl)


#target_link_libraries(http amqpcpp ev pthread dl ssl curl jsoncpp)

#target_link_libraries(client1 amqpcpp ev pthread dl ssl)
#target_link_libraries(client2 amqpcpp ev pthread dl ssl)

#target_link_libraries(client3 amqpcpp ev pthread dl ssl)
#target_link_libraries(client4 amqpcpp ev pthread dl ssl)

#target_link_libraries(thread amqpcpp ev pthread dl ssl)



#### dir ####
target_include_directories(discuss PRIVATE include)

#target_include_directories(client1 PRIVATE include)
#target_include_directories(client2 PRIVATE include)

#target_include_directories(client3 PRIVATE include)
#target_include_directories(client4 PRIVATE include)

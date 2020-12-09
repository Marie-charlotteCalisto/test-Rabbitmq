cmake_minimum_required(VERSION 3.19)

project(PocRabbitmq)

###################################
# Libevent
###################################


add_executable(amqpcpp_libev_example main.cpp ConnectionHandler.cpp parse.cpp)

#find_library(AMQPCPP_LIB amqpcpp)

#add_dependencies(amqpcpp_libev_example amqpcpp)

#target_link_libraries(amqpcpp_libev_example amqpcpp ev pthread dl ssl)
target_link_libraries(amqpcpp_libev_example amqpcpp)


#target_include_directories()

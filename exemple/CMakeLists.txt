cmake_minimum_required(VERSION 3.19)

project(PocRabbitmq)


###################################
# Libev
###################################

add_executable(amqpcpp_libev_example libev.cpp)

target_link_libraries(amqpcpp_libev_example amqpcpp ev pthread dl ssl)

#pragma once


#include <amqpcpp.h>
#include <amqpcpp/libev.h>
#include <unistd.h>

/**
 * \brief callback function that is called when message is received.
 * add one to the integer received
 *
 * TODO: check if received message is an integer
 * 
 * @param channel : the channel to acknowledge and publish messages
 * @param routingkey : the key to be checked
 * @param publishkey : the key to publish the message with
 */
const AMQP::MessageCallback AdditionMessage(
		AMQP::TcpChannel *channel,
		class Client *client);


/** \brief callback function that is called when the consume operation starts
 */
const AMQP::ConsumeCallback startCb = [](const std::string &consumertag) {

	std::cout << "consume operation started: " << consumertag << std::endl;
};

/** \breif callback function that is called when the consume operation failed
 */
const AMQP::ErrorCallback errorCb = [](const char *message) {

	std::cout << "consume operation failed : " << message << std::endl;
};

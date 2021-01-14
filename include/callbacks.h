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
 * \param client : the client who send the message
 *
 * \return lambda messageCallback function that add 1 to the message recieved 
 */
const AMQP::MessageCallback AdditionMessage(
		AMQP::TcpChannel *channel,
		class Client *client);

/**
 * \brief callback function that is called when queue is declared.
 * display number of consumers on the queue
 *
 * \param connection : the connection to be closed
 * \param queue : the queue where the consumers exchange
 */
const AMQP::QueueCallback displayNbConsumer(AMQP::TcpConnection *connection, std::string queue);

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

/** \breif callback fuction that is called when the binding of the queue and exchange succeded.
 */
const AMQP::SuccessCallback binded = []() {


	std::cout << "binded" << std::endl;
};

/** \brief callback function that is called when the queue declaration succeded
 */
const AMQP::SuccessCallback queueDeclared = []() {


	std::cout << "queue declared" << std::endl;
};

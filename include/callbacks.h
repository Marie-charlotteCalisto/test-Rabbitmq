#pragma once

#include <amqpcpp.h>

#include <amqpcpp/libev.h>
#include <unistd.h>


const AMQP::MessageCallback AdditionMessage(
		AMQP::TcpChannel *channel,
		std::string routingkey,
		std::string publishkey);


// callback function that is called when the consume operation starts
const AMQP::ConsumeCallback startCb = [](const std::string &consumertag) {

	std::cout << "consume operation started: " << consumertag << std::endl;
};

// callback function that is called when the consume operation failed
const AMQP::ErrorCallback errorCb = [](const char *message) {

	std::cout << "consume operation failed : " << message << std::endl;
};

// callback function that is called when the queue declaration succeded
const AMQP::SuccessCallback queueDeclared = []() {

	std::cout << "queue declared" << std::endl;
};

// callback fuction that is called when the binding of the queue and exchange succeded.
const AMQP::SuccessCallback binded = []() {

	std::cout << "binded" << std::endl;
};

// callback fuction that is called when the binding of the queue and exchange succeded.
// send first message to begin conversation
const AMQP::SuccessCallback bindAndSend(
		AMQP::TcpChannel *channel,
		std::string publishkey);

#include "callbacks.h"

const AMQP::MessageCallback AdditionMessage(
		AMQP::TcpChannel *channel,
		std::string routingkey,
		std::string publishkey)
{
	return [channel, routingkey, publishkey](
		const AMQP::Message &message, uint64_t deliveryTag,
		bool redelivered)
	{
		auto key = message.routingkey();
		if (key != routingkey)
			return;

		// acknowledge the message
		channel->ack(deliveryTag);

		//get number sent and add one
		auto messageS =
			std::stoi(std::string(message.body(), message.body() + message.bodySize())) + 1;

		std::cout << "message received from " << message.exchange()
			<< " key " << key << " :\"" << messageS << "\"" << std::endl;

		//publish after one second
		channel->publish("my-exchange", publishkey, std::to_string(messageS));
		usleep(1000000);
	};
};

const AMQP::SuccessCallback bindAndSend(
		AMQP::TcpChannel *channel,
		std::string publishkey)
{
	return [channel, publishkey] ()
	{
		std::cout << "binded" << std::endl;

		//publish first message to begin conversation
		channel->publish("my-exchange", publishkey, "0");
		usleep(1000000);
	};
};


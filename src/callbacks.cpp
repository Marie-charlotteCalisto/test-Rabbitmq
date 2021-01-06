#include "callbacks.h"
#include "client.h"

//#include "thread"
//#include "chrono"

const AMQP::MessageCallback AdditionMessage(
		AMQP::TcpChannel *channel,
		Client *client)
{
	return [channel, client](
		const AMQP::Message &message, uint64_t deliveredTag,
		bool redelivered)
	{
		auto key = message.routingkey();
		if (key != client->getRoutingkey())
		{
			channel->reject(deliveredTag, AMQP::requeue);
			return;
		}

		// acknowledge the message
		channel->ack(deliveredTag);

		//get number sent and add one
		auto messageS =
			std::stoi(std::string(message.body(), message.body() + message.bodySize())) + 1;

		std::cout << "message received from " << message.exchange()
			<< " key " << key << " :\"" << messageS << "\"" << std::endl;

		//publish after one second
		channel->publish(client->getExchange(), client->getPublishkey(), std::to_string(messageS));
		usleep(1000000);
	//	std::this_thread::sleep_for(std::chrono::seconds(1));
	};
};


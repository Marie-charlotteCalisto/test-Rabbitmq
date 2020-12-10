#include <amqpcpp.h>
#include "ConnectionHandler.h"

int main()
{
	// create an instance of your own connection handler
	MyConnectionHandler myHandler;

	// create a AMQP connection object
	AMQP::Connection connection(&myHandler, AMQP::Login("guest","guest"), "/");

	// and create a channel
	AMQP::Channel channel(&connection);

	while (true)
	{
	// use the channel object to call the AMQP method you like
	channel.declareExchange("my-exchange", AMQP::fanout);
	channel.declareQueue("my-queue");
	channel.bindQueue("my-exchange", "my-queue", "my-routing-key");
	}
	return 0;
}

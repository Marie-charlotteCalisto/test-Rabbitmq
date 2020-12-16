#include <amqpcpp.h>
#include <amqpcpp/libev.h>
#include "ConnectionHandler.h"

int main()
{

	std::string msg = "hello world";
	std::string queueName = "message_queue";
	std::string exchangeName = "myexchange";
	std::string routingKey = "hello";



	AMQP::Address address("amqp://guest:guest@127.0.0.1:5672");
	MyConnectionHandler myHandler;
	AMQP::Connection connection(&myHandler, address);
//	AMQP::Connection connection(&myHandler, AMQP::Login("guest","guest"));

//while (!connection.ready())
//{
//	std::cout << (connection.fail("")? "has failed" : "didn't fail") << std::endl;
	std::cout << "connection ready ? : "<<(connection.ready()? "true" : "false" )<< std::endl;
	std::cout << "connection waiting ? : "<<(connection.waiting()? "true" : "false" )<< std::endl;
	std::cout << "connection usable ? : " << (connection.usable()? "true" : "false" )<< std::endl;

	connection.heartbeat();
//	while (!channel.ready()){}

	AMQP::Channel channel(&connection);
	channel.declareExchange("my-exchange", AMQP::direct);
	channel.declareQueue("my-queue");
	channel.bindQueue("my-exchange", "my-queue", "my-routing-key");
/*
	channel.declareQueue(queueName);
	channel.declareExchange(exchangeName, AMQP::direct).onSuccess([](){});
	channel.bindQueue(exchangeName, queueName, routingKey);
	channel.publish(exchangeName, routingKey, msg, msg.size());
*/
	std::cout << "channel ready ? : " << (channel.ready()? "true" : "false" )<< std::endl;
	std::cout << "channel usable ? : " << (channel.usable()? "true" : "false" )<< std::endl;

	std::cout << "nb of channel : " <<  connection.channels() << std::endl;
//}
	while(true)
	{}
	return 0;
}
/*


// create an instance of your own connection handler
MyConnectionHandler myHandler;

// create a AMQP connection object
AMQP::Connection connection(&myHandler, AMQP::Login("guest","guest"), "/");

// and create a channel
AMQP::Channel channel(&connection);

std::cout << (channel.ready()? "true" : "false" )<< std::endl;
std::cout << (channel.usable()? "true" : "false" )<< std::endl;



channel.onError([](const char *message){
std::cout << "error channel " << message << std::endl;
});

channel.onReady([](){
std::cout << "is ready" << std::endl;
});


std::cout << "has it worked? "<< std::endl;
// use the channel object to call the AMQP method you like
channel.declareExchange("my-exchange")
.onError([](const char *message){
std::cout << "fail" << message << std::endl;
})
.onSuccess([]() {
std::cout << "succed " << std::endl;
}
);
channel.declareQueue("my-queue");
channel.bindQueue("my-exchange", "my-queue", "my-routing-key");
return 0;
}
*/

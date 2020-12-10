#include <amqpcpp.h>
#include <amqpcpp/libev.h>
#include <ev.h>
#include "ConnectionHandler.h"

int main()
{

	std::string msg = "hello world";
	std::string queueName = "message_queue";
	std::string exchangeName = "myexchange";
	std::string routingKey = "hello";




	//AMQP::Address address("amqp://guest:guest@localhost:5672");
	MyConnectionHandler myHandler;
	//  AMQP::Connection connection(&myHandler, address);
	AMQP::Connection connection(&myHandler, AMQP::Login("guest","guest"), "/");
	AMQP::Channel channel(&connection);


	while (!channel.ready()){}

	channel.declareQueue(queueName);
	channel.declareExchange(exchangeName, AMQP::direct).onSuccess([](){});
	channel.bindQueue(exchangeName, queueName, routingKey);
	channel.publish(exchangeName, routingKey, msg, msg.size());

	std::cout << (channel.ready()? "true" : "false" )<< std::endl;
	std::cout << (channel.usable()? "true" : "false" )<< std::endl;
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

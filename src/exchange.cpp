#include <amqpcpp.h>

///Hummm .... SO MANY ARGUMENTS WTF???
void exchange(AMQP::TcpChannel channel,const std::string exchange,const std::string queue, const std::string routingkey, const std::string message, const std::string queue_responding)
{
	//exchange
	channel.declareExchange(exchange, AMQP::direct);
	channel.bindQueue(exchange, queue, routingkey)
		.onSuccess([]()
				{
				std::cout << "binded"<< std::endl;
				});

	// Define callbacks and start
	auto messageCb = [&channel](
			const AMQP::Message &message, uint64_t deliveryTag, 
			bool redelivered)
	{
			std::cout << "message received :\"" << message.body() << "\"" << std::endl;
			// acknowledge the message
			channel.ack(deliveryTag);

			//time of response
			usleep(1000000);
			channel.publish(my-exchange, routingkey, message);
	};

	// callback function that is called when the consume operation starts
	auto startCb = [](const std::string &consumertag) {

		std::cout << "consume operation started: " << consumertag << std::endl;
	};

	// callback function that is called when the consume operation failed
	auto errorCb = [](const char *message) {

		std::cout << "consume operation failed" << std::endl;
	};

	channel.consume(queue_responding)
		.onReceived(messageCb)
		.onSuccess(startCb)
		.onError(errorCb);

}

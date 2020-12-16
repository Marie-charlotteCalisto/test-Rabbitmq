#include <amqpcpp.h>
#include <amqpcpp/libev.h>
#include <ev.h>

int main()
{
	auto *loop = EV_DEFAULT;

	AMQP::LibEvHandler myhandler(loop);

	AMQP::Address adress("amqp://guest:guest@localhost/");

	AMQP::TcpConnection connection(&myhandler, adress);

	AMQP::TcpChannel channel(&connection);
AMQP::Table arguments;
arguments["x-message-ttl"] = 120 * 1000;

// declare the queue
channel.declareQueue("my-queue", AMQP::durable + AMQP::passive, arguments);

	// Define callbacks and start
	auto messageCb = [&channel](
			const AMQP::Message &message, uint64_t deliveryTag, 
			bool redelivered)
	{
		std::cout << "message received" << std::endl;
		// acknowledge the message
		channel.ack(deliveryTag);
		//processMessage(message.routingkey(), message.body());
	};

	// callback function that is called when the consume operation starts
	auto startCb = [](const std::string &consumertag) {

		std::cout << "consume operation started: " << consumertag << std::endl;
	};

	// callback function that is called when the consume operation failed
	auto errorCb = [](const char *message) {

		std::cout << "consume operation failed" << std::endl;
	};

	channel.consume("my-queue")
		.onReceived(messageCb)
		.onSuccess(startCb)
		.onError(errorCb);

	ev_run(loop, 0);

	return 0;
}

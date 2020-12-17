#include <amqpcpp.h>
#include <amqpcpp/libev.h>
#include <ev.h>
#include <unistd.h>

int main()
{
	//create handler with loop event
	auto *loop = EV_DEFAULT;

	AMQP::LibEvHandler myhandler(loop);

	//connection
	AMQP::Address adress("amqp://guest:guest@localhost/");
	AMQP::TcpConnection connection(&myhandler, adress);

	//channel
	AMQP::TcpChannel channel(&connection);

	//queue
	AMQP::Table arguments;
	arguments["x-message-ttl"] = 120 * 1000;
	channel.declareQueue("second-queue", AMQP::durable + AMQP::passive,arguments)
		.onSuccess([]()
				{
				std::cout << "queue declared" << std::endl;

				});

	//exchange
	channel.declareExchange("my-exchange", AMQP::direct);
	channel.bindQueue("my-exchange", "second-queue", "second")
		.onSuccess([]()
				{
				std::cout << "binded"<< std::endl;
				});

	// Define callbacks and start
	auto messageCb = [&channel](
			const AMQP::Message &message, uint64_t deliveryTag, 
			bool redelivered)
	{
		auto messageS = std::stoi(std::string(message.body(), message.body() + message.bodySize())) + 1;
		std::cout << "message received :\"" << messageS<< "\"" << std::endl;
		// acknowledge the message
		channel.ack(deliveryTag);
		//processMessage(message.routingkey(), message.body());

		usleep(1000000);
		channel.publish("my-exchange", "second", std::to_string(messageS));
	};

	// callback function that is called when the consume operation starts
	auto startCb = [](const std::string &consumertag) {

		std::cout << "consume operation started: " << consumertag << std::endl;
	};

	// callback function that is called when the consume operation failed
	auto errorCb = [](const char *message) {

		std::cout << "consume operation failed" << std::endl;
	};

	channel.consume("first-queue")
		.onReceived(messageCb)
		.onSuccess(startCb)
		.onError(errorCb);

	ev_run(loop, 0);

	return 0;
}

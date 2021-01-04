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
	channel.declareQueue("second-queue")
		.onSuccess([]()
				{
				std::cout << "queue declared" << std::endl;
				});

	//exchange
	channel.declareExchange("my-exchange", AMQP::direct);
	channel.bindQueue("my-exchange", "second-queue", "forth")
		.onSuccess([&channel]()
				{
				std::cout << "binded"<< std::endl;

				//publish first message to begin conversation
				channel.publish("my-exchange", "forth", "0");
				});



	// Define callbacks and start
	auto messageCb = [&channel](
			const AMQP::Message &message, uint64_t deliveryTag, 
			bool redelivered)
	{
		if (message.routingkey() != "third")
			return;

		// acknowledge the message
		channel.ack(deliveryTag);

		//get number sent and add one
		auto messageS = std::stoi(std::string(message.body(), message.body() + message.bodySize())) + 1;

		std::cout << "message received :\"" << messageS << "\"" << std::endl;

		//publish after one second
		usleep(1000000);
		channel.publish("my-exchange", "forth", std::to_string(messageS));
	};

	// callback function that is called when the consume operation starts
	auto startCb = [](const std::string &consumertag) {

		std::cout << "consume operation started: " << consumertag << std::endl;
	};

	// callback function that is called when the consume operation failed
	auto errorCb = [](const char *message) {

		std::cout << "consume operation failed" << std::endl;
	};

	channel.consume("second-queue")
		.onReceived(messageCb)
		.onSuccess(startCb)
		.onError(errorCb);


	ev_run(loop, 0);

	return 0;
}

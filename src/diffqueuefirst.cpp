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
	channel.setQos(1);

	channel.declareQueue("rpc_queue");

	auto callback = [&channel](const AMQP::Message &message,
			uint64_t deliveryTag,
			bool redelivered)
	{
		const auto body = std::string(message.body(), message.body() + message.bodySize());
		//const auto response = std::to_string(std::stoi(body) + 1);

	//	const auto response = std::string(message.body());
		std::cout<< "message : " << body <<std::endl;

		AMQP::Message env(message.exchange(), "first");
		env.setCorrelationID(message.correlationID());

		channel.publish("", message.replyTo(), env);
	};

	channel.consume("", AMQP::noack)
		.onReceived(callback);
	std::cout << " [x] Awaiting RPC requests" << std::endl;
	ev_run(loop, 0);

	return 0;
}

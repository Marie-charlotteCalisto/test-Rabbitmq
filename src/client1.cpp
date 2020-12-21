#include <amqpcpp.h>
#include <amqpcpp/libev.h>
#include <ev.h>
#include <unistd.h>

int main()
{
	//create handler with loop event
	auto *loop = EV_DEFAULT;

	const std::string correlation("1");


	AMQP::LibEvHandler myhandler(loop);

	//connection
	AMQP::Address adress("amqp://guest:guest@localhost/");
	AMQP::TcpConnection connection(&myhandler, adress);

	

	//channel
	AMQP::TcpChannel channel(&connection);

	AMQP::QueueCallback callback = [&](const std::string &name,
			int msgcount,
			int consumercount)
	{
		AMQP::Envelope env("0", 1);
		env.setCorrelationID("2");
		env.setReplyTo(name);
		channel.publish("","rpc_queue", env);
		std::cout<<" [x] Send 0"<<std::endl;

	};
	channel.declareQueue("talk").onSuccess(callback);

	auto receiveCallback = [&](const AMQP::Message &message,
			uint64_t deliveryTag,
			bool redelivered)
	{
		if(message.correlationID() != correlation)
			return;


//		const auto body = std::string(message.body(), message.body() + message.bodySize());
	//	const char *response = std::to_string(std::stoi(body) + 1).c_str();

		usleep(1000000);
		std::cout<<" [.] Got "<< message.body() <<std::endl;
	//	std::cout<<" [.] responded "<< response <<std::endl;
		channel.ack(deliveryTag);

		AMQP::Envelope env(message.body(),message.bodySize());
		env.setCorrelationID("2");
		env.setReplyTo("talk");
		channel.publish("","rpc_queue", env);
	};

	channel.consume("")
		.onReceived(receiveCallback);

	ev_run(loop, 0);

	return 0;
}

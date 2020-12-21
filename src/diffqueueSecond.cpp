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
		AMQP::Message env("talk", "first");
		env.setCorrelationID(correlation);
		env.setReplyTo(name);
		channel.publish("","rpc_queue", env);
		std::cout<<" [x] Request "<<std::endl;

	};
	channel.declareQueue(AMQP::exclusive).onSuccess(callback);

	auto receiveCallback = [&](const AMQP::Message &message,
			uint64_t deliveryTag,
			bool redelivered)
	{
		if(message.correlationID() != correlation)
			return;

		std::cout<<" [.] Got "<<message.body()<<std::endl;
	};

	channel.consume("", AMQP::noack)
		.onReceived(receiveCallback);

	ev_run(loop, 0);

	return 0;
}

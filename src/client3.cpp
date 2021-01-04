#include "callbacks.h"

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
		.onSuccess(queueDeclared);

	//exchange
	channel.declareExchange("my-exchange", AMQP::direct);
	channel.bindQueue("my-exchange", "second-queue", "forth")
		.onSuccess(bindAndSend(&channel, "forth"));

	channel.consume("second-queue")
		.onReceived(AdditionMessage(&channel, "third", "forth"))
		.onSuccess(startCb)
		.onError(errorCb);


	ev_run(loop, 0);

	return 0;
}

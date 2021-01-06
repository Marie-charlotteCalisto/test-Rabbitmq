#include "callbacks.h"
#include "client.h"

void Client::RespondAdditionMessage(Client client)
{
	struct ev_loop *loop = ev_loop_new();
	AMQP::LibEvHandler myhandler(loop);


	AMQP::Address adress("amqp://guest:guest@localhost/");
	AMQP::TcpConnection connection(&myhandler, adress);
	AMQP::TcpChannel channel(&connection);


	channel.consume(client.getQueue())
		.onReceived(AdditionMessage(&channel, &client))
		.onSuccess(startCb)
		.onError(errorCb);

	ev_run(loop);
};


#include "callbacks.h"
#include "client.h"

void Client::SendAdditionMessage(AMQP::TcpChannel *channel)
{
	channel->consume(this->queue)
		.onReceived(AdditionMessage(channel, this))
		.onSuccess(startCb)
		.onError(errorCb);

};


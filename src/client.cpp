#include "callbacks.h"
#include "client.h"

void Client::RespondAdditionMessage(AMQP::TcpChannel *channel)
{

	channel->consume(this->queue)//, AMQP::nolocal)
		.onReceived(AdditionMessage(channel, this))
		.onSuccess(startCb)
		.onError(errorCb);

};


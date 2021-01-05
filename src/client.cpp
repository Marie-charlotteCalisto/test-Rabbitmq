#include "callbacks.h"
#include "client.h"

void Client::ClientAsk(AMQP::TcpChannel *channel)
{
	/*
	//queue
	channel->declareQueue(this->queue)
		.onSuccess(queueDeclared);
*/
	//exchange
	//channel->declareExchange(this->exchange, AMQP::direct);
	channel->bindQueue(this->exchange, this->queue, this->publishkey)
		.onSuccess(bindAndSend(channel, this->publishkey));

	channel->bindQueue(this->exchange, this->queue, this->routingkey);

	channel->consume(this->queue)
		.onReceived(AdditionMessage(channel, this->routingkey, this->publishkey))
		.onSuccess(startCb)
		.onError(errorCb);


};

void Client::ClientRespond(AMQP::TcpChannel *channel)
{
	/*
	//queue
	channel->declareQueue(this->queue)
		.onSuccess(queueDeclared);
*/
	//exchange
//	channel->declareExchange(this->exchange, AMQP::direct);
	channel->bindQueue(this->exchange, this->queue, this->publishkey)
		.onSuccess(binded);


	channel->consume(this->queue)
		.onReceived(AdditionMessage(channel, this->routingkey, this->publishkey))
		.onSuccess(startCb)
		.onError(errorCb);

};


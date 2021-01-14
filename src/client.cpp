#include "callbacks.h"
#include "client.h"

void Client::RespondAdditionMessage(Client client)
{
    struct ev_loop *loop = ev_loop_new();
    AMQP::LibEvHandler myhandler(loop);


    AMQP::Address adress("amqp://guest:guest@localhost/");
    AMQP::TcpConnection connection(&myhandler, adress);
    AMQP::TcpChannel channel(&connection);

    //queue
    channel.declareQueue(client.getQueue())
        .onSuccess(queueDeclared);

    //exchange
    channel.declareExchange(client.getExchange(), AMQP::direct);
    channel.bindQueue(client.getExchange(), client.getQueue(), client.getRoutingkey())
        .onSuccess(binded);

    //consume
    channel.consume(client.getQueue())
        .onReceived(AdditionMessage(&channel, &client))
        .onSuccess(startCb)
        .onError(errorCb);

    ev_run(loop);
};

void Client::publish(std::string message)
{
    struct ev_loop *loop = ev_loop_new();
    AMQP::LibEvHandler myhandler(loop);

    AMQP::Address adress("amqp://guest:guest@localhost/");
    AMQP::TcpConnection connection(&myhandler, adress);
    AMQP::TcpChannel channel(&connection);

    //queue
    channel.declareQueue(queue);
    //.onSuccess(queueDeclared);

    //exchange
    channel.declareExchange(exchange, AMQP::direct);
    channel.bindQueue(exchange, queue, publishkey)
        .onSuccess([this, &channel, message, loop] ()
                {
                channel.publish(exchange, publishkey, message);
                std::cout << "published : " << message << std::endl;
                ev_break (loop, EVBREAK_ALL);
                });


    ev_run(loop);
}

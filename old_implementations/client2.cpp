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
    channel.declareQueue("my-queue")
        .onSuccess(queueDeclared);

    //exchange
    channel.declareExchange("my-exchange", AMQP::direct);
    channel.bindQueue("my-exchange", "my-queue", "first")
        .onSuccess(binded);

    channel.consume("my-queue")
        .onReceived(AdditionMessage(&channel, "second", "first"))
        .onSuccess(startCb)
        .onError(errorCb);

    ev_run(loop, 0);

    return 0;
}

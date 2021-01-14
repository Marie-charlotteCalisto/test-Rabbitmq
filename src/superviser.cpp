#include "callbacks.h"
#include "client.h"

#include <vector>
#include <iostream>
#include <string>
#include <curl/curl.h>
#include <json/json.h>
#include <unistd.h>
#include <map>
#include "thread"


void supervisor(std::vector<std::string> queues)
{
    while (true)
    {
        auto *loop = ev_loop_new();

        //update number of consumers every 5 seconds

        AMQP::LibEvHandler myhandler(loop);

        AMQP::Address adress("amqp://guest:guest@localhost/");
        AMQP::TcpConnection connection(&myhandler, adress);
        AMQP::TcpChannel channel(&connection);

        for (auto queue : queues) 
        {
            channel.declareQueue(queue)
                .onSuccess(displayNbConsumer(&connection, queue)); //close connection
        }
        std::this_thread::sleep_for(std::chrono::seconds(5));
        ev_run(loop, 0);
    }

}

void init_all(std::vector<std::string> queues, std::vector<std::string> exchanges)
{
    struct ev_loop *loop = ev_loop_new();
    AMQP::LibEvHandler myhandler(loop);

    AMQP::Address adress("amqp://guest:guest@localhost/");
    AMQP::TcpConnection connection(&myhandler, adress);
    AMQP::TcpChannel channel(&connection);

    for (auto queue : queues)
    {
        channel.declareQueue(queue);
    }


    for (auto exchange : exchanges)
    {
        channel.declareExchange(exchange, AMQP::direct);
    }
    connection.close();

    ev_run(loop);

}


int main(void)
{
    std::string queue1 = "my-queue";
    std::string exchange = "my-exchange";
    std::string queue2 = "my-queue2";


    std::vector<std::string> queues {queue1, queue2};
    std::vector<std::string> exchanges{exchange};

    std::thread sup(supervisor, queues);

    //first discution
    auto client1 = Client(queue1, exchange, "first", "second");
    auto client2 = Client(queue1, exchange, "second", "first");

    //second discution
    auto client3 = Client(queue2, exchange, "third", "forth");
    auto client4 = Client(queue2, exchange, "forth", "third");

    client1.publish("0");
    client3.publish("0");


    std::thread clientThr3(Client::RespondAdditionMessage, client3);
    std::thread clientThr4(Client::RespondAdditionMessage, client4);

    std::thread clientThr1(Client::RespondAdditionMessage, client1);
    std::thread clientThr2(Client::RespondAdditionMessage, client2);
    sup.join();

    return 0;
}               

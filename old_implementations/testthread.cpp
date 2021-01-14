#include <amqpcpp.h>
#include <amqpcpp/libev.h>
#include "thread"
#include "chrono"



struct ev_loop* loop1 = ev_loop_new();
struct ev_loop* loop2 = ev_loop_new();


void publisher() {
    std::string host = "amqp://localhost/";
    AMQP::LibEvHandler* handler = new AMQP::LibEvHandler(loop1);
    AMQP::TcpConnection* conn = new AMQP::TcpConnection(handler, AMQP::Address(host));
    AMQP::TcpChannel* chan = new AMQP::TcpChannel(conn);

    auto startCb = [](const std::string&consumertag) {
        std::cout << "Consumer operation started" << std::endl;
    };

    auto errorCb = [](const char *message) {
        std::cout << message << std::endl;
        std::cout << "Consume operation failed" << std::endl;
    };

    auto messageCb = [chan](const AMQP::Message &message, uint64_t deliveryTag, bool redelivered) {
        auto key = message.routingkey();
        if (key != "first")
        {
            chan->reject(deliveryTag, AMQP::requeue);
            return;
        }
        // acknowledge the message
        chan->ack(deliveryTag);

        //get number sent and add one
        auto messageS =
            std::stoi(std::string(message.body(), message.body() + message.bodySize())) + 1;

        std::cout << "message received from " << message.exchange()
            << " key " << key << " :\"" << messageS << "\"" << std::endl;

        //publish after one second
        chan->publish("exchange", "second", std::to_string(messageS));
        std::this_thread::sleep_for(std::chrono::seconds(1));
    };	



    chan->consume("queue")
        .onReceived(messageCb)
        .onSuccess(startCb)
        .onError(errorCb);


    ev_run(loop1);
};

void subscriber() {
    std::string host = "amqp://localhost/";
    AMQP::LibEvHandler* handler = new AMQP::LibEvHandler(loop2);
    AMQP::TcpConnection* conn = new AMQP::TcpConnection(handler, AMQP::Address(host));
    AMQP::TcpChannel* chan = new AMQP::TcpChannel(conn);

    auto startCb = [](const std::string&consumertag) {
        std::cout << "Consumer operation started" << std::endl;
    };

    auto errorCb = [](const char *message) {
        std::cout << message << std::endl;
        std::cout << "Consume operation failed" << std::endl;
    };

    auto messageCb = [chan](const AMQP::Message &message, uint64_t deliveryTag, bool redelivered) {
        auto key = message.routingkey();
        if (key != "second")
        {
            chan->reject(deliveryTag, AMQP::requeue);
            return;
        }
        // acknowledge the message
        chan->ack(deliveryTag);

        //get number sent and add one
        auto messageS =
            std::stoi(std::string(message.body(), message.body() + message.bodySize())) + 1;

        std::cout << "message received from " << message.exchange()
            << " key " << key << " :\"" << messageS << "\"" << std::endl;

        //publish after one second
        chan->publish("exchange", "first", std::to_string(messageS));
        std::this_thread::sleep_for(std::chrono::seconds(1));
    };	

    chan->consume("queue")
        .onReceived(messageCb)
        .onSuccess(startCb)
        .onError(errorCb);

    ev_run(loop2);
}
/*
   int main() {
   std::thread sub(subscriber);
   std::thread pub(publisher);

   pub.join();
   sub.join();
   }i*/
#include <iostream>
#include <thread>
#include <chrono>

void foo()
{
    // simulate expensive operation
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "starting first helper...\n";
}

void bar()
{
    // simulate expensive operation
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << "starting second helper...\n";
}

int main()
{

    std::thread helper1(foo);


    std::thread helper2(bar);

    std::cout << "waiting for helpers to finish..." << std::endl;
    helper1.join();
    helper2.join();

    std::cout << "done!\n";
}

#include <amqpcpp.h>
#include <amqpcpp/libev.h>
#include <ev.h>
#include <unistd.h>


class Client {
	//private:
	public:
		std::string queue;
		std::string exchange;
		std::string routingkey;
		std::string publishkey;

	public:
		Client(std::string queue, std::string exchange, std::string routingkey, std::string publishkey)
		{
			this->queue = queue;
			this->exchange = exchange;
			this->routingkey = routingkey;
			this->publishkey = publishkey;
		};


		void RespondAdditionMessage(AMQP::TcpChannel *channel);

		std::string getExchange() { return exchange;}
		std::string getRoutingkey() { return routingkey;}
		std::string getPublishkey() { return publishkey;}
};

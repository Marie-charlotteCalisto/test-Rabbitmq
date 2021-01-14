#include <amqpcpp.h>
#include <amqpcpp/libev.h>
#include <ev.h>
#include <unistd.h>


class Client {
	private:
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


		static void RespondAdditionMessage(Client client);
		void publish(std::string message);

		std::string getQueue() { return queue;}
		std::string getExchange() { return exchange;}
		std::string getRoutingkey() { return routingkey;}
		std::string getPublishkey() { return publishkey;}
};

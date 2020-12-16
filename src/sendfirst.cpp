#include <amqpcpp.h>
#include <amqpcpp/libev.h>
#include <ev.h>

int main()
{
	auto *loop = EV_DEFAULT;

	AMQP::LibEvHandler myhandler(loop);

	AMQP::Address adress("amqp://guest:guest@localhost/");

	AMQP::TcpConnection connection(&myhandler, adress);

	AMQP::TcpChannel channel(&connection);

	channel.declareExchange("my-exchange", AMQP::direct);
	channel.declareQueue("my-queue")
		.onSuccess([]()
				{
				std::cout << "queue declared : my-queue" << std::endl;

				});


	channel.bindQueue("my-exchange", "my-queue", "my-key")
		.onSuccess([]()
				{
				std::cout << "binded"<< std::endl;
				});
	channel.publish("my-exchange", "my-key", "hello world");
/*				
	channel.consume("my-exchange");

	channel.get("my-exchange")
		.onSuccess([](const AMQP::Message &message, uint64_t deliveryTag, bool redelivered)
				{
				std::cout << "Message feteched" << std::endl;
				})
	.onEmpty([]()
			{
			std::cout << "queue is empty" << std::endl;
			});
*/
	ev_run(loop, 0);

	return 0;
}

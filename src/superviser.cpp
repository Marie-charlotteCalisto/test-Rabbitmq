#include "callbacks.h"
#include "client.h"

#include <iostream>
#include <string>
#include <curl/curl.h>
#include <json/json.h>
#include <unistd.h>
#include <map>
#include "thread"


static size_t WriteCallback(
		const char* in,
		std::size_t size,
		std::size_t num,
		char* out)
{
	std::string data(in, (std::size_t) size * num);
	*((std::stringstream*) out) << data;
	return size * num;        
}

Json::Value getJsonConsumers(CURL *curl, std::map<std::string, size_t> *queues)
{
	CURLcode res;
	std::string readBuffer;

	long httpCode(0);

	std::stringstream httpData;

	//Set url for consumer list
	curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:15672/api/consumers");
	curl_easy_setopt(curl, CURLOPT_USERPWD, "guest:guest");

	//write data to stringstream (to be parse to json)
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &httpData);

	res = curl_easy_perform(curl);

	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

	if (res == CURLE_OK && httpCode == 200)
	{
		//Get json data from string
		Json::Value jsonData;
		Json::CharReaderBuilder jsonReader;
		std::string err;

		if (Json::parseFromStream(jsonReader, httpData, &jsonData, &err))
		{
			queues->clear();
			(*queues)["my-queue"] = 0;
			(*queues)["second-queue"] = 0;

			for (int i = 0; i < jsonData.size(); i++)
			{
				std::string queueName = jsonData[i]["queue"]["name"].asString();
				(*queues)[queueName] += 1;
			}
			return jsonData;
		}
	} 
	return Json::Value::null;

}

void purgeQueue(std::string queueName)
{
	auto curl = curl_easy_init();


	if (curl)
	{
		std::string url = "http://localhost:15672/api/queues/%2F/" + queueName + "/contents";

		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_USERPWD, "guest:guest");

		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
	
		//For debug:
		//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);


		auto res = curl_easy_perform(curl);
		if(res != CURLE_OK)
			std::cout << "curl_easy_perform() failed "<< curl_easy_strerror(res) << std::endl;
		else
			std::cout << "purged" << std::endl;
		
		curl_easy_cleanup(curl);
	}
	usleep(100);
}

void declareQueue(std::string queue)
{
	auto curl = curl_easy_init();


	std::string data = "{\"name\":\"" + queue + "\",\
			\"auto_delete\":\"true\",\
			\"node\":\"guest@localhost\"}";
	if (curl)
	{
		std::string url = "http://localhost:15672/api/queues/%2F/" + queue;

		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_USERPWD, "guest:guest");

		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");

		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
			
		//For debug:
		//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);


		auto res = curl_easy_perform(curl);

		if(res != CURLE_OK)
			std::cout << res << "curl_easy_perform() failed "<< curl_easy_strerror(res) << std::endl;
		else
			std::cout << "queue declared : " << queue << std::endl;

		curl_easy_cleanup(curl);
	}

}

void declareExchange(std::string exchange, std::string type)
{
	auto curl = curl_easy_init();

	std::string data = "{\"name\":\"" + exchange + "\",\"type\":\"" + type + "\"}";

	if (curl)
	{
		std::string url = "http://localhost:15672/api/exchanges/%2F/" + exchange;

		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_USERPWD, "guest:guest");

		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");

		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
		
		//For debug:
		//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);



		auto res = curl_easy_perform(curl);

		if(res != CURLE_OK)
			std::cout << res << "curl_easy_perform() failed "<< curl_easy_strerror(res) << std::endl;
		else
			std::cout << "Exchange declared : " << exchange << std::endl;


		curl_easy_cleanup(curl);
	}
}

void bindExchangeAndQueue(std::string exchange, std::string queue, std::string routingkey)
{

	auto curl = curl_easy_init();

	std::string data = "{\"routing_key\":\"" + routingkey + "\"}";
	if (curl)
	{
		std::string url = "http://localhost:15672/api/bindings/%2F/e/" + exchange + "/q/" + queue;

		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_USERPWD, "guest:guest");

		curl_easy_setopt(curl, CURLOPT_POST, 1L);

		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());

		//For debug:
		//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);


		auto res = curl_easy_perform(curl);

		if(res != CURLE_OK)
			std::cout << res << "curl_easy_perform() failed "<< curl_easy_strerror(res) << std::endl;
		else
			std::cout << "Exchange and Queue binded : " << exchange << " " << queue << " " << routingkey << std::endl;

		curl_easy_cleanup(curl);
	}
}

void publishMessage(std::string exchange, std::string publishkey)
{

	auto curl = curl_easy_init();

	std::string data = "{\"properties\":{},\"routing_key\":\"" + publishkey + "\",\"payload\":\"0\",\"payload_encoding\":\"string\"}";
	if (curl)
	{
		std::string url = "http://localhost:15672/api/exchanges/%2F/" + exchange + "/publish";

		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_USERPWD, "guest:guest");

		curl_easy_setopt(curl, CURLOPT_POST, 1L);

		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());

		//For debug:
	//	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);


		auto res = curl_easy_perform(curl);

		
		if(res != CURLE_OK)
			std::cout << res << "curl_easy_perform() failed "<< curl_easy_strerror(res) << std::endl;
		else
			std::cout << " published" << std::endl;

		curl_easy_cleanup(curl);
	}

}


int main(void)
{

	std::string queue = "my-queue";
	std::string exchange = "my-exchange";

	declareQueue(queue);
	purgeQueue(queue);

	declareExchange(exchange, "direct");
	bindExchangeAndQueue(exchange, queue, "first");
	bindExchangeAndQueue(exchange, queue, "second");

	auto client1 = Client("my-queue", "my-exchange", "first", "second");
        auto client2 = Client("my-queue", "my-exchange", "second", "first");

/*	std::thread client1(Client, "my-queue", "my-exchange", );
	std::thread client2(client1);

	client1.join();
	client2.join();

*/
	std::thread pub(publishMessage, exchange, "first");

	pub.join();

	struct ev_loop *loop = EV_DEFAULT;
                                                                    
	AMQP::LibEvHandler myhandler(loop);


	//client1

        AMQP::Address adress1("amqp://guest:guest@localhost/");        
        AMQP::TcpConnection connection1(&myhandler, adress1);
        AMQP::TcpChannel channel1(&connection1);                        

	client1.RespondAdditionMessage(&channel1);
         


       	//client2

        AMQP::Address adress2("amqp://guest:guest@localhost/");        
        AMQP::TcpConnection connection2(&myhandler, adress2);           
        AMQP::TcpChannel channel2(&connection2);                        

	client2.RespondAdditionMessage(&channel2);

	
	                       
	ev_run(loop, 0);

	return 0;
}
/*
 	CURL *curl;
	size_t nbConsumers = 0;
	std::map<std::string, size_t> queues;
	//while (true)
	//{
		curl = curl_easy_init();
		if(curl) {
			auto JsonConsumers = getJsonConsumers(curl, &queues);
			auto NewnbConsumers = JsonConsumers.size();
			if (nbConsumers != NewnbConsumers)
			{
				if (nbConsumers > NewnbConsumers)
				{
					for (auto it = queues.begin(); it != queues.end(); it++)
					{
						if (it->second == 0)
						{
							std::cout << it->first << " " << it->second << std::endl;
							purgeQueue(it->first);
						}
					}

				}
				nbConsumers = NewnbConsumers;
				std::cout << "There is " << nbConsumers << " consumer" << std::endl;
			}

			curl_easy_cleanup(curl);
		}

	//	usleep(1000000);
	return 0;
}
*/

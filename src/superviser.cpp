#include "callbacks.h"
#include "client.h"

#include <iostream>
#include <string>
#include <curl/curl.h>
#include <json/json.h>
#include <unistd.h>
#include <map>


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

		/* Perform the custom request */ 
		auto res = curl_easy_perform(curl);

		/* Check for errors */ 
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


	if (curl)
	{
		std::string url = "http://localhost:15672/api/queues/%2F/" + queue;

		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_USERPWD, "guest:guest");

		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");

		//construct json spec
		std::stringstream httpData;
		httpData << "{\"name\":\"" << queue <<"\",\
			\"auto_delete\":\"true\",\
			\"node\":\"guest@localhost\"}";

		Json::Value json;
		Json::CharReaderBuilder jsonReader;
		std::string err;

		Json::parseFromStream(jsonReader, httpData, &json, &err);
		
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json);
		
		
		/* Perform the custom request */ 
		auto res = curl_easy_perform(curl);

		/* Check for errors */ 
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


	if (curl)
	{
		std::string url = "http://localhost:15672/api/exchanges/%2F/" + exchange;

		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_USERPWD, "guest:guest");

		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");

		//construct json spec
		std::stringstream httpData;
		httpData << "{\"name\":\"" << exchange << "\"\
			,\"type\":\"" << type << "\"}";

		Json::Value json;
		Json::CharReaderBuilder jsonReader;
		std::string err;

		Json::parseFromStream(jsonReader, httpData, &json, &err);
		
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json);
		
		
		/* Perform the custom request */ 
		auto res = curl_easy_perform(curl);

		/* Check for errors */ 
		if(res != CURLE_OK)
			std::cout << res << "curl_easy_perform() failed "<< curl_easy_strerror(res) << std::endl;
		else
			std::cout << "exchange declared : " << exchange << std::endl;

		curl_easy_cleanup(curl);
	}

}

void bindExchangeAndQueue(std::string exchange, std::string queue, std::string routingkey)
{

	curl_global_init(CURL_GLOBAL_ALL);
	auto curl = curl_easy_init();

	if (curl)
	{
		std::string url = "http://localhost:15672/api/bindings/%2F/e/" + exchange + "/q/" + queue;

		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_USERPWD, "guest:guest");


		//construct json spec
		std::stringstream httpData;

		//httpData << "[{\"source\":\"my-exchange\",\"vhost\":\"/\",\"destination\":\"my-queue\",\"destination_type\":\"queue\",\"routing_key\":\""<< routingkey <<"\",\"arguments\":{},\"properties_key\":\"~\"}]";
		httpData << "{\"routing_key\":\"" << routingkey <<"\"\
			,\"arguments\":\"{\"x-arg\": \"value\"}\"}";

		Json::Value json;
		Json::CharReaderBuilder jsonReader;
		std::string err;

		Json::parseFromStream(jsonReader, httpData, &json, &err);
		
	//	curl_easy_setopt(curl, CURLOPT_POST, 1);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json);
		
		
		/* Perform the custom request */ 
		auto res = curl_easy_perform(curl);

		/* Check for errors */ 
		if(res != CURLE_OK)
			std::cout << res << "curl_easy_perform() failed "<< curl_easy_strerror(res) << std::endl;
		else
			std::cout << "binded : " << exchange << " " << queue << " " << routingkey << std::endl;

		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();
}



int main(void)
{

	std::string queue = "my-queue";
	std::string exchange = "my-exchange";

	declareQueue(queue);
	purgeQueue(queue);

	declareExchange(exchange, "direct");
//	bindExchangeAndQueue(exchange, queue, "first");
//	bindExchangeAndQueue(exchange, queue, "second");



	CURL *curl;
	size_t nbConsumers = 0;

	std::map<std::string, size_t> queues;
	
	auto client1 = Client("my-queue", "my-exchange", "first", "second");
        auto client2 = Client("my-queue", "my-exchange", "second", "first");

	struct ev_loop *loop = EV_DEFAULT;
                                                                    
	AMQP::LibEvHandler myhandler(loop);
       	//client2
        AMQP::Address adress2("amqp://guest:guest@localhost/");        
        AMQP::TcpConnection connection2(&myhandler, adress2);           
        AMQP::TcpChannel channel2(&connection2);                        

	client2.ClientRespond(&channel2);

                                          
	//client1
        AMQP::Address adress1("amqp://guest:guest@localhost/");        
        AMQP::TcpConnection connection1(&myhandler, adress1);
        AMQP::TcpChannel channel1(&connection1);                        

	client1.ClientAsk(&channel1);
                                
	ev_run(loop, 0);
	return 0;
}
/*
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

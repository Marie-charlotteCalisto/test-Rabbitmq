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
		curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:15672/api/queues/%2F/my-queue/contents");
		curl_easy_setopt(curl, CURLOPT_USERPWD, "guest:guest");

		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");

		/* Perform the custom request */ 
		auto res = curl_easy_perform(curl);

		/* Check for errors */ 
		if(res != CURLE_OK)
			std::cout << "curl_easy_perform() failed "<< curl_easy_strerror(res) << std::endl;
		else
			std::cout << "hey " << std::endl;

		curl_easy_cleanup(curl);
	}
}

int main(void)
{
	CURL *curl;
	size_t nbConsumers = 0;

	std::map<std::string, size_t> queues;

	while (true)
	{
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

		usleep(1000000);
	}
	return 0;
}

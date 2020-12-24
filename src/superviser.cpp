#include <iostream>
#include <string>
#include <curl/curl.h>
#include <json/json.h>


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

std::string getNbChannel(CURL *curl)
{
	CURLcode res;
	std::string readBuffer;

	long httpCode(0);

	std::stringstream httpData;

	curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:15672/api/consumers");

	curl_easy_setopt(curl, CURLOPT_USERPWD, "guest:guest");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &httpData);

	res = curl_easy_perform(curl);
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

	if (res == CURLE_OK && httpCode == 200)
	{
		Json::Value jsonData;
		Json::CharReaderBuilder jsonReader;
		std::string err;

		if (Json::parseFromStream(jsonReader, httpData, &jsonData, &err))
		{
			return std::to_string(jsonData.size());
		}
	} 
	return "0";

}

int main(void)
{
	CURL *curl;
	std::string readBuffer;

	while (true)
	{
		curl = curl_easy_init();
		if(curl) {
			auto nbChannel = getNbChannel(curl);
			if (readBuffer != nbChannel)
			{
				readBuffer = nbChannel;
				std::cout << "There is " << readBuffer << " consumer" << std::endl;
			}
			curl_easy_cleanup(curl);
		}
	}
	return 0;
}

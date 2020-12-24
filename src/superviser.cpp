#include <iostream>
#include <string>
#include <curl/curl.h>


static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}


std::string getNbChannel(CURL *curl)
{
	CURLcode res;
	std::string readBuffer;

	curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:15672/api/consumers");
	curl_easy_setopt(curl, CURLOPT_USERPWD, "guest:guest");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
	res = curl_easy_perform(curl);


	return readBuffer;

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
				std::cout << readBuffer << std::endl;
			}
			curl_easy_cleanup(curl);
		}
	}
	return 0;
}

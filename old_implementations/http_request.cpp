/** This is an old implementation for http request using curl
 *
 */



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

/**
 * \brief get json consumer with curlo
 *
 * TODO: errors not mannaged
 *
 * \param queue : map of queues to be ubdated with number of consumers 
 *
 * \return the json consumer value 
 */
Json::Value getJsonConsumers(std::map<std::string, size_t> *queues)
{

    auto curl = curl_easy_init();
    if (!curl)
        return Json::Value::null;

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
            //update queue
            for (auto it = queue->begin(); it != queue->end(); it++)
                it->second = 0;

            for (int i = 0; i < jsonData.size(); i++)
            {
                std::string queueName = jsonData[i]["queue"]["name"].asString();
                (*queues)[queueName] += 1;
            }
            curl_easy_cleanup(curl);
            return jsonData;
        }
    } 
    curl_easy_cleanup(curl);
    return Json::Value::null;

}

/**
 * \brief purge a queue with curl
 *
 * \param queueName : the queue to be purged
 */
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

/**
 * \brief declare a queue with curl, channel not needed
 *
 * \param queue : name of the queue to be declared
 */
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


/**
 * \brief declare exchange with curl, no channel needed
 *
 * \param exchange : name of exchange to be decalared
 * \param type : type of the exchange
 */
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


/**
 * \brief bind exchange and queue with routing key using curl
 *
 * \param exchange : name of the exchange to be binded
 * \param queue : name of the queue to be binded
 * \param routingkey : name of the key to bind exchange and queue
 */
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


/**
 * \breif publish a message with curl
 *
 * \param exchange : the exchange to publish in
 * \param publishkey : the key to publish the message with
 * \param message : the message to be published
 */
void publishMessage(std::string exchange, std::string publishkey, std::string message)
{

    auto curl = curl_easy_init();

    std::string data = "{\"properties\":{},\"routing_key\":\"" + publishkey + "\",\"payload\":\"" + message + "\",\"payload_encoding\":\"string\"}";
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


/**
 * \brief supervisor that display number of consumers by queue
 * 
 * client 1, 2, 3 and 4 must be executed for this code to have meaning
 */
void main()
{
    //init all with curl
    std::string queue = "my-queue";
    std::string exchange = "my-exchange";

    declareQueue(queue);
    purgeQueue(queue);

    declareExchange(exchange, "direct");
    bindExchangeAndQueue(exchange, queue, "first");
    bindExchangeAndQueue(exchange, queue, "second");

    std::string queue2 = "my-queue2";
    std::string exchange2 = "my-exchange2";

    declareQueue(queue2);
    purgeQueue(queue2);

    declareExchange(exchange2, "direct");
    bindExchangeAndQueue(exchange2, queue2, "third");
    bindExchangeAndQueue(exchange2, queue2, "forth");

    //display consumers by queue with curl
    CURL *curl;
    size_t nbConsumers = 0;
    std::map<std::string, size_t> queues;

    queue[queue] = 0;
    queue[queue2] = 0;

    while (true)
    {
        curl = curl_easy_init();
        if(curl) {
            auto JsonConsumers = getJsonConsumers(&queues);
            auto NewnbConsumers = JsonConsumers.size();
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
            if (nbConsumers != NewnbConsumers)
                nbConsumers = NewnbConsumers;
            std::cout << "There is " << nbConsumers << " consumer" << std::endl;
        }

        curl_easy_cleanup(curl);

        usleep(1000000);
    }
    return 0;
}

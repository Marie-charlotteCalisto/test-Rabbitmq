# PocRabbitmq
Projet Test Rabbitmq with lib AMQP-CPP.
Make connection and messages exchange between 2 executable.

Download AMQP-CPP lib from https://github.com/CopernicaMarketingSoftware/AMQP-CPP

Check the install path of the lib. It will depend on your configuration. 
Mine had to be in path : /usr/lib and not /usr/local/lib
This project is compiled on manjaro. It will be different on Windows.
On Windows the lib path will have to be set in the Environemnt Variables.


```
mkdir build; cd build
cmake ..
make
```

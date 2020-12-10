# PocRabbitmq
Projet Test Rabbitmq with lib AMQP-CPP.\
Make connection and messages exchange between 2 executable.


http://www.lib4dev.in/info/CopernicaMarketingSoftware/AMQP-CPP/15631079

Download AMQP-CPP lib from https://github.com/CopernicaMarketingSoftware/AMQP-CPP

To install the lib on linux or on windows (see link above), this line :
```
cmake --build . --target install
```
has installed the lib on /usr/local/(include and lib) for Linux\
and on C:/Program File (x86)/amqpcpp/(include and lib) for Windows

Check the install path of the lib. It will depend on your configuration.\
Mine had to be in path : /usr/ and not /usr/local/\
This project is compiled on manjaro. It will be different on Windows.\
On Windows the lib path will have to be set in the Environemnt Variables.

To compile this project :
```
mkdir build; cd build
cmake ..
make
```

To run the two exe : 
```
./amqpcpp1
./amqpcpp2
```

https://stackoverflow.com/questions/41805270/rabbitmq-c-client-not-connecting-to-rabbitmq-server-at-5672-port

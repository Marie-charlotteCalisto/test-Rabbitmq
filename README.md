# PocRabbitmq
Projet Test Rabbitmq with lib AMQP-CPP.\
Make connection and messages exchange between executables.


http://www.lib4dev.in/info/CopernicaMarketingSoftware/AMQP-CPP/15631079

Download AMQP-CPP lib from https://github.com/CopernicaMarketingSoftware/AMQP-CPP

To install the lib on linux or on windows (see link above).
This line :
```
cmake --build . --target install
```
has installed the lib on /usr/local/(include and lib) for Linux\
and on C:/Program File (x86)/amqpcpp/(include and lib) for Windows

Check the install path of the lib. It will depend on your configuration.\
Mine had to be in path : /usr/ and not /usr/local/\
This project is compiled on manjaro. It will be different on Windows.\
On Windows the lib path will have to be set in the Environemnt Variables.

Some of the rabbitmq-plugins must be enable : \
rabbitmq_amqp1_0 \
rabbitmq_event_exchange\
rabbitmq_management\
```
rabbitmq-plugins enable <plugin_name>
```
Download the rabbitmqadmin file : https://www.rabbitmq.com/management-cli.html \
The file must be executable so ``` chmod +x rabbitmqadmin ``` \
I needed to change the  as follow : #!/usr/bin/pyton3 instead of #!/usr/bin/env python3\
And move the exe to /usr/bin/. Here again, this depends on the linux distrubution.

To compile this project :
```
mkdir build; cd build
cmake ..
make
```

To run the two exe : 
```
./client2
./client1
```

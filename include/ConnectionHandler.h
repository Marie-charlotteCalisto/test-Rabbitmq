#include <amqpcpp.h>

class MyConnectionHandler : public AMQP::ConnectionHandler
{

	/**
	 *  When the connection is being set up, the client and server exchange
	 *  some information. This includes for example their name and version, 
	 *  copyright statement and the operating system name. Nothing in this 
	 *  exchange of information is very relevant for the actual AMQP protocol, 
	 *  but by overriding this method you can read out the information that 
	 *  was sent by the server, and you can decide which information you 
	 *  want to send back that describe the client. In RabbitMQ's management 
	 *  console the client-properties are visible on the "connections" tab, 
	 *  which could be helpful in certain scenarios, like debugging.
	 * 
	 *  The read-only "server" parameter contains the information sent by 
	 *  the server, while the "client" table may be filled with information
	 *  about your application. The AMQP protocol says that this table should
	 *  at least be filled with data for the "product", "version", "platform", 
	 *  "copyright" and "information" keys. However, you do not have to 
	 *  override this method, and even when you do, you do not have to ensure 
	 *  that these properties are indeed set, because the AMQP-CPP library 
	 *  takes care of filling in properties that were not explicitly set.
	 * 
	 *  @param  connection      The connection about which information is exchanged
	 *  @param  server          Properties sent by the server
	 *  @param  client          Properties that are to be sent back
	 */
	virtual void onProperties(AMQP::Connection *connection, const AMQP::Table &server, AMQP::Table &client) override
	{
		std::cout << "connection ready onProperties ? : "<< (connection->ready()? "true" : "false") << std::endl;
		std::cout << "server : ";
		server.output(std::cout);
		std::cout << "\nclient : ";
		client.output(std::cout);
		std::cout << std::endl;
	}



	/**
	 *  Method that is called by the AMQP library every time it has data
	 *  available that should be sent to RabbitMQ.
	 *  @param  connection  pointer to the main connection object
	 *  @param  data        memory buffer with the data that should be sent to RabbitMQ
	 *  @param  size        size of the buffer
	 */
	virtual void onData(AMQP::Connection *connection, const char *data, size_t size) override
	{
		std::cout << "On Data : \"" << data << "\"" << std::endl;
		// @todo
		//  Add your own implementation, for example by doing a call to the
		//  send() system call. But be aware that the send() call may not
		//  send all data at once, so you also need to take care of buffering
		//  the bytes that could not immediately be sent, and try to send
		//  them again when the socket becomes writable again
	}	
	/**
	 *  Method that is called by the AMQP library when the login attempt
	 *  succeeded. After this method has been called, the connection is ready
	 *  to use.
	 *  @param  connection      The connection that can now be used
	 */
	virtual void onReady(AMQP::Connection *connection) override
	{
		std::cout << "Connection established " << std::endl;
		// @todo
		//  add your own implementation, for example by creating a channel
		//  instance, and start publishing or consuming
	}

	/**
	 *  Method that is called by the AMQP library when a fatal error occurs
	 *  on the connection, for example because data received from RabbitMQ
	 *  could not be recognized.
	 *  @param  connection      The connection on which the error occured
	 *  @param  message         A human readable error message
	 */
	virtual void onError(AMQP::Connection *connection, const char *message) override
	{
		std::cout << "Connection Error " << message << std::endl;
		// @todo
		//  add your own implementation, for example by reporting the error
		//  to the user of your program, log the error, and destruct the
		//  connection object because it is no longer in a usable state
	}
	/**
	 *  Method that is called when the connection was closed. This is the
	 *  counter part of a call to Connection::close() and it confirms that the
	 *  AMQP connection was correctly closed.
	 *
	 *  @param  connection      The connection that was closed and that is now unusable
	 */
	virtual void onClosed(AMQP::Connection *connection) override
	{
		std::cout << "Connection closed" << std::endl;
		// @todo
		//  add your own implementation, for example by closing down the
		//  underlying TCP connection too
	}


};


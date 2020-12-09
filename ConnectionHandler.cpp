#include "ConnectionHandler.h"

void MyConnectionHandler::onData(AMQP::Connection *connection, const char *data, size_t size)
	{
		printf("receved1? : %s", data);
		// @todo
		//  Add your own implementation, for example by doing a call to the
		//  send() system call. But be aware that the send() call may not
		//  send all data at once, so you also need to take care of buffering
		//  the bytes that could not immediately be sent, and try to send
		//  them again when the socket becomes writable again
	}

void MyConnectionHandler::onReady(AMQP::Connection *connection)
	{
		printf("receved?2");
		// @todo
		//  add your own implementation, for example by creating a channel
		//  instance, and start publishing or consuming
	}

void MyConnectionHandler::onError(AMQP::Connection *connection, const char *message)
	{
		printf("receved?3");
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
void MyConnectionHandler::onClosed(AMQP::Connection *connection) 
	{
		printf("receved?4");
		// @todo
		//  add your own implementation, for example by closing down the
		//  underlying TCP connection too
	}




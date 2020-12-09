#include <amqpcpp.h>

/**
 *  Parse data that was received from RabbitMQ
 *
 *  Every time that data comes in from RabbitMQ, you should call this method to parse
 *  the incoming data, and let it handle by the AMQP-CPP library. This method returns
 *  the number of bytes that were processed.
 *
 *  If not all bytes could be processed because it only contained a partial frame,
 *  you should call this same method later on when more data is available. The
 *  AMQP-CPP library does not do any buffering, so it is up to the caller to ensure
 *  that the old data is also passed in that later call.
 *
 *  @param  buffer      buffer to decode
 *  @param  size        size of the buffer to decode
 *  @return             number of bytes that were processed
 */
size_t parse(char *buffer, size_t size)
{
	return 0;
   // return _implementation.parse(buffer, size);
}

#include <zeromqb/zeromqb.hpp>
#include <zeromqb/zeromq_memory_queue.hpp>
#include <ExtremeCUnit.h>
#include <string.h>
TEST(create_message_queue) {
	InMemoryQueue queue(100000);
	MessageQueue mq(queue);
	return 0;
}
TEST(create_write_read) {
	InMemoryQueue queue(100000);
	MessageQueue mq(queue);
	std::vector<char> message;
	message.resize(strlen("foobar") + 1);
	strcpy(&message.at(0), "foobar");
	mq.writeMessage(message);
	std::vector<char> message_out;
	mq.readMessage(message_out);
	AssertEqStr(&message_out.at(0),"foobar"); 
	return 0;
}


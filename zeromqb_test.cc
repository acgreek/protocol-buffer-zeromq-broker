#include "include/zeromqb/zeromqb.hpp"
#include "include/zeromqb/zeromq_memory_queue.hpp"
#include <ExtremeCUnit.h>
#include <string.h>
using namespace ZeroMQb;
TEST(isFull_initial) {
	InMemoryQueue queue(10);
	MessageQueue mq(queue);
	Assert(false== mq.isFull());
	return 0;
}
TEST(isFull_one_insert) {
	InMemoryQueue queue(3);
	MessageQueue mq(queue);
	std::vector<char> message;
	message.resize(strlen("foobar") + 1);
	strcpy(&message.at(0), "foobar");
	mq.writeMessage(message);
	Assert(true == mq.isFull());
	return 0;
}
TEST(isFull_true_after_read_one) {
	InMemoryQueue queue(3);
	MessageQueue mq(queue);
	Assert(false == mq.isFull());
	std::vector<char> message;
	message.resize(strlen("foobar") + 1);
	strcpy(&message.at(0), "foobar");
	mq.writeMessage(message);
	Assert(true == mq.isFull());
	std::vector<char> message_out;
	mq.readMessage(message_out);
	Assert(false == mq.isFull());
	return 0;
}

TEST(create_message_queue) {
	InMemoryQueue queue(100000);
	MessageQueue mq(queue);
	return 0;
}
TEST(isEmpty_initial) {
	InMemoryQueue queue(100000);
	MessageQueue mq(queue);
	Assert(true == mq.isEmpty());
	return 0;
}
TEST(isEmpty_false) {
	InMemoryQueue queue(100000);
	MessageQueue mq(queue);
	std::vector<char> message;
	message.resize(strlen("foobar") + 1);
	strcpy(&message.at(0), "foobar");
	mq.writeMessage(message);
	Assert(false == mq.isEmpty());
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


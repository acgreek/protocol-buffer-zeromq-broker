#include "include/zeromqb/zeromqb.hpp"
#include "include/zeromqb/zeromq_memory_queue.hpp"
#include <ExtremeCUnit.h>
#include <string.h>
using namespace ZeroMQb;

TEST(Subscription) {
	GlobalSubscriptionManager<InMemoryQueue> queueSubscription;
	GlobalSubscriptionManager<InMemoryQueue>::Context &qc = queueSubscription.subscribe("Q1", "proc1");
	
	InMemoryQueue *q = qc.getQueue();
	std::vector<char> message;
	message.resize(strlen("foobar") + 1);
	strcpy(&message.at(0), "foobar");
	q->writeMessage(1,message);
	AssertEqInt(q->count(), 1);
	
	return 0;
	
}
/**
 * shouldn't be able to read a message written with same context
 */
TEST(SubscriptionReadEmpty_wrote1message) {
	GlobalSubscriptionManager<InMemoryQueue> queueSubscription;
	GlobalSubscriptionManager<InMemoryQueue>::Context &qc = queueSubscription.subscribe("Q1", "proc1");
	
	InMemoryQueue* q = qc.getQueue();
	std::vector<char> message;
	message.resize(strlen("foobar") + 1);
	strcpy(&message.at(0), "foobar");
	q->writeMessage(1,message);
	AssertEqInt(q->count(), 1);
	//Assert(qc.isEmpty()==true);
	
	
	return 0;
}

TEST(create_write_read_2) {
	InMemoryQueue mq(10,10);


	std::vector<char> message;
	message.resize(strlen("foobar") + 1);
	strcpy(&message.at(0), "foobar");
	mq.writeMessage(1,message);
	
	message.resize(strlen("baz") + 1);
	strcpy(&message.at(0), "baz");
	mq.writeMessage(1,message);

	std::vector<char> message_out;
	mq.readMessage(2, message_out);
	AssertEqStr(&message_out.at(0),"foobar"); 
	mq.readMessageDone(2,3);
	
	mq.readMessage(2,message_out);
	AssertEqStr(&message_out.at(0),"baz"); 
	return 0;
}

TEST(isFull_initial) {
	InMemoryQueue mq(10,10);
	Assert(false== mq.isFull());
	return 0;
}
TEST(isFull_one_insert) {
	InMemoryQueue mq(10,10);
	std::vector<char> message;
	message.resize(strlen("foobar") + 1);
	strcpy(&message.at(0), "foobar");
	mq.writeMessage(1,message);
	Assert(1 == mq.count());
	return 0;
}
TEST(isFull_true_after_read_one) {
	InMemoryQueue mq(10,10);
	Assert(false == mq.isFull());
	std::vector<char> message;
	message.resize(strlen("foobar") + 1);
	strcpy(&message.at(0), "foobar");
	mq.writeMessage(1,message);
	Assert(1== mq.count());
	std::vector<char> message_out;
	mq.readMessage(2, message_out);
	mq.readMessageDone(2,3);
	Assert(0== mq.count());
	return 0;
}

TEST(create_message_queue) {
	InMemoryQueue mq(10,10);
	return 0;
}
TEST(isEmpty_initial) {
	InMemoryQueue mq(10,10);
	Assert(true == mq.isEmpty());
	return 0;
}
TEST(isEmpty_false) {
	InMemoryQueue mq(10,10);
	std::vector<char> message;
	message.resize(strlen("foobar") + 1);
	strcpy(&message.at(0), "foobar");
	mq.writeMessage(1,message);
	Assert(false == mq.isEmpty());
	return 0;
}
TEST(create_write_read) {
	InMemoryQueue mq(100000);
	std::vector<char> message;
	message.resize(strlen("foobar") + 1);
	strcpy(&message.at(0), "foobar");
	mq.writeMessage(1,message);
	std::vector<char> message_out;
	mq.readMessage(2,message_out);
	AssertEqStr(&message_out.at(0),"foobar"); 
	mq.readMessageDone(2,3);
	return 0;
}


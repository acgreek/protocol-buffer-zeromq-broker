#ifndef ZEROMQB_HPP
#define ZEROMQB_HPP
#include <vector>
//#include <boost/thread/thread.hpp>
//#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

namespace ZeroMQb {

class MessageQueueInteraface {
	public:
		virtual ~MessageQueueInteraface() { }
		virtual void writeMessage(std::vector<char> & message) = 0;
		virtual void readMessage(std::vector<char> & message) = 0;
		virtual bool isFull() = 0;
		virtual bool isEmpty() = 0;
	private:
};

class MessageQueue :MessageQueueInteraface{
	public:
		MessageQueue(MessageQueueInteraface &mq) : mq_(mq) {}
		virtual ~MessageQueue() { }
		void writeMessage(std::vector<char> & message)  {
			mq_.writeMessage(message);
		}

		void readMessage(std::vector<char> & message)  {
			mq_.readMessage(message);
		}
		bool isFull() {return mq_.isFull();}
		bool isEmpty() {return mq_.isEmpty();}
	private:
		MessageQueueInteraface &mq_; 
};

//You should only use this with one thread reading and another writting because  isFull and isEmpty is not in a transational lock with read and write  message
//
class MessageQueue_threadsafe: MessageQueueInteraface {
	public:
		MessageQueue_threadsafe(MessageQueueInteraface &mq_internal) : mq_(mq_internal),mutex_(){
		}
		virtual ~MessageQueue_threadsafe() { }
		
		void writeMessage(std::vector<char> & message)  {
			boost::mutex::scoped_lock lock(mutex_);
			mq_.writeMessage(message);
		}

		void readMessage(std::vector<char> & message)  {
			mq_.readMessage(message);
		}
		bool isFull() {return mq_.isFull();}
		bool isEmpty() {return mq_.isEmpty();}
	private:
		MessageQueue mq_;
		boost::mutex mutex_;
};

class SubscriptionManager {

}


}
#endif

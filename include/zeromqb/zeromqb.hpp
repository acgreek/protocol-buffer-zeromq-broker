#ifndef ZEROMQB_HPP
#define ZEROMQB_HPP
#include <vector>
namespace ZeroMQb {
class MessageQueueInteraface {
	public:
		virtual void writeMessage(std::vector<char> & message) = 0;
		virtual void readMessage(std::vector<char> & message) = 0;
		virtual bool isFull() = 0;
		virtual bool isEmpty() = 0;
	private:
};

class MessageQueue{
	public:
		MessageQueue(MessageQueueInteraface &mq) : mq_(mq) {}
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
}
#endif

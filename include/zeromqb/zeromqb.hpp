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
		virtual void writeMessage(const std::vector<char> & message) = 0;
		virtual bool readMessage(std::vector<char> & message) = 0;
		virtual bool isFull() = 0;
		virtual bool isEmpty() = 0;
		virtual size_t count() = 0;

	private:
};

class MessageQueue :MessageQueueInteraface{
	public:
		MessageQueue(MessageQueueInteraface &mq) : mq_(mq) {}
		virtual ~MessageQueue() { }
		void writeMessage(const std::vector<char> & message)  {
			mq_.writeMessage(message);
		}

		bool readMessage(std::vector<char> & message)  {
			mq_.readMessage(message);
			return true;	
		}
		bool isFull() {return mq_.isFull();}
		bool isEmpty() {return mq_.isEmpty();}
		size_t count () {
			return mq_.count();
		}
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

		bool readMessage(std::vector<char> & message)  {
			mq_.readMessage(message);
			return true;
		}
		bool isFull() {return mq_.isFull();}
		bool isEmpty() {return mq_.isEmpty();}
		size_t count() {
			return mq_.count();
		}
	private:
		MessageQueue mq_;
		boost::mutex mutex_;
};

template <class T> 
class GlobalSubscriptionManager {
	public:
		class Context {
			public:
				Context() : queuep_(NULL) {}
				Context(T * queuep,unsigned int *maskp) : queuep_(queuep), maskp_(maskp) {}
				T * getQueue()  {
					return queuep_;
				}
				bool isEmpty()  {
					return queuep_->isEmpty();
				}
				void writeMessage(const std::vector<char> & message) {
					queuep_->writeMessage(id_, message);
				}
				bool readMessage(std::vector<char> & message) {
					return queuep_->readMessage(id_,message);
				}
				void readMessageDone() {
					return queuep_->readMessageDone(id_,*maskp_);

				}

				T * queuep_;
			private:
			public:
				unsigned int *maskp_;
				unsigned int id_;
		};

		GlobalSubscriptionManager() : queues_(){};
		Context & subscribe(std::string queue_name, std::string proc_name) {
			if (0 == queues_[queue_name].subscriptions_.count(proc_name)) {
				QueueSubscription &qs =queues_[queue_name];
				T * f= qs.getQueue();
				queues_[queue_name].subscriptions_[proc_name] = Context(f, qs.getMaskPtr());
				queues_[queue_name].subscriptions_[proc_name]. id_ = 1 << queues_[queue_name].current_id_;
				queues_[queue_name].subscriptions_[proc_name].queuep_ = f;
				queues_[queue_name].current_id_++;
			}
			
			return  queues_[queue_name].subscriptions_[proc_name];

		}
		class QueueSubscription {
			public :
				QueueSubscription() :queue_(), mask_(0),current_id_(0){
				}
				T* getQueue()  {
					return &queue_;
				}
				unsigned int * getMaskPtr() {
					return & mask_;
				}
					
			private :
				T queue_;
				unsigned int mask_;
			public:
				unsigned int current_id_;
				std::map<std::string, Context > subscriptions_;
		};
	private:
		std::map<std::string,  QueueSubscription > queues_;

};


}
#endif

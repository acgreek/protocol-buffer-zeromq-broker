#ifndef ZEROMQB_HPP 
#define ZEROMQB_HPP 
#include <vector>
//#include <boost/thread/thread.hpp>
//#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

typedef unsigned int SubscriberMask_t;

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
				Context() : queuep_(NULL),id_(0), maskp_(0) {}
				Context(T * queuep,SubscriberMask_t id, SubscriberMask_t *maskp) : queuep_(queuep),id_(id), maskp_(maskp) {
					queuep_->canRead(id_);
				}

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

			private:
				T * queuep_;
				SubscriberMask_t id_;
				SubscriberMask_t *maskp_;
		};

		GlobalSubscriptionManager() : queues_(){};
		Context & subscribe(std::string queue_name, std::string proc_name) {
			if (0 == queues_[queue_name].subscriptions_.count(proc_name)) {
				QueueSubscription &qs =queues_[queue_name];
				qs.subscriptions_[proc_name] = Context(qs.getQueue(), qs.getNextId(), qs.getMaskPtr());
			}
			return  queues_[queue_name].subscriptions_[proc_name];

		}
		class QueueSubscription {
			public :
				QueueSubscription() :queue_(), mask_(0),current_id_(0),subscriptions_(){
				}
				T* getQueue()  {
					return &queue_;
				}
				SubscriberMask_t getNextId() {
					SubscriberMask_t next_id = 1 <<current_id_;
					current_id_++;
					mask_ = (mask_<< 1) + 1;
					return next_id;
				}
				SubscriberMask_t * getMaskPtr() {
					return & mask_;
				}
					
				size_t number_of_subscribers() {
					return subscriptions_.size();
				}

			private :
				T queue_;
				SubscriberMask_t mask_;
			public:
				SubscriberMask_t current_id_;
				std::map<std::string, Context > subscriptions_;
		};
		size_t number_of_subscribers(std::string queue_name) {
			if (queues_.count(queue_name) == 0)
				return 0;
			return queues_[queue_name].number_of_subscribers();
		}
	private:
		std::map<std::string,  QueueSubscription > queues_;

};


}
#endif

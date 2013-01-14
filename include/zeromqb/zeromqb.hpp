#ifndef ZEROMQB_HPP 
#define ZEROMQB_HPP 
#include <vector>
//#include <boost/thread/thread.hpp>
//#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

typedef unsigned int SubscriberMask_t;

namespace ZeroMQb {

template <class T> 
class GlobalSubscriptionManager {
	public:
		class Context {
			public:
				Context() : queuep_(NULL),id_(0), maskp_(0) {}
				Context(T * queuep,SubscriberMask_t id, SubscriberMask_t *maskp) : queuep_(queuep),id_(id), maskp_(maskp) {
					queuep_->subscribe(id_);
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

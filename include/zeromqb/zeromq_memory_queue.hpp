#include "zeromqb.hpp"

#include <list>
#include <vector>
#include <utility>

namespace ZeroMQb  {
class InMemoryQueue : public MessageQueueInteraface {
	public: 
		InMemoryQueue(std::size_t max_size): max_size_(max_size),current_size_(0) {}
		void writeMessage(std::vector<char> & message)  {
			queue_. push_back (message);
			current_size_ +=message.size();

		}
		void readMessage(std::vector<char> & message) {
			message = queue_.front();
			queue_.pop_front();
			current_size_ -=message.size();
		}
		bool isFull() { return current_size_ >= max_size_; }
		bool isEmpty() { return current_size_ == 0; }
	private:
		std::size_t max_size_;
		std::size_t current_size_;
		std::list<std::vector<char> > 	queue_;
};
}

#include "zeromqb.hpp"

#include <list>
#include <vector>

class InMemoryQueue : public MessageQueueInteraface {
	public: 
		InMemoryQueue(std::size_t max_size): max_size_(max_size),current_size_(0) {}
		void writeMessage(std::vector<char> & message)  {}
		void readMessage(std::vector<char> & message) {}
		bool isFull() { return current_size_ >= max_size_; }
		bool isEmty() { return current_size_ == 0; }
	private:
		std::size_t max_size_;
		std::size_t current_size_;
		std::list<std::vector<char> > 	queue_;
};

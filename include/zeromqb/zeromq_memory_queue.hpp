#include "zeromqb.hpp"

#include <list>
#include <vector>
#include <utility>
#include <map>
#include <unordered_set>

namespace ZeroMQb  {
class InMemoryQueue {
	public: 
	
		InMemoryQueue(std::size_t max_messages, std::size_t max_size):max_messages_(max_messages), max_size_(max_size),current_size_(0),queued_messages_(0) {
			queue_.resize(max_size_, Entry());
			write_itr_= queue_.begin();
		}
		InMemoryQueue():max_messages_(1000), max_size_(1000),current_size_(0),queued_messages_(0) {
			queue_.resize(1000, Entry());
			write_itr_= queue_.begin();
		}

		InMemoryQueue(std::size_t max_size):max_messages_(1000), max_size_(max_size),current_size_(0),queued_messages_(0) {
			queue_.resize(1000, Entry());
			write_itr_= queue_.begin();
		}

		void writeMessage(unsigned id, std::vector<char> & message)  {
			if (write_itr_ == queue_.end())
				write_itr_ =queue_.begin(); 
			if (false == write_itr_->empty_) 
				throw std::string("Queue full");
			write_itr_->data_.resize(message.size());
			memcpy(&write_itr_->data_[0],&message[0], message.size());
			write_itr_->empty_= false;
			write_itr_->has_read_mask_= id;
			write_itr_++;
			current_size_ +=message.size();
			queued_messages_++;
		}

		bool readMessage(unsigned id,  std::vector<char> & message) {
			if (false ==canRead(id))
				return false;
		 	message.resize(subscriber_map_[id].read_itr_->data_.size());
			memcpy(&message[0],&subscriber_map_[id].read_itr_->data_[0], message.size());
			return true;
			
		}
		void readMessageDone(unsigned id, unsigned mask) {
			subscriber_map_[id].read_itr_->has_read_mask_ |=id; 
			if (mask == subscriber_map_[id].read_itr_->has_read_mask_) {
				current_size_ -=subscriber_map_[id].read_itr_->data_.size();
				queued_messages_--;
				subscriber_map_[id].read_itr_->empty_=true;
			}
			subscriber_map_[id].read_itr_++;

		}
		/**
		 * @return true if can read message with id
		 */
		bool canRead(unsigned id) {
			initReader(id);
			bool cont=false;
			std::vector<Entry> ::iterator &itr = subscriber_map_[id].read_itr_;
			do {
				cont=false;

				if (itr == queue_.end()) {
					itr = queue_.begin(); 
				}
				if (itr->empty_ == true)
					return false;
				if ((itr->has_read_mask_&id) == id) {
					itr++;
					cont = true;
				}
			} while (cont);
			return true;
		}
		bool isFull() { 
			return current_size_ >= max_size_ || queued_messages_ >= max_messages_; 
		}
		bool isEmpty() { return current_size_ == 0; }
		size_t count() {
			return queued_messages_;
		}

	protected:
		void initReader(unsigned int id) {
			if (0 == subscriber_map_.count(id)) {
				subscriber_map_[id].read_itr_ = queue_.begin(); 
			}
		}	
		struct Entry {
			Entry() : empty_(true), data_()  {};
			bool empty_;
			unsigned int has_read_mask_;
			std::vector<char> data_;
		};
		struct Subscriber {
			std::vector<Entry> ::iterator read_itr_;
		};
	private:
		std::size_t max_messages_;
		std::size_t max_size_;
		std::size_t current_size_;
		std::vector<Entry>	queue_;
		std::map<unsigned, Subscriber> subscriber_map_;
		std::vector<Entry> ::iterator write_itr_;
		std::size_t queued_messages_ ;

};
}

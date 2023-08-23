#pragma once
#include <array>
#include <mutex>

/*

This is my implementation for the coding task given to implement a circular buffer as per your specification.
It can store a fixed but arbitrary amount of elements. It also can store arbitrary types, both through the template implementation.
I thought about doing an implementation that can store different types dynamically, but ended up doing it as seen here since
you can also just instantiate this template with std::variant or std::any as its stored type, basically adding the functionality for free (std::any from C++17 on).

The class is made threadsafe by using a mutex lock on all public member functions. This fulfills the requirement of single consumer/producer thread safety.
As soon as you have multiple consumer threads, you cannot use count(...) as a means to reliably find out if data is available, but with this particular
implementation you could use count(...) as a guideline and check the boolean return value of pop(...) to see if you really got what you wanted. So this implementation is
also threadsafe for multiple consumers/producers if used accordingly.

Author: Leon Hellstern
*/

template<typename TYPE, int SIZE> class CircularBuffer{
public:
	CircularBuffer() {
		_readIndex = 0;
		_writeIndex = 0;
		_count = 0;
	}
	~CircularBuffer() {

	}

	//pushes a given element on the buffer
	void push(TYPE elem) {
		std::lock_guard<std::mutex> lock(_mutex);

		_container[_writeIndex] = elem;

		//make sure to move the read index if we just overflowed, do not increase count
		if (_readIndex == _writeIndex && _count > 0) {
			advance(_readIndex);
		}
		else {
			_count++;
		}
		advance(_writeIndex);
	}

	//returns the least recent element through a ref param if available
	//returns true if successful, false if no data was available (through return param)
	bool pop(TYPE& elem) {
		std::lock_guard<std::mutex> lock(_mutex);

		if (_count > 0) {
			elem = _container[_readIndex];
			advance(_readIndex);
			_count--;
			return true;
		}
		else {
			return false;
		}
	}

	//return current number of elements
	size_t count() const{
		std::lock_guard<std::mutex> lock(_mutex);
		return _count;
	}

private:

	//moves a given index forward through according to the array structure.
	void advance(size_t& index) {
		index = (index + 1) % _container.size();
	}

	std::array<TYPE, SIZE> _container;
	size_t _readIndex, _writeIndex;
	size_t _count; //extra variable to eliminate implicitness through indices

	//mutable to allow for const functions despite mutex lock guard
	mutable std::mutex _mutex;
};


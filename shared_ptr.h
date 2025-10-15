#ifndef SHARED_PTR_H
#define SHARED_PTR_H
#define WEAK_PTR_IMPLEMENTED

template<class T>
struct Counter {
	T* ptr;
	size_t strong_counter;
	size_t weak_counter;
	explicit Counter(size_t sc, size_t wc) {
		strong_counter = sc;
		weak_counter = wc;
	}
};

template <class T>
class SharedPtr {
public:
	T* ptr;
	Counter* counter;

	SharedPtr() : ptr(nullptr), counter(nullptr) {
	};
	explicit SharedPtr(T* other_ptr) : ptr(other_ptr), counter(new Counter(1, 0)) {
	};
	SharedPtr(const SharedPtr& other_shared_ptr) : ptr(other_shared_ptr.ptr), counter(other_shared_ptr.counter) {
		if (ptr) {
			(*counter).strong_counter++;
		}
	}
	SharedPtr(SharedPtr&& other_shared_ptr) noexcept : ptr(other_shared_ptr.ptr), counter(other_shared_ptr.counter) {
		other_shared_ptr.ptr = nullptr;
		other_shared_ptr.counter = nullptr;
	}

	SharedPtr& operator=(const SharedPtr& other_shared_ptr) {
		if (this != &other_shared_ptr) {
			Reset();
			ptr = other_shared_ptr.ptr;
			counter = other_shared_ptr.counter;
			if (ptr) {
				(*counter).strong_counter++;
			}
		}
		return *this;
	}
	SharedPtr& operator=(SharedPtr&& other_shared_ptr) noexcept {
		if (this != &other_shared_ptr) {
			Reset();
			ptr = other_shared_ptr.ptr;
			if (ptr) {
				counter = other_shared_ptr.counter;
				(*counter).strong_counter++;
			}
			else {
				counter = nullptr;
			}
			other_shared_ptr.Reset();
		}
		return *this;
	}

	void Reset(T* new_ptr = nullptr) {
		if (!counter || (counter->strong_counter == 1 && counter->weak_counter == 0) {
			delete ptr;
			delete counter;
			ptr = new_ptr;
		}
		else if (!counter || (counter->strong_counter == 1 && counter->weak_counter != 0) {
			delete ptr;
			ptr = new_ptr;
		}
		else {
			counter->strong_counter--;
			ptr = new_ptr;
		}

		if (!ptr) {
			counter = nullptr;
		}
		else {
			counter = new Counter(1, 0);
		}
	}
	void Swap(SharedPtr& other_shared_ptr) {
		std::swap(ptr, other_shared_ptr.ptr);
		std::swap(counter, other_shared_ptr.counter);
	}
	T* Get() const {
		return ptr;
	}
	size_t UseCount() const {
		return ptr ? counter->strong_counter : 0;
	}

	T& operator*() const {
		return *ptr;
	}
	T* operator->() const {
		return ptr;
	}
	explicit operator bool() const {
		return ptr;
	}

	~SharedPtr() {
		if (!counter || (counter->strong_counter == 1 && counter->weak_counter == 0) {
			delete ptr;
			delete counter;
		}
		else if (!counter || (counter->strong_counter == 1 && counter->weak_counter != 0) {
			delete ptr;
		}
		else {
			counter->strong_counter--;
		}
	}
};

#endif
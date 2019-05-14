#include <queue/Queue.hpp>

template <typename T>
Queue<T>::Queue(string s) { cout << "Created " << s << " queue " << endl;  }
template <typename T>
Queue<T>::Queue() {}

template <typename T>
void Queue<T>::push(T value) {
    {
        unique_lock<mutex> lock(this->d_mutex);
        d_queue.push_front(value);
    }
    this->d_condition.notify_one();
}

template <typename T>
void Queue<T>::pushAll(vector<T> values) {
    {
        unique_lock<mutex> lock(this->d_mutex);
        for (auto i = values.begin(); i != values.end(); ++i) {
            d_queue.push_front((*i));
        }
    }
    this->d_condition.notify_one();
}

template <typename T>    
T Queue<T>::pop() {
    unique_lock<mutex> lock(this->d_mutex);
    this->d_condition.wait(lock, [=]{ return !this->d_queue.empty(); });
    T retval(move(this->d_queue.back()));
    this->d_queue.pop_back();
    return retval;
}

template <typename T>
bool Queue<T>::is_empty() {
    unique_lock<mutex> lock(this->d_mutex);
    return(d_queue.empty());
}
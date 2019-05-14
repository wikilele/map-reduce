#include <mutex>
#include <string>
#include <condition_variable>
#include <deque>
#include <vector>
#include <iostream>
using namespace std;

//#ifndef QUEUEHPP
//#define QUEUEHPP

template <typename T>
class Queue{
    private:
        std::mutex                  d_mutex;
        std::condition_variable     d_condition;
        std::deque<T>             d_queue;
    
    public:
        Queue(std::string s);
        Queue();
        
        void push(T value);
        void pushAll(vector<T> values);

        T pop();

        bool is_empty();
};

//#endif /*QUEUEHPP*/
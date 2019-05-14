#include <thread>
#include <iostream>
#include <vector>
#include <Tasks.cpp>
#include <queue/Queue.cpp>
using namespace std;

template <typename TOUT>
class ReduceWorker{
    private:
    thread wthread;
    int number;
    Queue<pair<string,TOUT>>* reducetask_q;

    bool terminate = false;
    public:
    ReduceWorker(Queue<pair<string,TOUT>>* q, int number){
        this->reducetask_q = q;
        this->number = number;
    }
    
    Queue<pair<string,TOUT>>* getQueue(){return this->reducetask_q;}

    void start(){
        wthread = thread(&ReduceWorker::execute_tasks, this);

    }

    void join(){
        wthread.join();
    }

    void close(){
        this->terminate = true;
    }
    
    void execute_tasks(){
        bool emptyq = false;
        while (!this->terminate || !emptyq ){

            // FIXME probably there is a smarter way to do this active delay
            emptyq = (this->reducetask_q)->is_empty();
            if(!emptyq){ // can put true and decide not to wait for the join
                
                pair<string,TOUT> p = (this->reducetask_q)->pop();
                printf("i'm redworker %d, key %s value %d \n",this->number,p.first.c_str(),p.second);
            }
        }    
        cout << this->terminate << emptyq << endl;
        return;
    }

};
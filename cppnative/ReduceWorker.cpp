#include <thread>
#include <iostream>
#include <vector>
#include <Tasks.cpp>
#include <queue/Queue.cpp>
#include <unordered_map> 
using namespace std;

#define EOS "ENDOFSTREAMKEY"

template <typename TOUT>
class ReduceWorker{
    private:
    thread wthread;
    int id;
    int mapworkersnum;
    Queue<pair<string,TOUT>>* reducetask_q;
    TOUT (*freduce)(TOUT,TOUT);

    unordered_map<string,TOUT> hashmap;
    public:

    ReduceWorker(Queue<pair<string,TOUT>>* q, int id, int mapworkersnum, TOUT (*freduce)(TOUT,TOUT)){
        this->reducetask_q = q;
        this->id = id;
        this->mapworkersnum = mapworkersnum;
        this->freduce = freduce;
    }
    
    Queue<pair<string,TOUT>>* getQueue(){return this->reducetask_q;}

    void start(){
        wthread = thread(&ReduceWorker::execute_tasks, this);

    }

    void join(){
        wthread.join();
    }

    
    void execute_tasks(){

        int eosreceived = 0;
        while ( eosreceived < mapworkersnum){
                
                pair<string,TOUT> p = (this->reducetask_q)->pop();
               
                if (p.first == EOS) eosreceived ++;
                else {
                    //printf("i'm redworker %d, key %s value %d \n",this->id,p.first.c_str(),p.second);   
                    if ((this->hashmap).find(p.first) != (this->hashmap).end()){
                        // it exists
                        (this->hashmap)[p.first] = this->freduce((this->hashmap)[p.first], p.second);
                    } else {
                        //first time i see that elem
                        (this->hashmap)[p.first] = p.second;
                    }

                }

        }    

        for (auto x : this->hashmap){
            printf("::redworker %d:: KEY %s VALUE %d \n",this->id,x.first.c_str(),x.second);   
        }

        return;
    }

};
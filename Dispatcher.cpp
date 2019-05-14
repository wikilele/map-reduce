#include <thread>
#include <iostream>
#include <vector>
#include <string>
#include<string.h> 
#include <ReduceWorker.cpp>
#include <cmath>
#include <limits.h>

using namespace std;

// ASSUME TKEY is string
template < typename TOUT>
class Dispatcher{
    private:
    vector<ReduceWorker<TOUT>*>* reduceWorkers;

    public:
    Dispatcher(vector<ReduceWorker<TOUT>*>* redW){
        this->reduceWorkers = redW;
    }

    void dispatch(vector<pair<string,TOUT>> localreduce_results){
        //assuming 3
        const int nworker = this->reduceWorkers->size();

        for (auto i = localreduce_results.begin(); i != localreduce_results.end(); ++i) {

            std::hash<string> str_hash;
            size_t hashedvalue = str_hash((*i).first);

            int worker_index = hashedvalue % nworker;
          
            //printf(" key %s %lu\n",(*i).first.c_str(),hashedvalue);

            this->reduceWorkers
                ->at(worker_index)
                ->getQueue()
                ->push((*i));
        }
     
        return ;
    }

};

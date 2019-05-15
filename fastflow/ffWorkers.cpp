#include <string>
#include <vector>
#include <iostream>
#include <ff/ff.hpp>
#include <Tasks.cpp>
#include <SeqMapReduce.cpp>
#include <unordered_map> 
using namespace ff;

#define EOSK "ENDOFSTREAMKEY"

template <typename TIN, typename TOUT>
class ffMapWorker: public ff_monode_t <MapTask<TIN>, pair<string,TOUT>> {
    private:
    MapTask<TIN>* maptask;
    SeqMapReduce<TIN,TOUT>* seqmapreduce;

    public:
    ffMapWorker(MapTask<TIN>* maptask, SeqMapReduce<TIN,TOUT>* seqmapreduce)
        : maptask(maptask),
          seqmapreduce(seqmapreduce) {}

    pair<string,TOUT>* svc(MapTask<TIN> *) {


        vector<pair<string,TOUT>> localreduce_results = this->seqmapreduce
                                                            ->mapreduce(this->maptask->inputvect,
                                                                        this->maptask->begin,
                                                                        this->maptask->end);

        //dispatcher logic
        
        const int nworker = this->get_num_outchannels();

        for (auto i = localreduce_results.begin(); i != localreduce_results.end(); ++i) {

            std::hash<string> str_hash;
            size_t hashedvalue = str_hash((*i).first);

            
            int worker_index = hashedvalue % nworker;
          
            //printf(" key %s %lu\n",(*i).first.c_str(),hashedvalue);
            pair<string,TOUT>* task = new pair<string,TOUT>((*i).first,(*i).second);
            this->ff_send_out_to(task, worker_index);
    
        }

        for(int i=0; i < nworker; ++i){
            pair<string,TOUT>* task = new pair<string,TOUT>(EOSK, NULL);
            this->ff_send_out_to(task, i);
        }

        return this->EOS;
    }
    
};



template <typename TOUT>
class ffReduceWorker: public ff_minode_t<pair<string,TOUT>> {
    private:
    unordered_map<string,TOUT> hashmap;
    TOUT (*freduce)(TOUT,TOUT);
    int id;
    int mapworkersnum;
    
    public:
    ffReduceWorker(TOUT (*freduce)(TOUT,TOUT), int id, int mapworkersnum)
         :freduce(freduce),
          id(id),
          mapworkersnum(mapworkersnum){
              //ff_minode_t<pair<string,TOUT>>::set_id(id);
          }

    // return type not relevant
    pair<string,TOUT>* svc(pair<string,TOUT> *in) {
        pair<string,TOUT> p = *in;
               
        if (p.first == EOSK) this->mapworkersnum --;
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
        if (this->mapworkersnum <= 0) return this->EOS;
        else return this->GO_ON;
    }


        void svc_end() {
            for (auto x : this->hashmap){
            printf("::redworker %d:: KEY %s VALUE %d \n",this->id,x.first.c_str(),x.second);   
            }
        }
      
};

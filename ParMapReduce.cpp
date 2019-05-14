#include <iostream>
#include <unordered_map> 
#include <vector>
#include <utimer.hpp>
#include <SeqMapReduce.cpp>
using namespace std;





template <typename TIN, typename TOUT, typename TKEY>
class ParMapReduce{
    public:

    // sequential
    unordered_map<TKEY, vector<TOUT>> par_map(pair<TKEY,TOUT> (*f)(TIN), vector<TIN> invect){
        unordered_map<TKEY, vector<TOUT>> rethashmap;
        for (auto i = invect.begin(); i != invect.end(); ++i) {
            pair<TKEY,TOUT> tmppair = (*f) (*i);
            
            rethashmap[tmppair.first].push_back(tmppair.second);
        }

        return rethashmap;
    }

    vector<pair<TKEY,TOUT>> par_reduce(TOUT (*f) (TOUT,TOUT), unordered_map<TKEY, vector<TOUT>> inhashmap ){
        vector<pair<TKEY,TOUT>> retvect;
        for (auto x : inhashmap){
            TKEY key = x.first;
            TOUT reduceout = *(x.second.begin());
            // starting from the second element
            for (auto i = ++x.second.begin() ; i != x.second.end(); ++i) {
                reduceout = (*f) (reduceout, (*i));
            }

            retvect.push_back(pair<TKEY,TOUT>(key,reduceout));
        }

        return retvect;
    }

    vector<pair<TKEY,TOUT>> par_mapreduce(pair<TKEY,TOUT> (*mapf)(TIN), TOUT (*redf) (TOUT,TOUT), vector<TIN> invect){
        utimer timer("sequential mapreduce ");
        return this->seq_reduce(redf, this->seq_map(mapf,invect));
    }
};
#include <iostream>
#include <unordered_map> 
#include <vector>
#include <string>
#include <utimer.hpp>
using namespace std;

// ASSUME TKEY is string
template <typename TIN, typename TOUT>
class SeqMapReduce{
    protected:
    pair<string,TOUT> (*fmap)(TIN);
    TOUT (*freduce)(TOUT,TOUT);
    
    public:
    SeqMapReduce(pair<string,TOUT> (*fmap)(TIN), TOUT (*freduce)(TOUT,TOUT)){
        this->fmap = fmap;
        this->freduce = freduce;
    }

    // sequential
    unordered_map<string, vector<TOUT>> map(vector<TIN> invect){
        return this->map(invect, invect.begin(),invect.end());
    }

    unordered_map<string, vector<TOUT>> map(vector<TIN> invect,const typename  vector<TIN>::iterator begin,const typename  vector<TIN>::iterator end ){
        unordered_map<string, vector<TOUT>> rethashmap;
        for (auto i = begin; i != end; ++i) {
            pair<string,TOUT> tmppair = (*(this->fmap)) (*i);
            
            rethashmap[tmppair.first].push_back(tmppair.second);
        }

        return rethashmap;
    }


    vector<pair<string,TOUT>> reduce( unordered_map<string, vector<TOUT>> inhashmap ){
        vector<pair<string,TOUT>> retvect;
        for (auto x : inhashmap){
            string key = x.first;
            TOUT reduceout = *(x.second.begin());
            // starting from the second element
            for (auto i = ++x.second.begin() ; i != x.second.end(); ++i) {
               
                reduceout = (*(this->freduce)) (reduceout, (*i));
            }
            
            retvect.push_back(pair<string,TOUT>(key,reduceout));
        }

        return retvect;
    }

    vector<pair<string,TOUT>> mapreduce(vector<TIN> invect){
        utimer timer("sequential mapreduce ");
        return this->reduce(this->map(invect));
    }

    vector<pair<string,TOUT>> mapreduce(vector<TIN> invect,const typename  vector<TIN>::iterator begin,const typename  vector<TIN>::iterator end){
        utimer timer("sequential mapreduce ");
        return this->reduce(this->map(invect,begin,end));
    }
};



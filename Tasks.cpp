#include <iostream>
#include <vector>
using namespace std;

#ifndef TASKSCPP
#define TASKSCPP

class ThreadTask{};

template <typename TIN>
class MapTask: public ThreadTask{
    public:
    typename vector<TIN>::iterator begin;
    typename vector<TIN>::iterator end; //excluded
    vector<TIN> inputvect;


    MapTask(vector<TIN> inputvect,const typename vector<TIN>::iterator begin,const typename vector<TIN>::iterator end){
        
        this->inputvect =  inputvect;
        this->begin = begin;
        this->end = end;
    }
};

#endif

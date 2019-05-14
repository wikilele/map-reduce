
#include <SeqMapReduce.cpp>
#include <Dispatcher.cpp>
#include <Tasks.cpp>
using namespace std;

//ccomputes a map and a local reduce than dies
template <typename TIN,typename TOUT>
class MapWorker : public SeqMapReduce<TIN,TOUT>{
    private:
    thread wthread;
    Dispatcher<TOUT> * dispatcher;

    public:

    MapWorker(pair<string,TOUT> (*fmap)(TIN),
                 TOUT (*freduce)(TOUT,TOUT),
                 Dispatcher<TOUT> * dispatcher)
        :SeqMapReduce<TIN,TOUT>(fmap,freduce){
        this->dispatcher = dispatcher;
    }

    void start(MapTask<TIN>* maptask){
        wthread = thread(&MapWorker::execute_tasks, this, maptask);

    }

    void join(){
            wthread.join();
    }

    
    void execute_tasks(MapTask<TOUT>* maptask){
        vector<pair<string,TOUT>> localreduce_results = SeqMapReduce<TIN,TOUT>::mapreduce(maptask->inputvect,maptask->begin,maptask->end);

        dispatcher->dispatch(localreduce_results);

        return;
    }
};

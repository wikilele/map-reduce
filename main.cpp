#include <iostream>
#include <unordered_map> 

#include <MapWorker.cpp>

#include <cmath>
#include <string>


using namespace std;

pair<string,int> mapfunc(int x){
  
    return pair<string,int>(to_string(x), x + 1);
}

int reducefunc(int x, int y){
    return x + y;
}


int main(int argc, char * argv[]) {

    if(argc < 2) {
        cout << "Usage is: [nw] " << endl;
        return(0);
    }
    // get matrix dimensions from the command line
    int nw = atoi(argv[1]);
    
    vector<int> vect{ 10, 10, 20, 20, 30, 30 }; 

    
    SeqMapReduce<int,int> * smr = new SeqMapReduce<int,int>((&mapfunc),(&reducefunc));
   /*
    unordered_map<string, vector<int>> resultumap = smr->map(vect);

    for (auto x : resultumap){
      cout << "key " << x.first << "  values ";
      for (auto i = x.second.begin(); i != x.second.end(); ++i) {
          cout << (*i) << " ";
      }
      cout << endl;
    }

    vector<pair<string,int>> reducedvect = smr->reduce(resultumap );
    for (auto i = reducedvect.begin(); i != reducedvect.end(); ++i) {
          cout << (*i).first << " " << (*i).second << endl;
      }*/
    /*
    vector<pair<string,int>> reducedvect = smr->mapreduce(vect );
    for (auto i = reducedvect.begin(); i != reducedvect.end(); ++i) {
          cout << (*i).first << " " << (*i).second << endl;
      }*/
    
    vector<ReduceWorker<int>*> rvect ;
    for (int i = 0; i < (nw/2); i++){
      Queue<pair<string,int>>* qu = new Queue<pair<string,int>>();
      ReduceWorker<int>* rw = new ReduceWorker<int>(qu,i);

      rvect.push_back(rw);
      rw->start();
    }

    vector<int>::iterator iter = vect.begin();
    int step = vect.size()/nw;
    vector<MapWorker<int,int>*> mworkers; 
    for(int i = 0; i < nw; i++){
      
      Dispatcher<int> * dstp = new Dispatcher<int>(&rvect);
      MapWorker<int,int> * mw = new MapWorker<int,int>((&mapfunc),(&reducefunc),dstp);

      MapTask<int>* mtask = new MapTask<int>(vect,iter,iter + step);

      iter = iter + step;
      mworkers.push_back(mw);
      mw->start(mtask);

    }

    for (MapWorker<int,int>* mw : mworkers){
      mw->join();
    }

    //cout << "joining joined\n";
    
    //closing thread automatically
    /*
    for (ReduceWorker<int>* rw : rvect){
      //rw->close();
      rw->join();  
    }*/

    return 0;
}

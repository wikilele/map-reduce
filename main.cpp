#include <iostream>
#include <unordered_map> 

#include <MapWorker.cpp>

#include <cmath>
#include <string>


using namespace std;

pair<string,int> mapfunc(int x){
  // active delay
    auto start = chrono::high_resolution_clock::now();
    while (true)  {
      auto elapsed = chrono::high_resolution_clock::now() - start;
      long long microseconds = chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

      if (microseconds >= 1000) break;
    }
    

    return pair<string,int>(to_string(x), x + 1);
}

int reducefunc(int x, int y){
    return x + y;
}

vector<int> init_vector(int length){
  vector<int> vect;
  srand (42);
  for(int i = 0; i< length ; i++){
   
    vect.push_back( rand() % 42 + 1 );
  }
  return vect;
}

int main(int argc, char * argv[]) {

    if(argc < 3) {
        cout << "Usage is: [nw] [vlength] " << endl;
        return(0);
    }

    int nw = atoi(argv[1]);
    int vectorlength = atoi(argv[2]);
    
    //vector<int> vect{ 10, 10, 20, 20, 30, 30 }; 
    vector<int> vect = init_vector(vectorlength);
    
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
    {  utimer timer("parallel mapreduce ");
      vector<ReduceWorker<int>*> rvect ;
      for (int i = 0; i < (nw/2); i++){
        Queue<pair<string,int>>* qu = new Queue<pair<string,int>>();
        ReduceWorker<int>* rw = new ReduceWorker<int>(qu,i,nw,(&reducefunc));

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

      for (ReduceWorker<int>* rw : rvect){

        rw->join();  
      }
    }
    return 0;
}

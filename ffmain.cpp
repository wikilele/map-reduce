#include <cmath>
#include <string>
#include <vector>
#include <iostream>
#include <ff/ff.hpp>
#include <ff/all2all.hpp>
#include <ffWorkers.cpp>
using namespace ff;



pair<string,int> mapfunc(int x){
    for(int i = 0; i < 10000 ; i ++){
      int delay;
      delay ++;
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


int main(int argc, char *argv[]) {    
    if(argc < 3) {
        cout << "Usage is: [nw] [vlength] " << endl;
        return(0);
    }

    int nw = atoi(argv[1]);
    int vectorlength = atoi(argv[2]);
    
    //vector<int> vect{ 10, 10, 20, 20, 30, 30 }; 
    vector<int> vect = init_vector(vectorlength);


   


    std::vector<ff_node*> LeftWorkers; //mapWorkers
    std::vector<ff_node*> RightWorkers; //reduce workers

    vector<int>::iterator iter = vect.begin();
    int step = vect.size()/nw;
    for(int i = 0; i < nw; ++i) {

        MapTask<int>* mtask = new MapTask<int>(vect,iter,iter + step);
        // this can problably be done inside the class
        SeqMapReduce<int,int>* sqmr = new SeqMapReduce<int,int>((&mapfunc),(&reducefunc));
        LeftWorkers.push_back(new ffMapWorker<int,int>(mtask,sqmr));

        iter = iter + step;
    }
       
    for(int i = 0; i < nw/2; ++i){
	    RightWorkers.push_back(new ffReduceWorker<int>((&reducefunc),i,nw));
    }

    ff_a2a a2a;
    a2a.add_firstset(LeftWorkers);
    a2a.add_secondset(RightWorkers);
    
     ffTime(START_TIME);

    if (a2a.run_and_wait_end()<0) {
	    error("running a2a\n");
	    return -1;
    }


    ffTime(STOP_TIME);
    std::cout << "Time: " << ffTime(GET_TIME) << " (ms)\n";
    std::cout << "A2A Time: " << a2a.ffTime() << " (ms)\n";

    return 0;
}
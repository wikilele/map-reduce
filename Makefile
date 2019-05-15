

main: main.cpp queue/Queue.cpp
	g++ -o main main.cpp queue/Queue.cpp -lpthread  -I. -I./cppnative/

ffmain: ffmain.cpp queue/Queue.cpp
	g++ -o ffmain ffmain.cpp queue/Queue.cpp -lpthread  -I. -I./../../fastflow/ -I./fastflow/


clean:
	rm main ffmain


main: main.cpp queue/Queue.cpp
	g++ -o main main.cpp queue/Queue.cpp -lpthread  -I. 

clean:
	rm main
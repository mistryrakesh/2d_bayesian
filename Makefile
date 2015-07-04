CXXFLAGS=-g

main: main.o bayesian.o utils.o
	g++ $(CXXFLAGS) main.o bayesian.o utils.o -o main

main.o: main.cpp bayesian.h
	g++ -c $(CXXFLAGS) main.cpp -o main.o

bayesian.o: bayesian.cpp bayesian.h
	g++ -c $(CXXFLAGS) bayesian.cpp -o bayesian.o

utils.o: utils.cpp utils.h
	g++ -c $(CXXFLAGS) utils.cpp -o utils.o

clean:
	rm -rf *.o main

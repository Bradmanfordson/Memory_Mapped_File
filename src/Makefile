
make:
	make alloc
	make prov

alloc: alloc.cpp
	g++ -lpthread -Wall -c alloc.cpp -o alloc.o
	g++ alloc.o mmf_functions.h -lpthread -o alloc

prov: prov-rep.cpp
	g++ -lpthread -Wall -c prov-rep.cpp -o prov-rep.o
	g++ prov-rep.o mmf_functions.h -lpthread -o prov-rep
	

clean:
	rm alloc prov-rep
	rm alloc.o prov-rep.o
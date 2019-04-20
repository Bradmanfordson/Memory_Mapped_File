
make:
	make alloc
	make prov

alloc: alloc.cpp
	g++ -c alloc.cpp -o alloc.o
	g++ alloc.o mmf_functions.h -o alloc

prov: prov-rep.cpp
	g++ -c prov-rep.cpp -o prov-rep.o
	g++ prov-rep.o mmf_functions.h -o prov-rep
	

clean:
	rm alloc prov-rep
	rm alloc.o prov-rep.o

make: alloc.cpp prov-rep.cpp
	make alloc
	make prov

alloc: alloc.cpp
	g++ alloc.cpp -lpthread -o alloc

prov: prov-rep.cpp
	g++ prov-rep.cpp -lpthread -o prov-rep

clean:
	rm alloc prov-rep

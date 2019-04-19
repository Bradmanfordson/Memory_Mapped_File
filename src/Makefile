
make: alloc.cpp prov-rep.cpp
	make alloc
	make prov

alloc: alloc.cpp
	g++ alloc.cpp -o alloc

prov: prov-rep.cpp
	g++ prov-rep.cpp -o prov-rep

clean:
	rm alloc prov-rep

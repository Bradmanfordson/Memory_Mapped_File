#include "mmf_functions.h"

char *mem_map;
struct stat filestat;

int main() { 
    int file = open("res.txt", O_RDWR); // Open file with int for fstat()

    fstat(file, &filestat); // give the filestat struct stats about the file such as file size.
	mem_map = (char*)mmap(NULL, filestat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, file, 0);

    while(true){
       allocator(filestat.st_size, mem_map);
    }

    close(file);
    delete mem_map;

    return 0;
}
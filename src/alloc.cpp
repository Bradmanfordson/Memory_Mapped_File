#include "mmf_functions.h"

void allocator(int size, char *&mem_map);

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

void allocator(int size, char *&mem_map){
    std::string answer;
    std::cout << "Do you need to take resources? (y/n): ";
    std::cin >> answer;

    if(answer.compare("y") == 0){
        int type = get_alloc_info("Type");
        int num_units = get_alloc_info("Number of Units");

        //std::cout <<"Type: " << type <<std::endl<< "Units: " << num_units << std::endl;
        
        update_mmf(type, num_units, size, mem_map, true);  
    }
}

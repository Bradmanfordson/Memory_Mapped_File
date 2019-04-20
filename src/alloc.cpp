#include "mmf_functions.h"

void allocator();

char *mem_map;
struct stat filestat;

int main() { 
    int file = open("res.txt", O_RDWR); // Open file with int for fstat()

    fstat(file, &filestat); // give the filestat struct stats about the file such as file size.
	mem_map = (char*)mmap(NULL, filestat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, file, 0);

    while(true){
       allocator();
    }

    close(file);
    munmap(mem_map,filestat.st_size);

    return 0;
}

void allocator(){
    std::string answer;
    std::cout << "Do you need to take resources? (y/n): ";
    std::cin >> answer;

    if(answer.compare("y") == 0){
        int type = get_alloc_info("Type");
        int num_units = get_alloc_info("Number of Units");

        //std::cout <<"Type: " << type <<std::endl<< "Units: " << num_units << std::endl;
        
        for(int i = 0; i < filestat.st_size; i+=4){
            // sem wait
            int t = mem_map[i] - '0';
            int val = mem_map[i+2] - '0';

            if(t == type){
                int new_val = val - num_units;

                if(new_val >= 0){
                    mem_map[i+2] = (char)(new_val + '0');
                    
                } else {
                    std::cout << "Not enough resources for type " << type << "." << std::endl;
                }
                //signal
                break;
            }
            msync(mem_map, filestat.st_size, MS_SYNC); 
            //signal
        }  
    }else if (answer.compare("n") == 0){
        exit(0);
    }
}

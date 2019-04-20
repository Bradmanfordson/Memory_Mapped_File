#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <sys/stat.h> 
#include <unistd.h>    
#include <iostream>
#include <sys/sem.h>
#include <sys/mman.h>

void update_mmf(int type, int num_units, int size, char* &mem_map, bool alloc);
int get_alloc_info(std::string kind);


void update_mmf(int type, int num_units, int size, char* &mem_map, bool alloc){
    // sem_wait(semaphore); 

    for(int i = 0; i < size; i+=4){ // lines in res.txt should be: <type> <space> <value> <newline>
        int t = mem_map[i] - '0';
        int val = mem_map[i+2] - '0';

        if( t == type){
            if(alloc){
                int new_val = val - num_units;

                if(new_val >= 0){
                    mem_map[i+2] = (char)(new_val + '0');
                }else{
                    std::cout << "Not enough resources for type " << type << "." << std::endl;
                }
            } else {
                int new_val = val + num_units;
                if(new_val <= 9){
                    mem_map[i+2] = (char)(new_val + '0');
                } else {
                    std::cout << "Cannot add that many resources to type " << type << "." << std::endl;

                }
            }
        }
        
        msync(mem_map, size, MS_SYNC);
    }

    // sem_post(semaphore);
}


int get_alloc_info(std::string kind){
    int data;
    std::cout << kind << " (integer): ";
    std::cin >> data;
    while(std::cin.fail()){
        std::cout << "Please, enter an integer value for" << kind<<"."<< std::endl;
        std::cin.clear();
        std::cin.ignore(256, '\n');
        std::cout << kind << " (integer): ";
        std::cin>>data;
    }
    return data;
}

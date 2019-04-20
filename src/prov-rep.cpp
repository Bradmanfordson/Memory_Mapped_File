#include "mmf_functions.h"

void reporter(int size, char *&mem_map);
void provider(int size, char *&mem_map);

char *mem_map;
struct stat filestat;

int main(){
    int file = open("res.txt" , O_RDWR);
    fstat(file, &filestat);
    mem_map = (char*)mmap(NULL, filestat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, file, 0);

    pid_t childpid;
    if((childpid = fork()) == 0){ // Child Process
        while(true){
            reporter(filestat.st_size, mem_map);
        }
    } else { // Parent Process
        while(true){
            provider(filestat.st_size, mem_map);
        }
    }

    close(file);
    delete mem_map;
    
    return 0;
}


void reporter(int size, char *&mem_map){
    std::cout << "Page size: " << getpagesize() << std::endl;
    unsigned char v[size];
    if(!mincore(mem_map, size, &v[0])){
        std::cout << "Page is resident." << std::endl;
        
    } else {
        std::cout << "Page is NOT resident." << std::endl;
    }
    std::cout << "Current Resources:" << std::endl;
    std::cout << mem_map << std::endl;

    sleep(10);
}


void provider(int size, char *&mem_map){
    std::string answer;
    std::cout << "Do you need to add resources? (y/n): ";
    std::cin >> answer;

    if(answer.compare("y") == 0){
        int type = get_alloc_info("Type");
        int num_units = get_alloc_info("Number of Units");
        
        //std::cout <<"Type: " << type <<std::endl<< "Units: " << num_units << std::endl;
        
        update_mmf(type, num_units, size, mem_map, false); 
    }
}

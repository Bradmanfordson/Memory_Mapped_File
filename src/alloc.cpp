#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <sys/stat.h>  // fstat
#include <unistd.h>    // fstat
#include <iostream>
#include <semaphore.h>
#include <sys/mman.h>

void get_resources();
void take_from_memory(int type, int num_units);

char *mem_map;
sem_t *semaphore;
struct stat filestat;

int main() { 
    semaphore = sem_open("semaphore", O_CREAT);
    int file = open("res.txt", O_RDWR); // Open file with int for fstat()

    fstat(file, &filestat); // give the filestat struct stats about the file such as file size.
	mem_map = (char*)mmap(NULL, filestat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, file, 0);

    while(true){
       get_resources();
    }

    close(file);
    sem_close(semaphore);
    delete mem_map;

    return 0;
}

void get_resources(){
    std::string answer;
    std::cout << "Do you need to take resources? (y/n): ";
    std::cin >> answer;

    if(answer.compare("y") == 0){
        int type, num_units;

        std::cout << "Type (integer): ";
        std::cin >> type;
        while(std::cin.fail()){
            std::cout << "Please, enter an integer value for Type." << std::endl;
            std::cin.clear();
            std::cin.ignore(256, '\n');
            std::cout << "Type (integer): ";
            std::cin>>type;
        }

        std::cout << "Number of Units (integer): ";
        std::cin >> num_units;
        while(std::cin.fail()){
            std::cout << "Please, enter an integer value for Number of Units." << std::endl;
            std::cin.clear();
            std::cin.ignore(256, '\n');
            std::cout << "Number of Units (integer): ";
            std::cin>>num_units;
        }
        std::cout <<"Type: " << type <<std::endl<< "Units: " << num_units << std::endl;
        
        take_from_memory(type, num_units);  
    }//else if(answer.compare("n") == 0 ){
     //    exit(0);
     //}
}

void take_from_memory(int type, int num_units){
    // sem_wait(semaphore);
    
    // lines in res.txt should be: <type> <space> <value> <newline>
    for(int i = 0; i < filestat.st_size; i+=4){ 
        int t = mem_map[i] - '0';
        int val = mem_map[i+2] - '0';

        if( t == type){
            int new_val = val - num_units;

            if(new_val >= 0){
                mem_map[i+2] = (char)(new_val + '0');
            }else{
                std::cout << "Not enough resources for type " << type << "." << std::endl;
            }
        }
        
        msync(mem_map, filestat.st_size, MS_SYNC);
    }

    // sem_post(semaphore);
}
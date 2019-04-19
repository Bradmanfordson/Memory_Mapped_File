#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <sys/stat.h>  // fstat
#include <sys/types.h> // fstat
#include <unistd.h>    // fstat
#include <iostream>
#include <vector>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>

void get_resources();
void update_memory();

char *mem_map;
sem_t* semaphore;

int main() {
    // Create mapped memory region
    
    bool sem_create = true;

    // std::ifstream file("res.txt");
    struct stat filestat;
    int file = open("res.txt", O_RDWR | O_SYNC,S_IWRITE | S_IREAD); // Open file with int for fstat()

    fstat(file, &filestat); // give the filestat struct stats about the file such as file size.
    int size = filestat.st_size;

	mem_map = (char*)mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, file, 0);

    std::cout << size<< std::endl;
    std::cout << mem_map << std::endl; // print everything in memory
    
    while(true){
       get_resources();
    }


    close(file);
    return 0;
}

void get_resources(){
    std::string answer;
    std::cout << "Do you need to allocate resources? (y/n): ";
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
        
        update_memory();  
    }
}

void update_memory(){
    sem_wait(semaphore);

    
}
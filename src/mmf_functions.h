#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <sys/stat.h>  // fstat
#include <unistd.h>    // fstat
#include <iostream>
#include <semaphore.h>
#include <sys/mman.h>

void update_mmf(int type, int num_units, int size, char* &mem_map, bool alloc){
    // sem_wait(semaphore);
    // lines in res.txt should be: <type> <space> <value> <newline>

    for(int i = 0; i < size; i+=4){ 
        int t = mem_map[i] - '0';
        int val = mem_map[i+2] - '0';

        if( t == type){
            if(alloc){
                std::cout << "Value: " << val<< std::endl;
                int new_val = val - num_units;
                std::cout<< "New Value: " << new_val << std::endl;

                if(new_val >= 0){
                    mem_map[i+2] = (char)(new_val + '0');
                }else{
                    std::cout << "Not enough resources for type " << type << "." << std::endl;
                }
            } else {
                int new_val = val + num_units;
                mem_map[i+2] = (char)(new_val + '0');
            }
            
        }
        
        msync(mem_map, size, MS_SYNC);
    }

    // sem_post(semaphore);
}

void reporter(int size, char *&mem_map){
    std::cout << "Page size: " << getpagesize() << std::endl;
    unsigned char v[size];
    if(mincore(mem_map, size, &v[0]) == 0){
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
        
        update_mmf(type, num_units, size, mem_map, false); 
    }
}

void allocator(int size, char *&mem_map){
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
        
        update_mmf(type, num_units, size, mem_map, true);  
    }
}
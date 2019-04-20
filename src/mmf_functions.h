#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <sys/stat.h> 
#include <unistd.h>    
#include <iostream>
#include <sys/sem.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <signal.h>

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

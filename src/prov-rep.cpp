#include "mmf_functions.h"

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
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <sys/stat.h>  // fstat
#include <unistd.h>    // fstat
#include <iostream>
#include <semaphore.h>
#include <sys/mman.h>

#include "mmf_functions.h"

char *mem_map;
sem_t *semaphore;
struct stat filestat;

int main(){
    semaphore = sem_open("semaphore", O_CREAT);
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
    sem_close(semaphore);
    delete mem_map;
    
    return 0;
}
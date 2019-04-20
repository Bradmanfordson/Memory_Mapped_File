/* Author: Allen Bradley Roberts
Date: April 19, 2019
Course: Computer Science 352 - Operating Systems

This program simulates the allocator logic for memory mapped files.  
The Allocator is a program which allows a user to ask for 
resources from a specific type which is specified in res.txt.
*/

#include "mmf_functions.h"

void allocator();

struct stat filestat;
struct sembuf sem[1];

int sem_id;
char *mem_map;

/* main():
    - Initialize Semaphore to be used in the allocator.
    - Open and map res.txt to be used in the allocator.
    - Run the allocator(). 
    - When the allocator is finished, clean up memory and exit the program.
*/
int main() { 
    //Initialize the UNIX Semaphore.
    int semval;
    sem_id = semget(123456, 1, IPC_CREAT);

    sem[0].sem_num = 0;
    sem[0].sem_op = -1;
    sem[0].sem_flg = 0;
    semval = semop(sem_id, sem, 1);

    // Open the file
    int file = open("res.txt", O_RDWR); 

    // Give the filestat struct stats about the file such as file size.
    fstat(file, &filestat); 

    // create the memory map
	mem_map = (char*)mmap(NULL, filestat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, file, 0);

    allocator();

    // When allocator is finished, clean up memory and close program
    close(file);
    munmap(mem_map,filestat.st_size); // destroy the memory map

    return 0;
}

/* allocator():
    Continually loop 
        -ask if the user needs to take resources.
        - If user needs resources, indicated by inputting 'y':
            - ask them what type of resource they need
            - ask them how many resources they need
            - check that they aren't asking for more resources than exist
            - if their request can be met:
                - give them the resources from the mapped memory
                - sync the mapped memory with the file
            - if their request can NOT be met, inform them and break
        -If the user wants to exit, indicated by inputting 'n':
            - break out of the infinite loop
*/
void allocator(){
    while(true){
        int semval;
        std::string answer;
        std::cout << "Do you need to take resources? (y/n): ";
        std::cin >> answer;

        if(answer.compare("y") == 0){ // User wants resources
            int type = get_alloc_info("Type"); // Ask what type of resource they need
            int num_units = get_alloc_info("Number of Units"); // Ask how many of that resource they need
            
            // Loop through the file, each line will have 4 characters, thus i+=4
            for(int i = 0; i < filestat.st_size; i+=4){
                // Sem Wait
                sem[0].sem_num = 0;
                sem[0].sem_op = -1;
                sem[0].sem_flg = 0;
                semval = semop(sem_id, sem, 1);

                int t = mem_map[i] - '0'; // get mem_map type
                int val = mem_map[i+2] - '0'; // get mem_map value

                if(t == type){ // check if the current mem_map type is the type the user is wanting
                    int new_val = val - num_units;

                    if(new_val >= 0){ // check the request is valid (i.e. they aren't asking for more resources than possible to give)
                        mem_map[i+2] = (char)(new_val + '0'); // assign new value to the type in mem_map
                        
                    } else { // User asked for more resources than could be given
                        std::cout << "Not enough resources for type " << type << "." << std::endl;
                        break;
                    }
                }
                msync(mem_map, filestat.st_size, MS_SYNC); // Sync the memory and file

                // Sem Signal
                sem[0].sem_num = 0;
                sem[0].sem_op = 1;
                sem[0].sem_flg = 0;
                semval = semop(sem_id, sem, 1); 
        }  
        }else if (answer.compare("n") == 0){ // User wants to quit the program
            break;
        }
    }
}

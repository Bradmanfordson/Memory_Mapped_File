/* Author: Allen Bradley Roberts
Date: April 19, 2019
Course: Computer Science 352 - Operating Systems

This program simulates the reporter and provider logic for memory mapped files.

Reporter runs every 10 seconds and reports the current status of the memory mapped file, including:
    1. Page size
    2. Whether the pages are resident in virtual memory or not
    3. current resources

Provider is a program which allows a user to give more resources to res.txt
*/

#include "mmf_functions.h"

void reporter();
void provider();

struct stat filestat;
struct sembuf sem[1];

int sem_id;
char *mem_map;

/* main():
    - Initialize Semaphore to be used in the provider.
    - Open and map res.txt to be used in the provider.
    - fork the process
        - the Child process is the reporter()
        - the Parent process is the provider() 
    - When the provider is finished, kill child process, clean up memory and exit the program.
*/
int main(){
    //Initialize the horrible UNIX Semaphore.
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

    // fork the process
    pid_t childpid;
    if((childpid = fork()) == 0){ // Child Process
            reporter();
    } else { // Parent Process
        provider();
    }

    // When provider is finished, kill child process, clean memory, and close the program.
    kill(childpid, SIGTERM);
    close(file);
    munmap(mem_map,filestat.st_size);
    
    return 0;
}

/* reporter()
    Continually loop 
        - display the page size
        - determine if the mem_map is resident
        - display is mem_map is resident or not
        - display current resources in mem_map
        - sleep for 10 seconds before looping again
*/
void reporter(){
    while(true){
        std::cout << "Page size: " << getpagesize() << std::endl; // display page size

        unsigned char v[filestat.st_size];
        if(!mincore(mem_map, filestat.st_size, &v[0])){ // check if the mem_map is resident (mincore returns 0 if it is resident)
            std::cout << "Page is resident." << std::endl;       
        } else { // page is not resident (mincore returned -1)
            std::cout << "Page is NOT resident." << std::endl;
        }
        std::cout << "Current Resources:" << std::endl;
        std::cout << mem_map << std::endl; // display all resources in mem_map

        sleep(10);
    }
}

/* provider():
    Continually loop 
        -ask if the user wants to provide resources.
        - If user want to provide resources, indicated by inputting 'y':
            - ask them what type of resource they are providing
            - ask them how many resources they are providing
            - check that they aren't giving too many resources
            - if their request can be met:
                - add their resources to the the mapped memory
                - sync the mapped memory with the file
            - if their request can NOT be met, inform them and break
        -If the user wants to exit, indicated by inputting 'n':
            - break out of the infinite loop
*/
void provider(){
    while(true){
        int semval;
        std::string answer;
        std::cout << "Do you need to add resources? (y/n): ";
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
                    int new_val = val + num_units;

                    if(new_val <= 9){ // check that the request is valid (i.e. they aren't exceeding 9 resources for the given type)
                        mem_map[i+2] = (char)(new_val + '0'); // assign new value to the type in mem_map
                        
                    } else { // User asked for more resources than could be given
                        std::cout << "Cannot add that many resources to type " << type << "." << std::endl;
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


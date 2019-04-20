#include "mmf_functions.h"

void reporter();
void provider();

char *mem_map;
struct stat filestat;
struct sembuf sem[1];

int sem_id;

int main(){

    //Initialize Semaphore
    sem_id = semget(123456, 1, IPC_CREAT);

    if(sem_id < 0){
        std::cout << "Error: Unable to initialize semaphore." << std::endl;
    }

    int file = open("res.txt" , O_RDWR);
    fstat(file, &filestat);
    mem_map = (char*)mmap(NULL, filestat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, file, 0);

    pid_t childpid;
    if((childpid = fork()) == 0){ // Child Process
            reporter();
    } else { // Parent Process
        provider();
    }

    kill(childpid, SIGTERM);
    close(file);
    munmap(mem_map,filestat.st_size);
    
    return 0;
}


void reporter(){
    while(true){
        std::cout << "Page size: " << getpagesize() << std::endl;
        unsigned char v[filestat.st_size];
        if(!mincore(mem_map, filestat.st_size, &v[0])){
            std::cout << "Page is resident." << std::endl;
            
        } else {
            std::cout << "Page is NOT resident." << std::endl;
        }
        std::cout << "Current Resources:" << std::endl;
        std::cout << mem_map << std::endl;

        sleep(10);
    }
}


void provider(){
    while(true){
        std::string answer;
        std::cout << "Do you need to add resources? (y/n): ";
        std::cin >> answer;

        if(answer.compare("y") == 0){
            int type = get_alloc_info("Type");
            int num_units = get_alloc_info("Number of Units");
            
            //std::cout <<"Type: " << type <<std::endl<< "Units: " << num_units << std::endl;
            
            for(int i = 0; i < filestat.st_size; i+=4){
                // sem wait
                int t = mem_map[i] - '0';
                int val = mem_map[i+2] - '0';

                if(t == type){
                    int new_val = val + num_units;

                    if(new_val <= 9){
                        mem_map[i+2] = (char)(new_val + '0');
                        
                    } else {
                        std::cout << "Cannot add that many resources to type " << type << "." << std::endl;
                    }
                    //signal
                    break;
                }
                msync(mem_map, filestat.st_size, MS_SYNC); 
                //signal           
            }
        } else if (answer.compare("n") == 0){
            break;
        }
    }
}


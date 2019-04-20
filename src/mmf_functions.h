/* Author: Allen Bradley Roberts
Date: April 19, 2019
Course: Computer Science 352 - Operating Systems

This program provides all of the required header files and a single
function for getting user input when asking for or providing resources.

Originally, most of my code was neatly written in this and alloc.cpp and prov-rep.cpp
called what they needed. However, after completing the main logic and adding semaphores
I needed to put most of the code back into the files because semaphores don't play nice.
*/

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
#include <inttypes.h>

/* get_alloc_info(string)
    - take in a string parameter which will be either "Type" or "Number of Units"
    - attempt to read in an positive integer
    - loop while user doesnt provide a positive integer
        - display error message
        - clear input
        - ask for input again
    - once the user has provided valid input
        - return input data as integer

*/

int get_alloc_info(std::string kind){
    int data;
    std::cout << kind << " (Integer): ";
    std::cin >> data; // get user input
    while(std::cin.fail() || data < 0){ // while user isn't providing valid input
        std::cout << "Please, enter a positive integer value for " << kind <<"."<< std::endl;
        std::cin.clear(); // clear input so it can be used again
        std::cin.ignore(256, '\n'); // keeps infinite loop from running forever without allowing user to input data
        std::cout << kind << " (Integer): ";
        std::cin>>data; // get user input
    }
    return data; 
}

// TODO: Do i use a vector of structs OR do i need to something else with mmap?

#include <sys/mman.h>  // mmap(), msync()
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

struct mem_resource {
    int type;
    int num;
};

void read_file(vector<mem_resource> test);
int num_lines();
void print_mem(vector<mem_resource> test);

int main() {
    // TODO...
    vector<mem_resource> test;
    read_file(test);
    print_mem(test);

    return 0;
}

void read_file(vector<mem_resource> test) {
    ifstream file("res.txt");
    string line;
    string tmp = "";
    int i = 0;

    while (getline(file, line)) {  // keep reading until no more lines
        // idk
    }
    file.close();
}

void print_mem(vector<mem_resource> test) {
    vector<mem_resource>::iterator it;
    for (it = test.begin(); it != test.end(); it++) {
        cout << "test 1" << it->type << endl;
        cout << "num" << it->num << endl;
    }
}

int num_lines() {
    int lines = 0;
    ifstream file("res.txt");
    string line;

    while (getline(file, line)) {
        lines++;
    }
    file.close();

    return lines;
}
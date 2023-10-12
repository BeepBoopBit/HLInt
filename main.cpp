#include "HLint.h"

int main(int argc, char** argv){
    // read the argument from the command line

    if (argc == 1){
        std::cout << "Please provide the file name" << std::endl;
    }else{
        std::string filename = argv[1];
        HLint* hlint = new HLint(filename);
        hlint->start();
    }
}


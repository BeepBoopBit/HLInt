#include "HLint.h"

int main(int argc, char** argv){
    
    // Ensure that the user has provided the file name
    if (argc == 1){
        std::cout << "Please provide the file name. Going to default 'test.txt'" << std::endl;
        HLint* hlint = new HLint();
        hlint->start();
        delete hlint;
    }else{
        std::string filename = argv[1];
        HLint* hlint = new HLint(filename);
        hlint->start();
        delete hlint;
    }
}


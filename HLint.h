#ifndef HLINT_H
#define HLINT_H

#include "LexicalAnalyzer/lexicalAnalyzer.h"

class HLint{
private:
    LexicalAnalyzer* lexicalAnalyzer;

public:
    HLint(std::string filename = "test.txt"){
        lexicalAnalyzer = new LexicalAnalyzer(filename);
    }

    ~HLint(){
        delete lexicalAnalyzer;
    }

public:
    void start(){
        lexicalAnalyzer->analyze();
    }
};

#endif // HLINT_H

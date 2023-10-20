#ifndef TESTCASEHANDLER_H
#define TESTCASEHANDLER_H

#include <iostream>
#include <string>

class TestCaseHandler{
private:
    std::string _parentDir = "../TestCases/";
public:
    TestCaseHandler();
    ~TestCaseHandler();
    void runTestCases(){
        std::cout << "[/] Running Test Cases..." << std::endl;
    }
};

#endif // TESTCASEHANDLER_H

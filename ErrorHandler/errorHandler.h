#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

#include <string>
#include <iostream>
#include <fstream>

/*
 * This class will be used to log errors to a file and to the console.
 */
class ErrorHandler{

public:
    ErrorHandler();


public:
    void logError();
};

#endif // ERRORHANDLER_H

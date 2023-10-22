#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

#include <string>
#include <iostream>
#include <fstream>

#include "../LanguageDictionary/LanguageDictionary.h"

/*
 * This class will be used to log errors to a file and to the console.
 */
class ErrorHandler{
private:
    using LanguageToken = LanguageDictionary::LanguageToken;
private:
    std::string     _errorLogPath           = "ERROR.log";          // Default error log path
    std::ofstream   _errorLog;
    std::string     _errorString            = "";                   // The error string that will be displayed
    bool            _hasError               = false;                // If there is an error
    bool            _hasAlreadyDisplayed    = false;                // If the error has already been displayed
    int             _errorCount             = 0;                    // The number of errors

private:
    ErrorHandler(){
        _errorLog.open(_errorLogPath);
    }
    ~ErrorHandler(){
        // Ensure that the error will be saved and closed
        if(_hasError){
            saveError();
        }
        _errorLog.close();
    }

public:
    static ErrorHandler& getInstance(){
        static ErrorHandler instance;
        return instance;
    }

public:

    // Delete the copy constructor and assignment operator
    ErrorHandler(ErrorHandler const&) = delete;
    void operator=(ErrorHandler const&) = delete;

private:
    void errorBreakdown(){
        std::cout << "\n#########################ERROR BREAKDOWN#########################"<< std::endl;
        std::cout << _errorString;
        std::cout << "##################################################################\n"<< std::endl;
    }

    void saveError(){
        _errorLog << "\n#########################ERROR BREAKDOWN#########################"<< std::endl;
        _errorLog << _errorString;
        _errorLog << "##################################################################"<< std::endl;

        if(!_hasAlreadyDisplayed){
            std::cout << "[/] Error saved to " << _errorLogPath << std::endl;
            _hasAlreadyDisplayed = true;
        }
    }
public:
    bool displayError(){
        if(_hasError){
            errorBreakdown();
            saveError();
        }
        return _hasError;
    }
    void displaySuccess(std::string at, std::string message){
#ifdef DEBUG
            std::cout << "[SUCCESS] " << at << ": " << message << std::endl;
#endif
    }

    void addError(std::string error){
        _errorCount++;
        _hasError = true;
        _errorString += "[ERROR] " + error + "\n";
    }

    void addError(std::string error, int line, int column){
        _errorCount++;
        _hasError = true;
        _errorString += "[ERROR] " + error +  "at line: " + std::to_string(line) + " column: " + std::to_string(column) + "\n"; 
    }

    int getErrorCount(){
        return _errorCount;
    }
};

#endif // ERRORHANDLER_H

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
    std::string _errorLogPath = "ERROR.log";
    std::ofstream _errorLog;
    bool _isDebug = true;
    std::string _errorString = "";
    bool _hasError = false;
    bool _hasAlreadyDisplayed = false;

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
        if(_isDebug){
            std::cout << "[SUCCESS] " << at << ": " << message << std::endl;
        }
    }

    void addError(std::string error){
        _hasError = true;
        _errorString += "[ERROR] " + error + "\n";
    }

    void addError(std::string error, int line, int column){
        _hasError = true;
        _errorString += "[ERROR] " + error +  "at line: " + std::to_string(line) + " column: " + std::to_string(column) + "\n"; 
    }

    void logError();
    void parentesisError(){
        _errorString += "[!] Error no matching parenthesis.\n";
        _debug("Trying to Continue");
    }
    void identifierError(){
        _errorString += "[!] Error in Identifier.\n";
        _debug("Trying to Continue");
    }
    void oneWayIfConditionError(){
        _errorString += "[!] Error in One-Way If Condition.\n";
        _debug("Trying to Continue");
    }
    void mathematicalExpressionError(){
        _errorString += "[!] Error in Mathematical Expression.\n";
        _debug("Trying to Continue");
    }
    void outputError(){
        _errorString += "[!] Error in Output.\n";
        _debug("Trying to Continue");
    }
    void syntaxAnalyzerError(LanguageToken token){
        _errorString += "[!] Error in Syntax Analyzer. Last token is: " + LanguageDictionary::getInstance().getTokenName(token) + "\n";
        _debug("Trying to Continue");
    }
    void lexicalAnalyzerError(){
        _errorString += "[!] Error in Lexical Analyzer.\n";
        _debug("Trying to Continue");
    }
    void assignmentError(){
        _errorString += "[!] Error in Assignment.\n";
        _debug("Trying to Continue");
    }
    void declarationError(){
        _errorString += "[!] Error in Declaration.\n";
        _debug("Trying to Continue");
    }
    void conditionError(){
        _errorString += "[!] Error in Condition.\n";
        _debug("Trying to Continue");
    }


// Debug
private:
    void _debug(std::string message){
        if(_isDebug){
            _errorString += "[DEBUG] " + message + "\n";
        }
    }
};

#endif // ERRORHANDLER_H

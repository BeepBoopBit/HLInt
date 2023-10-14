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
    std::string _errorLogPath = "error.log";
    std::ofstream _errorLog;
    bool _isDebug = true;

private:
    ErrorHandler(){}
    ~ErrorHandler(){}

public:
    static ErrorHandler& getInstance(){
        static ErrorHandler instance;
        return instance;
    }

public:

    // Delete the copy constructor and assignment operator
    ErrorHandler(ErrorHandler const&) = delete;
    void operator=(ErrorHandler const&) = delete;


public:
    bool error(LanguageToken token){
        switch(token){
            case LanguageToken::OpenParenthesisToken:
            case LanguageToken::CloseParenthesisToken:
                parentesisError();
                break;
            case LanguageToken::IdentifierToken:
                identifierError();
                break;
            case LanguageToken::IfToken:
                oneWayIfConditionError();
                break;
            case LanguageToken::NumberToken:
                mathematicalExpressionError();
                break;
            case LanguageToken::OutputToken:
                outputError();
                break;
            case LanguageToken::AssignmentToken:
                assignmentError();
                break;
            case LanguageToken::TypeIntegerToken:
            case LanguageToken::TypeDoubleToken:
                declarationError();
                break;
            case LanguageToken::LessThanToken:
            case LanguageToken::GreaterThanToken:
            case LanguageToken::EqualityToken:
            case LanguageToken::NotEqualToken:
                conditionError();
                break;
            default:
                syntaxAnalyzerError(token);
                break;
        }
        return false;
    }
    void logError();
    void parentesisError(){
        std::cout << "[!] Error no matching parenthesis." << std::endl;
        _debug("Trying to Continue");
    }
    void identifierError(){
        std::cout << "[!] Error in Identifier." << std::endl;
        _debug("Trying to Continue");
    }
    void oneWayIfConditionError(){
        std::cout << "[!] Error in One-Way If Condition." << std::endl;
        _debug("Trying to Continue");
    }
    void mathematicalExpressionError(){
        std::cout << "[!] Error in Mathematical Expression." << std::endl;
        _debug("Trying to Continue");
    }
    void outputError(){
        std::cout << "[!] Error in Output." << std::endl;
        _debug("Trying to Continue");
    }
    void syntaxAnalyzerError(LanguageToken token){
        std::cout << "[!] Error in Syntax Analyzer. Last token is: " << std::endl;
        _debug("Trying to Continue");
    }
    void lexicalAnalyzerError(){
        std::cout << "[!] Error in Lexical Analyzer." << std::endl;
        _debug("Trying to Continue");
    }
    void assignmentError(){
        std::cout << "[!] Error in Assignment." << std::endl;
        _debug("Trying to Continue");
    }
    void declarationError(){
        std::cout << "[!] Error in Declaration." << std::endl;
        _debug("Trying to Continue");
    }
    void conditionError(){
        std::cout << "[!] Error in Condition." << std::endl;
        _debug("Trying to Continue");
    }


// Debug
private:
    void _debug(std::string message){
        if(_isDebug){
            std::cout << "[DEBUG]" << message << std::endl;
        }
    }
};

#endif // ERRORHANDLER_H

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
    std::string _errorString = "";

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
    bool lastError(LanguageToken, bool state = false){
        if (state){
            return true;
        }
        std::cout << "#########################ERROR BREAKDOWN#########################"<< std::endl;
        std::cout << _errorString << std::endl;
        std::cout << "##################################################################"<< std::endl;
        return false;
    }
    bool error(LanguageToken token, bool state = false){
        if(state){
            if(_isDebug){
                std::cout << "[SUCCESS] Token: " << LanguageDictionary::getInstance().getTokenName(token) << std::endl;
            }
            return true;
        }
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
    void displayError(std::string error, std::string caller = "Unknown"){
        std::cout << "\n\n#########################ERROR BREAKDOWN#########################\n";
        std::cout << "[ERROR] " << error << " from " << caller << '\n';
        std::cout << "#################################################################\n\n";
    }
    void displaySuccess(std::string success){
        if(_isDebug){
            std::cout << "[SUCCESS] " << success << '\n';
        }
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

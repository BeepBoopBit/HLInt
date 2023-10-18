#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

// Standard Libraries
#include <iostream>
#include <fstream>
#include <string>
#include <utility>

// Created Classes
#include "../SymbolTable/symbolTable.h"
#include "../SyntaxAnalyzer/SyntaxAnalyzer.h"
#include "../LanguageDictionary/LanguageDictionary.h"
#include "../ErrorHandler/ErrorHandler.h"
#include "../AbstractSyntaxTree/AbstractSyntaxTree.h"

class LexicalAnalyzer{

// Owned Types
private:
    //SyntaxAnalyzer* syntaxAnalyzer;
    LanguageDictionary* _languageDictionary;
    SyntaxAnalyzer* _syntaxAnalyzer;
    ErrorHandler* _errorHandler;
    AST* _ast;

private:
    
    // Alias LanguageToken to make things easier
    using LanguageToken = LanguageDictionary::LanguageToken;
    using TokenData = std::pair<std::string, std::string>;

// Other Variables
private:
    bool _isDebug = true;
    std::string _filename;
    std::ifstream _file;
    std::ofstream _oFile;
    int _line;
    int _column;
    int _state;
    std::string _token;
    std::string _tokenType;
    int _tokenLine;
    int _tokenColumn;
    int _tokenCount;
    int _errorCount;
    bool _error;
    bool _isNegativeNumber = false;
    std::string _totalStringNoSpace = "";
    bool _isEndedSuccessfully = false;

// Constructors
public:
    LexicalAnalyzer(std::string filename){
        // Initialize Filename
        this->_filename = filename;

        // Signify the current line
        this->_line = 1;

        // Signify the current Column
        this->_column = 1;

        // Signify the current state
        this->_state = 0;
        
        // Initialize the token
        this->_token = "";

        // Initialize the token type
        this->_tokenType = "";

        // Initialize the token line
        this->_tokenLine = 0;

        // Initialize the token column
        this->_tokenColumn = 0;

        // Initialize the token count
        this->_tokenCount = 0;

        // Initialize the error count
        this->_errorCount = 0;

        // Initialize the error
        this->_error = false;

        // Initialize the symbol table
        //this->symbolTable = new SymbolTable();

        // Initialize the language dictionary
        this->_languageDictionary = &LanguageDictionary::getInstance();

        // Initialize the syntax analyzer
        this->_syntaxAnalyzer = new SyntaxAnalyzer();

        // Initialize the error handler
        this->_errorHandler = &ErrorHandler::getInstance();

        // Initialize the Abstract Syntax Tree
        this->_ast = &AST::getInstance();

        // Open the file
        this->_file.open(filename);

        this->_oFile.open("NOSPACES.txt");
    }

    ~LexicalAnalyzer(){
        this->_file.close();

        // Put all the string with no space in the file
        _oFile << _totalStringNoSpace;

        this->_oFile.close();

        if(_isDebug){
            std::cout << "Successfuly Close the files";
        }
    }

// Methods
public:

    void analyze(){
        while(_file.is_open()){
            while(_file.good()){
                // Take note if a statement is ended correctly;
                _isEndedSuccessfully = false;

                // Container of the character
                char c = ' ';
                
                // Get a character from the file
                _file.get(c);
                
                if(_file.eof()){
                    return;
                }

                // Peek the next character
                char next = _file.peek();
                
                // Check if the character is a digit
                bool isDigit = this->isDigit(c) != LanguageToken::InvalidToken;
                bool isIdentifier = this->isIdentifier(c) != LanguageToken::InvalidToken;
                bool isOperator= this->isOperator(c) != LanguageToken::InvalidToken;
                

                // Can Handle String Literals
                if(c == '"'){
                    processStringLiteral();
                }
 
                // Can handle single or double operator
                else if(isOperator){
                    processOperator(c);
                }

                // Ensure that this will only be called if the token starts with a digit
                else if(isDigit){
                    processDigit(c);
                }

                // Can Handle Keywords
                else if(isIdentifier){
                    processIdentifier(c);
                }
                
                else if(c == ';'){
                    _ast->insert(LanguageToken::EndOfStatementToken, ";");
                    _isEndedSuccessfully = true;
                }
            }
            _file.close();
        }
        _ast->print();
    }

    bool isEndOfStatement(char c){
        if(c == ';'){
            return true;
        }
        return false;
    }


private:
    void tryPush(){
    }

private:
    void processDigit(char c){

        std::string total_value = std::string(1,c); 

        // Flags
        bool isDouble = false;
        bool isDigit = false;
        bool isAlreadyContainsDot = false;

        // Create a storage for the character
        char tempC = _file.peek();

        // Check if the character is a digit
        isDigit = this->isDigit(tempC) != LanguageToken::InvalidToken;

        if(isDigit){
            _file.get(tempC);
        }
        
        // If it's a digit, then loop until it's not a digit
        while(isDigit){

            if(tempC == '.'){
                if(isAlreadyContainsDot){
                    // ERROR
                    std::cout << "Invalid Token" << std::endl;
                }
                isDouble = true;
                isAlreadyContainsDot = true;
            }

            // Append the character to the data_value
            total_value += tempC;

            tempC = _file.peek();

            // Check if the character is a digit
            isDigit = this->isDigit(tempC) != LanguageToken::InvalidToken;
            if(!isDigit){
                break;
            }

            // Get the next character
            _file.get(tempC);

        }

        if(isDouble){
            _ast->insert(LanguageToken::NumberDoubleToken, total_value);
        }else{
            _ast->insert(LanguageToken::NumberIntegerToken, total_value);
        }

    }

    void processIdentifier(char c){

        // Contains the total value of the identifier
        std::string total_value = std::string(1,c); 

        // Create a storage for the character
        char tempC = _file.peek();

        // Check if the character is in the alphabet
        bool isIdentifier = this->isIdentifier(tempC) != LanguageToken::InvalidToken;

        if(isIdentifier){
            _file.get(tempC);
        }
        
        // If it's a digit, then loop until it's not a digit
        while(isIdentifier){

            // Append the character to the data_value
            total_value += tempC;

            tempC = _file.peek();

            isIdentifier = this->isIdentifier(tempC) != LanguageToken::InvalidToken;
            // Check if the character is a digit
            if(!isIdentifier){
                break;
            }

            // Get the next character
            _file.get(tempC);

        }

        if(this->isKeyword(total_value) != LanguageToken::InvalidToken){
            if(total_value != "if" && !_isEndedSuccessfully){
                throw std::runtime_error("Missing Semicolon");
            }
            _ast->insert(this->isKeyword(total_value), total_value);
        }else{
            _ast->insert(LanguageToken::IdentifierToken, total_value);
        }

    }

    void processOperator(char c){

        //  To Check if It's a double Operator
        char next = _file.peek();
        
        // Create a possible double operator
        std::string possibleDoubleOperator = std::string(1,c) + std::string(1,next);

        // Check if it's a double operator
        bool isDoubleOperator = this->isOperator(possibleDoubleOperator) != LanguageToken::InvalidToken;

        // If it's not a double operator, then append the normal operator
        if(!isDoubleOperator){
            // Insert the operator
            _ast->insert(this->isOperator(c), std::string(1,c));
        }
        
        // Otherwise, append the double operator
        else{
            // Insert the double operator
            _ast->insert(this->isOperator(possibleDoubleOperator), possibleDoubleOperator);
            _file.get(c);
        }
    }

    void processStringLiteral(){
        std::string total_value = "";

        // Create a storage for the character
        char tempC = ' ';

        // Get the next character
        _file.get(tempC);

        while(tempC != '"'){
            total_value += tempC;

            _file.get(tempC);

            if(_file.eof()){
                // ERROR
                std::cout << "Invalid Token" << std::endl;
                return;
            }

        }
        _ast->insert(LanguageToken::StringToken, total_value);
    }

// Checkers
private:
    

    // Check if the character is a valid identifier
    LanguageToken isIdentifier(char c){
        auto alphabet = this->_languageDictionary->getAlphabet();
        return alphabet.find(c) != alphabet.end() ? alphabet[c] : LanguageToken::InvalidToken;
    }

    
    // Check if the character is a valid digit
    LanguageToken isDigit(char c){
        auto numberAlphabet = this->_languageDictionary->getNumberAlphabet();
        return numberAlphabet.find(c) != numberAlphabet.end() ? numberAlphabet[c] : LanguageToken::InvalidToken;
    }
    
    // Check if the character is a valid operator
    LanguageToken isOperator(char c){
        auto alphabet = this->_languageDictionary->getOperatorAlphabet();
        std::string str = std::string(1,c);
        return alphabet.find(str) != alphabet.end() ? alphabet[str] : LanguageToken::InvalidToken;
    }

    LanguageToken isOperator(std::string str){
        auto alphabet = this->_languageDictionary->getOperatorAlphabet();
        return alphabet.find(str) != alphabet.end() ? alphabet[str] : LanguageToken::InvalidToken;
    }
   
    // Check if the character is a valid keyword
    LanguageToken isKeyword(std::string str){
        auto keywords = this->_languageDictionary->getLanguageKeywords();
        return keywords.find(str) != keywords.end() ? keywords[str] : LanguageToken::InvalidToken;;
    }

// Others
private:
    void debugPrint(std::string str){
        if(this->_isDebug){
            std::cout << str << "\n";
        }
    }
};

#endif // LEXICALANALYZER_H

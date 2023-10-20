#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

// Standard Libraries
#include <iostream>
#include <fstream>
#include <string>
#include <utility>

// Created Classes
#include "../SymbolTable/symbolTable.h"
#include "../LanguageDictionary/LanguageDictionary.h"
#include "../ErrorHandler/ErrorHandler.h"
#include "../AbstractSyntaxTree/AbstractSyntaxTree.h"
#include "../Interpreter/Interpreter.h"

class LexicalAnalyzer{

// Owned Types
private:
    LanguageDictionary* _languageDictionary;
    ErrorHandler* _errorHandler;
    AST* _ast;
    Interpreter* _interpreter;

private:
    
    // Alias LanguageToken to make things easier
    using LanguageToken = LanguageDictionary::LanguageToken;
    using TokenData = std::pair<std::string, std::string>;

// Other Variables
private:

    std::string     _filename               = "test.txt";                   // Default input filename
    std::string     _outfile                = "NOSPACES.txt";                // Default output filename
    std::ifstream   _file;                                                  // Input file stream
    std::ofstream   _oFile;                                                 // Output file stream
    int             _line;                                                  // Current line. Used for Error handling
    int             _column;                                                // Current column. Used for Error handling
    int             _errorCount;                                            // Current error count for the file
    std::string     _totalStringNoSpace     = "";                           // Total string without space. Use for the output file
    LanguageToken   _prevToken              = LanguageToken::InvalidToken;  // Previous token. Used for Sign Identfication
    std::string     _prevValue              = "";                           // Previous value. Used for Sign Identification
    bool            _hasEndedSuccessfully   = false;                        // Check if the file has ended successfully with semicolon at the end

// Constructors
public:
    LexicalAnalyzer(std::string filename = "test.txt"){

        this->_filename             = filename;                             // Set the filename
        this->_line                 = 0;                                    // Signify the current line
        this->_column               = 0;                                    // Signify the current column
        this->_errorCount           = 0;                                    // Signify the current error count
        this->_languageDictionary   = &LanguageDictionary::getInstance();   // Get the instance of the language dictionary
        this->_errorHandler         = &ErrorHandler::getInstance();         // Get the instance of the error handler
        this->_ast                  = &AST::getInstance();                  // Get the instance of the AST
        this->_interpreter          = &Interpreter::getInstance();          // Get the instance of the Interpreter
        

        // Input File
        this->_file.open(filename);                                         // Open the file
        if(!isInFileGood()){return;}                                        // Check if the file is good
        
        // Output File
        this->_oFile.open(_outfile);                                        // Open the file
        if(!isOutFileGood()){return;}                                       // Check if the file is good
    }

    ~LexicalAnalyzer(){
        this->closeFiles(0, _filename);                                    // Close the input file to avoid memory leak
        this->closeFiles(1, _outfile);                                     // Close the output file to avoid memory leak
        (&SymbolTable::getInstance())->saveToFile();                        // Save the Symbols in the Default File
    }

// Methods
public:

    void analyze(){

        // Check if the file is open
        while(_file.is_open()){
            // If it's, then check if it's good
            while(_file.good()){

                // Container of the character
                char c = ' ';
                
                // Get a character from the file
                _file.get(c);
                
                if(_file.eof()){
                    break;
                }

                // Peek the next character
                char next = _file.peek();
                
                // Check if the character is a digit
                bool isDigit        = this->isDigit(c)      != LanguageToken::InvalidToken;
                bool isIdentifier   = this->isIdentifier(c) != LanguageToken::InvalidToken;
                bool isOperator     = this->isOperator(c)   != LanguageToken::InvalidToken;
                

                // Can Handle String Literals
                if(c == '"'){
                    _totalStringNoSpace += c;                               // Add the current character to the total string
                    processStringLiteral();                                 // Process the string literal
                    _hasEndedSuccessfully = false;                          // Set the flag to false
                }
 
                // Can handle single or double operator or EndOfStatement Token
                else if(isOperator){

                    _totalStringNoSpace += c;                               // Add the current character to the total string
                    processOperator(c);                                     // Process the operator
                    _hasEndedSuccessfully = false;                          // Set the flag to false
                    // If it's a semicolon
                    if(c == ';'){
                        this->_line++;                                      // Increment the line
                        this->_column = 0;                                  // Reset the column
                        _hasEndedSuccessfully = true;                          // Set the flag to false
                    }
                }

                // Ensure that this will only be called if the token starts with a digit
                else if(isDigit){
                    _totalStringNoSpace += c;                               // Add the current character to the total string
                    processDigit(c);                                        // Process the digit
                    _hasEndedSuccessfully = false;                          // Set the flag to false
                }

                // Can Handle Keywords
                else if(isIdentifier){
                    _totalStringNoSpace += c;                               // Add the current character to the total string
                    processIdentifier(c);                                   // Process the identifier
                    _hasEndedSuccessfully = false;                          // Set the flag to false
                }
            }
            _file.close();
        }

        if(!_hasEndedSuccessfully){
            _errorHandler->addError("No semicolon at the end of the file",_line, _column);
        }

#ifdef DEBUG
        std::cout << "[/] Lexical Analyzer has Successfully Finished. Going to the next Phase (Syntax Analyzer)" << std::endl;
#endif
        // Check for Syntax Error
        _ast->evaluateTree();

        // Display the error if there is any
        if(_errorHandler->displayError()){
            // If there is an error, then don't continue to the next phase
            std::cout << "[!] Will not continue to the next phase" << std::endl;
            std::cout << "[!] Please fix the error(s) above" << std::endl;
            return;
        }

#ifdef DEBUG 
            std::cout << "[/] Syntax Analyzer Successfuly Finished. Tree has been created and validated. Going to the next Phase (Interpreting)" << std::endl;

    #ifdef DEBUG_AST_BEFORE_INTERPRETER
            _ast->print();
    #endif

#endif
        auto trees = _ast->getTrees();
        for(auto tree : trees){
            _interpreter->interpret(tree);

#ifdef DEBUG 
        #ifdef DEBUG_AST_INSIDE_INTERPRETER
            std::cout << "[/] Succesfuly Interpreter" << std::endl;
        #endif
#endif
        }

#ifdef DEBUG 
    #ifdef DEBUG_AST_AFTER_INTERPRETER
            _ast->print();
    #endif
#endif
        _oFile << _totalStringNoSpace;                                     // Put all the string with no space in the output file
    }

    bool isEndOfStatement(char c){
        if(c == ';'){
            return true;
        }
        return false;
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
        isDigit = this->isDigit(tempC) != LanguageToken::InvalidToken || tempC == '.';

        if(isDigit){
            _file.get(tempC);
        }
        
        // If it's a digit, then loop until it's not a digit
        while(isDigit){
            this->_column++;

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
            _totalStringNoSpace += tempC;

            tempC = _file.peek();

            // Check if the character is a digit
            isDigit = this->isDigit(tempC) != LanguageToken::InvalidToken || tempC == '.';
            if(!isDigit){
                break;
            }

            // Get the next character
            _file.get(tempC);

        }

        if(isDouble){
            _ast->insert(LanguageToken::NumberDoubleToken, total_value, _line, _column);
            _prevToken = LanguageToken::NumberDoubleToken;
            _prevValue = total_value;
        }else{
            _ast->insert(LanguageToken::NumberIntegerToken, total_value, _line, _column);
            _prevToken = LanguageToken::NumberIntegerToken;
            _prevValue = total_value;
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
            this->_column++;

            // Append the character to the data_value
            total_value += tempC;
            _totalStringNoSpace += tempC;

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
            //char next = _file.peek();
            LanguageToken nextToken = this->isKeyword(total_value);
            _ast->insert(nextToken, total_value, _line, _column);
            _prevToken = nextToken;
            _prevValue = total_value;
        }else{
            _ast->insert(LanguageToken::IdentifierToken, total_value, _line, _column);
            _prevToken = LanguageToken::IdentifierToken;
            _prevValue = total_value;
        }

    }

    void processOperator(char c){
        this->_column++;

        //  To Check if It's a double Operator
        char next = _file.peek();
        
        // Create a possible double operator
        std::string possibleDoubleOperator = std::string(1,c) + std::string(1,next);

        // Check if it's a double operator
        bool isDoubleOperator = this->isOperator(possibleDoubleOperator) != LanguageToken::InvalidToken;
        bool isDigit = this->isDigit(next) != LanguageToken::InvalidToken;

        // Then it's a negative operator
        if((c != ')' && c != '(')&& isDigit && detectIfASign(c)){
            processDigit(c);
            return;
        }

        // If it's not a double operator, then append the normal operator
        if(!isDoubleOperator){
            // Insert the operator
            LanguageToken nextToken = this->isOperator(c);
            _ast->insert(nextToken, std::string(1,c), _line, _column);
            _prevToken = nextToken;
            _prevValue = std::string(1,c);
        }
        
        // Otherwise, append the double operator
        else{
            // Insert the double operator
            LanguageToken nextToken = this->isOperator(possibleDoubleOperator);
            _ast->insert(nextToken, possibleDoubleOperator, _line, _column);
            _prevToken = nextToken;
            _prevValue = possibleDoubleOperator;
            _file.get(c);
            _totalStringNoSpace += c;
        }
    }

    void processStringLiteral(){
        std::string total_value = "\"";

        // Create a storage for the character
        char tempC = ' ';

        // Get the next character
        _file.get(tempC);

        while(tempC != '"'){
            this->_column++;
            total_value += tempC;
            _totalStringNoSpace += tempC;

            _file.get(tempC);

            if(_file.eof()){
                // ERROR
                std::cout << "Invalid Token" << std::endl;
                return;
            }

        }
        total_value += "\"";
        _totalStringNoSpace += tempC;
        _ast->insert(LanguageToken::StringToken, total_value, _line, _column);
        _prevToken = LanguageToken::StringToken;
        _prevValue = total_value;
    }

// Checkers
private:
    bool detectIfASign(char c){
        bool isOperator = false;
        bool isNull = (_prevToken == LanguageToken::InvalidToken) && (_prevValue == "");
        bool isDoubleOperator = this->isOperator(_prevValue) != LanguageToken::InvalidToken;
        if(_prevValue.length() == 1){
            c = _prevValue[0];
            isOperator = this->isOperator(c) != LanguageToken::InvalidToken;
            isDoubleOperator = false;
        }

        if(isOperator || isDoubleOperator || isNull){
            return true;
        }
        return false;
    }

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

    bool isInFileGood(){
        if(_file.is_open()){
#ifdef DEBUG
                std::cout << "[/] Successfuly Open the file" << std::endl;
#endif
        }else{
            std::cout << "[!] Failed to open the file [" << _filename << "]. Maybe it's not existing" << std::endl;
            return false;
        }
        return true;

    }
    bool isOutFileGood(){
        if(_oFile.is_open()){
#ifdef DEBUG
            std::cout << "[/] Successfuly Open the file" << std::endl;
#endif
        }else{
            std::cout << "[!] Failed to open the file [" << _outfile << "]. Maybe it's not existing" << std::endl;
            return false;
        }
        return true;
    }

    void closeFiles(int fileIndex, std::string filename){
        if(fileIndex == 0){
            _file.close();
#ifdef DEBUG
            std::cout << "[/] Successfuly Closed the file [" << filename << "]" << std::endl;
#endif
        }else{
            _oFile.close();
#ifdef DEBUG
            std::cout << "[/] Successfuly Closed the file [" << filename << "]" << std::endl;
#endif
        }
    }

};

#endif // LEXICALANALYZER_H

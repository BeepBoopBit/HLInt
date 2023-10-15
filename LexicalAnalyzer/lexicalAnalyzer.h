#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

// Standard Libraries
#include <iostream>
#include <fstream>
#include <string>

// Created Classes
#include "../SymbolTable/symbolTable.h"
#include "../SyntaxAnalyzer/SyntaxAnalyzer.h"
#include "../LanguageDictionary/LanguageDictionary.h"
#include "../ErrorHandler/ErrorHandler.h"

class LexicalAnalyzer{

// Owned Types
private:
    //SyntaxAnalyzer* syntaxAnalyzer;
    LanguageDictionary* _languageDictionary;
    SyntaxAnalyzer* _syntaxAnalyzer;
    ErrorHandler* _errorHandler;

private:
    
    // Alias LanguageToken to make things easier
    using LanguageToken = LanguageDictionary::LanguageToken;

// Other Variables
private:
    bool _isDebug = true;
    std::string _filename;
    std::ifstream _file;
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

        // Open the file
        this->_file.open(filename);
    }

// Methods
public:
    void analyze(){

        // Stores the token of the line
        std::vector<LanguageToken> line_token;

        // Store the String
        // Not all tokens have token_value;
        std::string token_value = "";

        // Store the character that will be read
        char c = ' ';

        // flags
        bool isDigit = false;
        bool isDouble = false;
        
        // While the file is open
        while(this->_file.is_open()){

            // and the file is good
            while(this->_file.good()){
                // Store the data from the current Character;
                _file.get(c);
                
                // If the file is at the end of the file
                if(_file.eof()){
                    break;
                }

                // Check if it's a white space
                if(c == ' '){
                    continue;
                }
                
                // Stores the type of the token
                LanguageToken tokenType = LanguageToken::InvalidToken;
                
                // Enesure that the end of line is ignored. (In the language it isn't relevant)
                if(c == '\n'){
                    this->_column = 0;
                    this->_line += 1;
                    token_value = "";
                    continue;
                }

                // Number
                if((tokenType = this->isDigit(c)) != LanguageToken::InvalidToken){
                    processDigit(token_value, c, isDigit);
                }

                // Operator
                else if((tokenType = this->isOperator(c)) != LanguageToken::InvalidToken){
                    processOperator(token_value, c, isDigit, isDouble, line_token, tokenType);
                }

                // Identifier
                else if((tokenType = this->isAlphabet(c)) != LanguageToken::InvalidToken){
                    processIdentifier(token_value, c, isDigit);
                }

                // Floating point support
                else if(c == '.'){
                    token_value += c;
                    isDouble = true;
                }

                // Either it's not supported yet, or it's an invalid token.
                else{
                    std::cout << "Unknown Token Detected at " << this->_line << ":" << this->_column << std::endl;
                }

                // Add Column Value
                this->_column++;
            }
            this->_file.close();
        }
        
        if(_isDebug){
            for(int i = 0; i < line_token.size(); i++){
                if (line_token[i] == LanguageToken::CharacterToken){
                    std::cout << "Character" << std::endl;
                }else if (line_token[i] == LanguageToken::IdentifierToken){
                    std::cout << "Identifier" << std::endl;
                }else if (line_token[i] == LanguageToken::NumberToken){
                    std::cout << "NumberToken" << std::endl;
                }else if (line_token[i] == LanguageToken::WhiteSpaceToken){
                    std::cout << "WhiteSpace" << std::endl;
                }else if (line_token[i] == LanguageToken::PlusToken){
                    std::cout << "Plus" << std::endl;
                }else if (line_token[i] == LanguageToken::MinusToken){
                    std::cout << "Minus" << std::endl;
                }else if (line_token[i] == LanguageToken::EndLineToken){
                    std::cout << "EndLine" << std::endl;
                }else if (line_token[i] == LanguageToken::AssignmentToken){
                    std::cout << "Assignment" << std::endl;
                }else if (line_token[i] == LanguageToken::EndOfStatementToken){
                    std::cout << "EndOfStatement" << std::endl;
                }else if (line_token[i] == LanguageToken::ColonToken){
                    std::cout << "Colon" << std::endl;
                }else if (line_token[i] == LanguageToken::InvalidToken){
                    std::cout << "Invalid" << std::endl;
                }else if (line_token[i] == LanguageToken::QuoteToken){
                    std::cout << "Quote" << std::endl;
                }else if (line_token[i] == LanguageToken::LeftShiftToken){
                    std::cout << "LeftShift" << std::endl;
                }else if (line_token[i] == LanguageToken::LessThanToken){
                    std::cout << "LessThan" << std::endl;
                }else if (line_token[i] == LanguageToken::GreaterThanToken){
                    std::cout << "GreaterThan" << std::endl;
                }else if (line_token[i] == LanguageToken::IfToken){
                    std::cout << "If" << std::endl;
                }else if (line_token[i] == LanguageToken::OpenParenthesisToken){
                    std::cout << "OpenParenthesis" << std::endl;
                }else if (line_token[i] == LanguageToken::CloseParenthesisToken){
                    std::cout << "CloseParenthesis" << std::endl;
                }else if (line_token[i] == LanguageToken::LiteralToken){
                    std::cout << "Literal" << std::endl;
                }else if(line_token[i] == LanguageToken::StringToken){
                    std::cout << "StringToken" << std::endl;
                }else if(line_token[i] == LanguageToken::EqualityToken){
                    std::cout << "EqualityToken" << std::endl;
                }else if(line_token[i] == LanguageToken::NotEqualToken){
                    std::cout << "NotEqualToken" << std::endl;
                }else if(line_token[i] == LanguageToken::TypeIntegerToken){
                    std::cout << "TypeIntegerToken" << std::endl;
                }else if(line_token[i] == LanguageToken::TypeDoubleToken){
                    std::cout << "TypeDoubleToken" <<std::endl;
                }
            }
        }
    }

// Checkers
private:

    // Check if the character is in the alphabet
    LanguageToken isAlphabet(char c){
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
        return alphabet.find(c) != alphabet.end() ? alphabet[c] : LanguageToken::InvalidToken;
    }
   
    // Check if the character is a valid keyword
    LanguageToken isKeyword(std::string str){
        auto keywords = this->_languageDictionary->getLanguageKeywords();
        return keywords.find(str) != keywords.end() ? keywords[str] : LanguageToken::InvalidToken;;
    }

// Auxillary Functions
private:
    void processDigit(std::string& token_value, char &c, bool &isDigit){
        // It's a possibility that an identifier ends with a number.

        // If the token_value contains something
        if (token_value != ""){
            // Then it's probably an identifier with a number at the end.
            // Add the character to the token_value
            token_value += c;

            // If isDigit flag is on, then it's probably a digit
        }
        
        // Otherwise, it's a literal number
        else{
            token_value += c;
            isDigit = true;
        }
    }
    
    void processOperator(std::string& token_value, char &c, bool &isDigit, bool& isDouble, std::vector<LanguageToken> &line_token, LanguageToken &tokenType){
        // If it's an operator, this would mean that whatever is on the LHS can be an identifier or a keyword.
        
        // Check if the token_value contains something
        if (token_value != ""){
            
            LanguageToken temp_tokenType = this->isKeyword(token_value);

            // If it's a keyword.
            if (temp_tokenType != LanguageToken::InvalidToken){

                // Check if it's an OutputToken
                if(temp_tokenType == LanguageToken::OutputToken){
                    
                    // If it's then the next character in the file should be '<'
                    char next = _file.peek();
                    if(c == '<' && next == '<'){
                        tokenType = LanguageToken::LeftShiftToken;

                        // Consume the next file
                        _file.get(next);
                    }else{
                        std::cout << "Error: << operator expected after output keyword at " << this->_line << ":" << this->_column << std::endl;
                    }
                }
               line_token.push_back(temp_tokenType);
            }
            
            // Otherwise, it's an identifier or a digit or a literal.
            else{
                
                if(isDigit){
                    if(isDouble){
                        if(_isNegativeNumber){
                            _errorHandler->displaySuccess("Is Negative Number (Will support token in SymbolTable)");
                        }
                        line_token.push_back(LanguageToken::TypeDoubleToken);
                    }else{
                        if(_isNegativeNumber){
                            _errorHandler->displaySuccess("Is Negative Number (Will support token in SymbolTable)");
                        }
                        line_token.push_back(LanguageToken::NumberToken);
                    }
                    isDigit = false;
                    isDouble = false;
                    _isNegativeNumber = false;
                }else{
                    line_token.push_back(LanguageToken::IdentifierToken);
                }

                char next = _file.peek();

                if(c == ':' && next == '='){
                    tokenType = LanguageToken::AssignmentToken;
                    _file.get(next);
                }else if(c == '='){
                    tokenType = LanguageToken::InvalidToken;
                }

                // Check if it's a conditional operator
                processCheckConditionalOperator(c, next, tokenType);

            }
            if(tokenType == LanguageToken::EndOfStatementToken){
                line_token.push_back(tokenType);
                if(!this->_syntaxAnalyzer->analyze(line_token)){
                    std::cout << "Error at line " << this->_line << " before column" << this->_column << std::endl;
                }
                line_token.clear();
            }else{
                line_token.push_back(tokenType);
            }
        }
        
        // If it's empty, then it's an operator.
        // NOTE: Possible Error Handling. If it's not the EndOfStatementToken then it's probably an error.
        else{
            
            // If it's quoteToken, then we'll go through all the file until we see an equivalent token
            if(tokenType == LanguageToken::QuoteToken){
                //line_token.push_back(tokenType);
                std::string tempValue = "";
                char next = ' ';
                _file.get(next);
                while(this->isOperator(next) != LanguageToken::QuoteToken){
                    tempValue += next;
                    if(_file.eof()){
                        _errorHandler->displayError("Missing Quote Equivalent", "LexicalAnalyzer");
                        return;
                    }
                    _file.get(next);
                }
                line_token.push_back(LanguageToken::StringToken);
            }else{
                char next = _file.peek();

                // Check if it's a conditional operator
                processCheckConditionalOperator(c, next, tokenType);

                if(tokenType == LanguageToken::EndOfStatementToken){
                    line_token.push_back(tokenType);
                    if(!this->_syntaxAnalyzer->analyze(line_token)){
                        _errorHandler->displayError("Error at line " + std::to_string(this->_line) + " before column " + std::to_string(this->_column), "LexicalAnalyzer");
                    }
                    line_token.clear();
                }
                
                else{
                    line_token.push_back(tokenType);
                }

                // Probably a negative value
                if(c == '-'){
                    processPossibleNegativeNumber(line_token, tokenType);
                    return;
                }
            }
        }
        token_value = "";
    }

    void processPossibleNegativeNumber(std::vector<LanguageToken>& line_token, LanguageToken new_token){
        LanguageToken token = line_token[line_token.size()-2];
        if(this->_syntaxAnalyzer->isIdentifierOrLiteral(token)){
            return;
        }
        switch(token){
            // An Operator
            case LanguageToken::CloseParenthesisToken:
                break;
            // A negative Number
            default:
                _isNegativeNumber = true;
                line_token.pop_back();
                break;
        }
    }

    void processIdentifier(std::string& token_value, char& c, bool& isDigit){
        if (isDigit){
            _errorHandler->displayError("Error: You can't start an identifier with a number", "LexicalAnalyzer");
        }
        // Add the character to the token_value
        token_value += c;
    }

    void processCheckConditionalOperator(char &c, char &next, LanguageToken &tokenType){
        if(c == '<' && next == '<'){
            tokenType = LanguageToken::LeftShiftToken;
            _file.get(next);
        }else if(c == '!' && next  == '='){
            tokenType = LanguageToken::NotEqualToken;
            _file.get(next);
        }else if(c == '=' && next == '='){
            tokenType = LanguageToken::EqualityToken;
            _file.get(next);
        }
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

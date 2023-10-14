#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

// Standard Libraries
#include <iostream>
#include <fstream>
#include <string>

// Created Classes
#include "../SymbolTable/symbolTable.h"
#include "../SyntaxAnalyzer/SyntaxAnalyzer.h"

class LexicalAnalyzer{

private:
    SyntaxAnalyzer* syntaxAnalyzer;
    LanguageDictionary* languageDictionary;


private:
    
    using LanguageToken = LanguageDictionary::LanguageToken;

// Other Variables
private:
    bool isDebug = true;
    std::string filename;
    std::ifstream file;
    int line;
    int column;
    int state;
    std::string token;
    std::string tokenType;
    int tokenLine;
    int tokenColumn;
    int tokenCount;
    int errorCount;
    bool error;

// Constructors
public:
    LexicalAnalyzer(std::string filename){
        // Initialize Filename
        this->filename = filename;

        // Signify the current line
        this->line = 1;

        // Signify the current Column
        this->column = 1;

        // Signify the current state
        this->state = 0;
        
        // Initialize the token
        this->token = "";

        // Initialize the token type
        this->tokenType = "";

        // Initialize the token line
        this->tokenLine = 0;

        // Initialize the token column
        this->tokenColumn = 0;

        // Initialize the token count
        this->tokenCount = 0;

        // Initialize the error count
        this->errorCount = 0;

        // Initialize the error
        this->error = false;

        // Initialize the symbol table
        //this->symbolTable = new SymbolTable();

        // Initialize the language dictionary
        this->languageDictionary = new LanguageDictionary();

        // Initialize the syntax analyzer
        this->syntaxAnalyzer = new SyntaxAnalyzer();

        // Open the file
        this->file.open(filename);
    }
    ~LexicalAnalyzer();

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
        
        // While the file is open
        while(this->file.is_open()){

            // and the file is good
            while(this->file.good()){
                // Store the data from the current Character;
                file.get(c);
                
                // If the file is at the end of the file
                if(file.eof()){
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
                    this->column = 0;
                    this->line += 1;
                    token_value = "";
                    continue;
                }

                // Number
                if((tokenType = this->isDigit(c)) != LanguageToken::InvalidToken){
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

                // Operator
                else if((tokenType = this->isOperator(c)) != LanguageToken::InvalidToken){
                    // If it's an operator, this would mean that whatever is on the LHS can be an identifier or a keyword.
                    
                    // Check if the token_value contains something
                    if (token_value != ""){
                        
                        LanguageToken temp_tokenType = this->isKeyword(token_value);

                        // If it's a keyword.
                        if (temp_tokenType != LanguageToken::InvalidToken){
                            if(temp_tokenType == LanguageToken::OutputToken){
                                char next = file.peek();
                                if(c == '<' && next == '<'){
                                    tokenType = LanguageToken::LeftShiftToken;

                                    // Consume the next file
                                    file.get(next);
                                }
                            }
                           line_token.push_back(temp_tokenType);
                        }
                        
                        // Otherwise, it's an identifier or a digit or a literal.
                        else{
                            
                            if(isDigit){
                                isDigit = false;
                                line_token.push_back(LanguageToken::NumberToken);
                            }else{
                                line_token.push_back(LanguageToken::IdentifierToken);
                            }

                            char next = file.peek();
                            if(c == '<' && next == '<'){
                                tokenType = LanguageToken::LeftShiftToken;

                                // Consume the next file
                                file.get(next);
                            }else if(c == '!' && next  == '='){
                                tokenType = LanguageToken::NotEqualToken;

                                // Consume the next file
                                file.get(next);
                            }else if(c == '=' && next == '='){
                                tokenType = LanguageToken::EqualityToken;

                                // Consume the next file
                                file.get(next);
                            }

                            if(c == ':' && next == '='){
                                tokenType = LanguageToken::AssignmentToken;
                                file.get(next);
                            }
                        }
                        if(tokenType == LanguageToken::EndOfStatementToken){
                            line_token.push_back(tokenType);
                            if(!this->syntaxAnalyzer->analyze(line_token)){
                                std::cout << "Error at line " << this->line << " before column" << this->column << std::endl;
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
                            file.get(next);
                            while(this->isOperator(next) != LanguageToken::QuoteToken){
                                tempValue += next;
                                if(file.eof()){
                                    std::cout << "ERROR: Missing Quote Equivalent" << std::endl;
                                    return;
                                }
                                file.get(next);
                            }
                            line_token.push_back(LanguageToken::StringToken);
                        }else{
                            char next = file.peek();
                            if(c == '<' && next == '<'){
                                tokenType = LanguageToken::LeftShiftToken;

                                // Consume the next file
                                file.get(next);
                            }else if(c == '!' && next  == '='){
                                tokenType = LanguageToken::NotEqualToken;

                                // Consume the next file
                                file.get(next);
                            }else if(c == '=' && next == '='){
                                tokenType = LanguageToken::EqualityToken;

                                // Consume the next file
                                file.get(next);
                            }
                            line_token.push_back(tokenType);
                        }
                    }
                    token_value = "";
                }

                // Identifier
                else if((tokenType = this->isAlphabet(c)) != LanguageToken::InvalidToken){
                    if (isDigit){
                        std::cout << "ERROR: You can't start an identifier with a number";
                    }
                    // Add the character to the token_value
                    token_value += c;
                }
                // Either it's not supported yet, or it's an invalid token.
                else{
                    std::cout << "Unknown Token Detected at " << this->line << ":" << this->column << std::endl;
                }

                // Add Column Value
                this->column++;
            }
            this->file.close();
        }
        
        if(isDebug){
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

    // Contains the alphabet of the language
    // I used map because it's faster to search than normal looping
    std::map<char, LanguageToken> alphabet ={
        {'a', LanguageToken::CharacterToken},
        {'b', LanguageToken::CharacterToken},
        {'c', LanguageToken::CharacterToken},
        {'d', LanguageToken::CharacterToken},
        {'e', LanguageToken::CharacterToken},
        {'f', LanguageToken::CharacterToken},
        {'g', LanguageToken::CharacterToken},
        {'h', LanguageToken::CharacterToken},
        {'i', LanguageToken::CharacterToken},
        {'j', LanguageToken::CharacterToken},
        {'k', LanguageToken::CharacterToken},
        {'l', LanguageToken::CharacterToken},
        {'m', LanguageToken::CharacterToken},
        {'n', LanguageToken::CharacterToken},
        {'o', LanguageToken::CharacterToken},
        {'p', LanguageToken::CharacterToken},
        {'q', LanguageToken::CharacterToken},
        {'r', LanguageToken::CharacterToken},
        {'s', LanguageToken::CharacterToken},
        {'t', LanguageToken::CharacterToken},
        {'u', LanguageToken::CharacterToken},
        {'v', LanguageToken::CharacterToken},
        {'w', LanguageToken::CharacterToken},
        {'x', LanguageToken::CharacterToken},
        {'y', LanguageToken::CharacterToken},
        {'z', LanguageToken::CharacterToken},
        {'A', LanguageToken::CharacterToken},
        {'B', LanguageToken::CharacterToken},
        {'C', LanguageToken::CharacterToken},
        {'D', LanguageToken::CharacterToken},
        {'E', LanguageToken::CharacterToken},
        {'F', LanguageToken::CharacterToken},
        {'G', LanguageToken::CharacterToken},
        {'H', LanguageToken::CharacterToken},
        {'I', LanguageToken::CharacterToken},
        {'J', LanguageToken::CharacterToken},
        {'K', LanguageToken::CharacterToken},
        {'L', LanguageToken::CharacterToken},
        {'M', LanguageToken::CharacterToken},
        {'N', LanguageToken::CharacterToken},
        {'O', LanguageToken::CharacterToken},
        {'P', LanguageToken::CharacterToken},
        {'Q', LanguageToken::CharacterToken},
        {'R', LanguageToken::CharacterToken},
        {'S', LanguageToken::CharacterToken},
        {'T', LanguageToken::CharacterToken},
        {'U', LanguageToken::CharacterToken},
        {'V', LanguageToken::CharacterToken},
        {'W', LanguageToken::CharacterToken},
        {'X', LanguageToken::CharacterToken},
        {'Y', LanguageToken::CharacterToken},
        {'Z', LanguageToken::CharacterToken},
        {'_', LanguageToken::CharacterToken},
        {'0', LanguageToken::CharacterToken},
        {'1', LanguageToken::CharacterToken},
        {'2', LanguageToken::CharacterToken},
        {'3', LanguageToken::CharacterToken},
        {'4', LanguageToken::CharacterToken},
        {'5', LanguageToken::CharacterToken},
        {'6', LanguageToken::CharacterToken},
        {'7', LanguageToken::CharacterToken},
        {'8', LanguageToken::CharacterToken},
        {'9', LanguageToken::CharacterToken},
    };
    
    // Check if the character is in the alphabet
    LanguageToken isAlphabet(char c){
        return this->alphabet.find(c) != this->alphabet.end() ? this->alphabet[c] : LanguageToken::InvalidToken;
    }
    
    // Contains the digit of the language
    // I used map because it's faster to search than normal looping
    std::map<char, LanguageToken> numberAlphabet ={
        {'0', LanguageToken::NumberToken},
        {'1', LanguageToken::NumberToken},
        {'2', LanguageToken::NumberToken},
        {'3', LanguageToken::NumberToken},
        {'4', LanguageToken::NumberToken},
        {'5', LanguageToken::NumberToken},
        {'6', LanguageToken::NumberToken},
        {'7', LanguageToken::NumberToken},
        {'8', LanguageToken::NumberToken},
        {'9', LanguageToken::NumberToken}
    };

    // Check if the character is a valid digit
    LanguageToken isDigit(char c){
        return this->numberAlphabet.find(c) != this->numberAlphabet.end() ? this->numberAlphabet[c] : LanguageToken::InvalidToken;
    }
    
    // Contains the operator of the language
    std::map<char, LanguageToken> operatorAlphabet ={
        {'+', LanguageToken::PlusToken},
        {'-', LanguageToken::MinusToken},
        {'=', LanguageToken::AssignmentToken},
        {';', LanguageToken::EndOfStatementToken},
        {':', LanguageToken::ColonToken},
        {'"', LanguageToken::QuoteToken},
        {'<', LanguageToken::LessThanToken},
        {'>', LanguageToken::GreaterThanToken},
        {'(', LanguageToken::OpenParenthesisToken},
        {')', LanguageToken::CloseParenthesisToken},
        {'!', LanguageToken::NotEqualToken}
    };
    
    // Check if the character is a valid operator
    LanguageToken isOperator(char c){
        return this->operatorAlphabet.find(c) != this->operatorAlphabet.end() ? this->operatorAlphabet[c] : LanguageToken::InvalidToken;
    }

    std::map<std::string, LanguageToken> LanguageKeywords ={
        {"if", LanguageToken::IfToken},
        {"integer", LanguageToken::TypeIntegerToken},
        {"double", LanguageToken::TypeDoubleToken},
        {"output", LanguageToken::OutputToken}
    };

    // Check if the character is a valid keyword
    LanguageToken isKeyword(std::string str){
        return this->LanguageKeywords.find(str) != this->LanguageKeywords.end() ? this->LanguageKeywords[str] : LanguageToken::InvalidToken;;
    }


// Others
private:
    void debugPrint(std::string str){
        if(this->isDebug){
            std::cout << str << "\n";
        }
    }
};

#endif // LEXICALANALYZER_H

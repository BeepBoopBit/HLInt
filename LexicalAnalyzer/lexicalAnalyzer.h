#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

// Standard Libraries
#include <iostream>
#include <fstream>
#include <string>

// Created Classes
#include "../SymbolTable/symbolTable.h"

class LexicalAnalyzer{

private:
    enum LanguageToken{
        CharacterToken,
        IdentifierToken,
        NumberToken,
        WhiteSpaceToken,
        PlusToken,
        MinusToken,
        EndLineToken,
        AssignmentToken,
        EndOfStatement,
        ColonToken,
        InvalidToken,
        QuoteToken,
        LeftShiftToken,
        LessThanToken,
        GreaterThanToken,
        IfToken,
        OpenParenthesisToken,
        CloseParenthesisToken,
        LiteralToken
    };
// Variables
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

        // Open the file
        this->file.open(filename);
    }
    ~LexicalAnalyzer();

// Methods
public:
    void analyze(){
        
        std::vector<LanguageToken> line_token;

        // Store the String
        std::string token;

        // Store the character that will be read
        char c;
        
        // While the file is open
        while(this->file.is_open()){
            // and the file is good
            while(this->file.good()){

                // Store the data from the current Character;
                file.get(c);
                LanguageToken tokenType;

                if (this->isDigit(c)){
                }
                if (this->isDigit(c)){
                }

                // Add Column Value
                this->column++;
            }
            this->file.close();
        }
    }

// Checkers
private:

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

   LanguageToken inAlphabet(char c){
        return this->alphabet.find(c) != this->alphabet.end() ? this->alphabet[c] : LanguageToken::InvalidToken;
    }
    
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
    LanguageToken isDigit(char c){
        return this->numberAlphabet.find(c) != this->numberAlphabet.end() ? this->numberAlphabet[c] : LanguageToken::InvalidToken;
    }
    
    std::map<char, LanguageToken> operatorAlphabet ={
        {'+', LanguageToken::PlusToken},
        {'-', LanguageToken::MinusToken},
        {'=', LanguageToken::AssignmentToken},
        {';', LanguageToken::EndOfStatement},
        {':', LanguageToken::ColonToken},
        {'"', LanguageToken::QuoteToken},
        {'<', LanguageToken::LessThanToken},
        {'>', LanguageToken::GreaterThanToken},
        {'(', LanguageToken::OpenParenthesisToken},
        {')', LanguageToken::CloseParenthesisToken}
    };

    LanguageToken isOperator(char c){
        return this->operatorAlphabet.find(c) != this->operatorAlphabet.end() ? this->operatorAlphabet[c] : LanguageToken::InvalidToken;
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

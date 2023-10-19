#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "../AbstractSyntaxTree/AbstractSyntaxTree.h"
#include "../LanguageDictionary/LanguageDictionary.h"
#include <string>

class Interpreter{
private:
    using LanguageToken = LanguageDictionary::LanguageToken;
private:
    Interpreter(){

    }
    ~Interpreter(){

    }
    Interpreter(const Interpreter&) = delete;
    Interpreter& operator=(const Interpreter&) = delete;
public:
    static Interpreter& getInstance(){
        static Interpreter instance;
        return instance;
    }

public:
    void interpret(AuxillaryTree* tree){
        if(tree == nullptr){
            return;
        }
        interpret(tree->_right);
        interpret(tree->_left);

        bool isCorrect = false;
        switch(tree->_token){
            case LanguageToken::CharacterToken:
            case LanguageToken::IdentifierToken:
            case LanguageToken::NumberToken:
            case LanguageToken::StringToken:
            case LanguageToken::NumberIntegerToken:
            case LanguageToken::NumberDoubleToken:
            case LanguageToken::LiteralToken:
            case LanguageToken::OutputToken:
                // This means that left and right is nulptr
                return;
                break;
            case LanguageToken::AdditionToken:
            case LanguageToken::SubtractionToken:
            case LanguageToken::MultiplicationToken:
                std::cout << getTreeValues(tree) << std::endl;
                break;
            case LanguageToken::LeftShiftToken:
            case LanguageToken::AssignmentToken:
            case LanguageToken::LessThanToken:
            case LanguageToken::GreaterThanToken:
            case LanguageToken::EqualityToken:
            case LanguageToken::NotEqualToken:
            case LanguageToken::ColonToken:
            case LanguageToken::IfToken:
            case LanguageToken::TypeIntegerToken:
            case LanguageToken::TypeDoubleToken:
                std::cout << getTreeValues(tree) << std::endl;
                break;

            // Non-Existent or Non-Essential Tokens
            case LanguageToken::RootNode:
                std::cout << "[DEBUG] Reach Root Node" << std::endl;
                break;
            case LanguageToken::OpenParenthesisToken:
                throw std::runtime_error("Open Parenthesis should not be in the Tree. Please Check the Lexer");
                break;
            case LanguageToken::CloseParenthesisToken:
                throw std::runtime_error("Close Parenthesis should not be in the Tree. Please Check the Lexer");
                break;
            case LanguageToken::QuoteToken:
                throw std::runtime_error("Quote Token Should not be in the Tree. Please Check the Lexer");
                break;
            case LanguageToken::InvalidToken:
                throw std::runtime_error("Invalid Token");
                break;
            default:
                break;
        }
        if(!isCorrect){
        }
    }
private:
    std::string getTreeValues(AuxillaryTree* &tree){
        if(tree == nullptr){
            return "";
        }
        return getTreeValues(tree->_left) + tree->_value + getTreeValues(tree->_right);
    }
};

#endif // INTERPRETER_H

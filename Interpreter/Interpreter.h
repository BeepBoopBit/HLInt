#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "../AbstractSyntaxTree/AbstractSyntaxTree.h"
#include "../LanguageDictionary/LanguageDictionary.h"
#include "../SymbolTable/symbolTable.h"
#include <string>

class Interpreter{
private:
    using LanguageToken = LanguageDictionary::LanguageToken;
private:
    Interpreter(){}
    ~Interpreter(){}
    Interpreter(const Interpreter&) = delete;
    Interpreter& operator=(const Interpreter&) = delete;
public:
    static Interpreter& getInstance(){
        static Interpreter instance;
        return instance;
    }
private:
    SymbolTable *_symbolTable = &SymbolTable::getInstance();
public:
    void interpret(AuxillaryTree* &tree){
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
                // This means that left and right is nullptr 
                return;
                break;
            case LanguageToken::AdditionToken:
            case LanguageToken::SubtractionToken:
            case LanguageToken::MultiplicationToken:
                deleteReplaceTree(tree, LanguageToken::NumberToken, getTreeValues(tree));
                break;
            case LanguageToken::AssignmentToken:
                //handleAssignment(tree);
                //break;
            case LanguageToken::LeftShiftToken:
                // LHS will always be the output keyword
                // RHS will always be a printable value
                // Call To Print Values
            case LanguageToken::LessThanToken:
            case LanguageToken::GreaterThanToken:
            case LanguageToken::EqualityToken:
            case LanguageToken::NotEqualToken:
                // Both LHS and RHS will always be a mathematical expression
                // Call to Evaluate Conditions
            case LanguageToken::ColonToken:
                // Pass
            case LanguageToken::IfToken:
                // LHS will always be a condition
                // RHS will always be a statement
                // Evaluate Before Passing
                break;
            case LanguageToken::TypeIntegerToken:
            case LanguageToken::TypeDoubleToken:
                handleDeclaration(tree);
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

// This assume that the tree is already evaluated and conforms to the language
private:
    void handleDeclaration(AuxillaryTree* &tree){
        // Tree Token wil always be its type
        // LHS will always be a colon operator
        // RHS will always be a nulptr
        // LHS->LHS will always be an identifier

        AuxillaryTree* lhs = tree->_left;
        AuxillaryTree* lhsLhs = lhs->_left;

        if(tree->_token == LanguageToken::TypeIntegerToken){
            ObjectTypeInt* variable = new ObjectTypeInt(lhsLhs->_value, 0);
            _symbolTable->declare(lhsLhs->_value, variable);
        }else if(tree->_token == LanguageToken::TypeDoubleToken){
            ObjectTypeDouble* variable = new ObjectTypeDouble(lhsLhs->_value, 0.0);
            _symbolTable->declare(lhsLhs->_value, variable);
        }
        std::cout << "[DEBUG] Declared Variable: " << lhsLhs->_value << std::endl;
    }

    void handleAssignment(AuxillaryTree* &tree){
        // LHS will always be an Identifier
        // RHS will always be a mathematical expression
    }
private:
    LanguageToken getNumberType(AuxillaryTree* &tree){
        if(tree->_token == LanguageToken::NumberIntegerToken){
            return LanguageToken::NumberIntegerToken;
        }else if(tree->_token == LanguageToken::NumberDoubleToken){
            return LanguageToken::NumberDoubleToken;
        }
        throw std::runtime_error("Invalid Number Type");
    }
private:
    void deleteReplaceTree(AuxillaryTree* &tree, LanguageToken token, std::string value){
        tree->_token = token;
        tree->_value = value;
        delete tree->_left;
        delete tree->_right;
        tree->_left = nullptr;
        tree->_right = nullptr;
    }
    std::string getTreeValues(AuxillaryTree* &tree){
        if(tree == nullptr){
            return "";
        }
        return getTreeValues(tree->_left) + tree->_value + getTreeValues(tree->_right);
    }
};

#endif // INTERPRETER_H

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
    LanguageDictionary* _languageDictionary = &LanguageDictionary::getInstance();

    // Temporary Support for One Way If
    bool _inIfScope = false;
public:
    void interpret(AuxillaryTree* &tree, bool isInterpretAll = false){
        if(tree == nullptr){
            return;
        }
        if(isInterpretAll){
            interpret(tree->_right);
            interpret(tree->_left);
        }

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
                {
                    std::string value = getTreeValues(tree);
                    deleteReplaceTree(tree, LanguageToken::NumberToken, value);
                    evaluateMathematicalExpression(tree);
                }
                break;
            case LanguageToken::AssignmentToken:
                handleAssignment(tree);
                break;
            case LanguageToken::LeftShiftToken:
                {
                    handleOutput(tree);
                    break;
                }
            case LanguageToken::LessThanToken:
            case LanguageToken::GreaterThanToken:
            case LanguageToken::EqualityToken:
            case LanguageToken::NotEqualToken:
            case LanguageToken::ColonToken:
                break;
            case LanguageToken::IfToken:
                handleIfCondition(tree);
                break;
            case LanguageToken::TypeIntegerToken:
            case LanguageToken::TypeDoubleToken:
                handleDeclaration(tree);
                break;

            // Non-Existent or Non-Essential Tokens
            case LanguageToken::RootNode:
#ifdef DEBUG
                std::cout << "[DEBUG] Reach Root Node" << std::endl;
#endif
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
    }

// This assume that the tree is already evaluated and conforms to the language
private:
    void handleIfCondition(AuxillaryTree* tree){
        // LHS will always be a condition
        // RHS will always be a statement
        
        AuxillaryTree* lhs = tree->_left;
        AuxillaryTree* rhs = tree->_right;

        _inIfScope = true;

        bool isTrue = handleCondition(lhs);

        if(isTrue){
            interpret(rhs, true);
        }else{
        }
        _inIfScope = false;
    }

    bool handleCondition(AuxillaryTree* tree){
        // LHS will always be a mathematical expression
        // RHS will always be a mathematical expression
        // Tree Token will always be a condition token

        AuxillaryTree* lhs = tree->_left;
        AuxillaryTree* rhs = tree->_right;
        
        if(lhs->_token == LanguageToken::StringToken){
            if(rhs->_token != LanguageToken::StringToken){
                throw std::runtime_error("Cannot compare string with non-string");
            }
            // Then it is a string comparison
            std::string lhsValue = lhs->_value;
            std::string rhsValue = rhs->_value;
            bool isTrue = false;
            switch(tree->_token){
                case LanguageToken::LessThanToken:
                    isTrue = lhsValue < rhsValue;
                    break;
                case LanguageToken::GreaterThanToken:
                    isTrue = lhsValue > rhsValue;
                    break;
                case LanguageToken::EqualityToken:
                    isTrue = lhsValue == rhsValue;
                    break;
                case LanguageToken::NotEqualToken:
                    isTrue = lhsValue != rhsValue;
                    break;
                default:
                    break;
            }
            return isTrue;
        }

        // Then it's an error
        if(rhs->_token == LanguageToken::StringToken){
            throw std::runtime_error("Cannot compare string with non-string");
        }

        interpret(lhs, true);
        interpret(rhs, true);
        double lhsValue = evaluateMathematicalExpression(lhs);
        double rhsValue = evaluateMathematicalExpression(rhs);
        bool isTrue = false;
        switch(tree->_token){
            case LanguageToken::LessThanToken:
                isTrue = lhsValue < rhsValue;
                break;
            case LanguageToken::GreaterThanToken:
                isTrue = lhsValue > rhsValue;
                break;
            case LanguageToken::EqualityToken:
                isTrue = lhsValue == rhsValue;
                break;
            case LanguageToken::NotEqualToken:
                isTrue = lhsValue != rhsValue;
                break;
            default:
                break;
        }
        return isTrue;
    }
    void handleOutput(AuxillaryTree* tree){
        // Tree Token will always be <<
        // LHS wll always be the output keyword
        // RHS will always be a printable value
        
        AuxillaryTree* rhs = tree->_right;
        if(rhs->_token == LanguageToken::StringToken){
            std::string value = rhs->_value;
            // Remove the quotes
            value = value.substr(1, value.size() - 2);
            std::cout << value << std::endl;
        }else{
            interpret(rhs, true);
            if(rhs->_token == LanguageToken::IdentifierToken){
                std::string value = rhs->_value;
                auto variable = _symbolTable->get(value);
                if(variable->getType() == "integer"){
                    ObjectTypeInt* variableInt = _symbolTable->parseToInt(variable);
                    std::cout << variableInt->getValue() << std::endl;
                }else if(variable->getType() == "double"){
                    ObjectTypeDouble* variableDouble = _symbolTable->parseToDouble(variable);
                    std::cout << variableDouble->getValue() << std::endl;
                }else if(variable->getType() == "string"){
                    ObjectTypeString* variableString = _symbolTable->parseToString(variable);
                    std::cout << variableString->getValue() << std::endl;
                }
                return;
            }
            std::string value = rhs->_value;
            double realValue = std::stod(value);
            std::cout << realValue << std::endl;
        }
    }
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

        if(_inIfScope){
            _inIfScope = false;
            _symbolTable->remove(lhsLhs->_value);
        }

    }

    void handleAssignment(AuxillaryTree* &tree){
        // Tree token is :=
        // LHS will always be an Identifier
        // RHS will always be a mathematical expression
        
        AuxillaryTree* lhs = tree->_left;
        AuxillaryTree* rhs = tree->_right;
        double realValue = evaluateMathematicalExpression(tree->_right);
        auto variable = _symbolTable->get(lhs->_value);
        if(variable == nullptr){
            throw std::runtime_error("Variable not declared");
        }
        if(variable->getType() == "integer"){
            ObjectTypeInt* variableInt = _symbolTable->parseToInt(variable);
            variableInt->setValue(realValue);
            this->_symbolTable->set(lhs->_value, variableInt);
        }else if(variable->getType() == "double"){
            ObjectTypeDouble* variableDouble = _symbolTable->parseToDouble(variable);
            variableDouble->setValue(realValue);
            this->_symbolTable->set(lhs->_value, variableDouble);
        }else if(variable->getType() == "string"){
            // Not Handled Correctly
            ObjectTypeString* variableString = _symbolTable->parseToString(variable);
            variableString->setValue(std::to_string(realValue));
            this->_symbolTable->set(lhs->_value, variableString);
        }
    }

    double evaluateMathematicalExpression(AuxillaryTree* &tree){
        // Tree token will always be a mathetical expression
        interpret(tree, true);
        std::string expression = tree->_value;
        
        char c = ' ';
        std::string token = "";
        double evaluatedValue = 0.0;
        
        // 0 = Addition
        // 1 = Subtraction
        // 2 = Multiplication
        // 3 = Division
        int typeOfOperation = 0;
        for(int i = 0; i < expression.length(); ++i){
            c = expression[i];
            token += c;
            if(token == "\""){
                throw std::runtime_error("Cannot perform mathematical operations on strings");
            }
            if(this->isDigit(token)){
                char tempC = c;
                tempC = expression[++i];
                while(this->isDigit(tempC) || tempC  == '.'){
                    tempC = expression[i];
                    token += tempC;
                    tempC = expression[++i];
                }
                if(typeOfOperation == 0){
                    evaluatedValue += std::stod(token);
                }else if(typeOfOperation == 1){
                    evaluatedValue -= std::stod(token);
                }else if(typeOfOperation == 2){
                    evaluatedValue *= std::stod(token);
                }else if(typeOfOperation == 3){
                    evaluatedValue /= std::stod(token);
                }
                --i;
            }
            else if(this->isIdentifier(token)){
                char tempC = c;
                tempC = expression[++i];
                while(this->isIdentifier(tempC)){
                    tempC = expression[i];
                    token += tempC;
                    tempC = expression[++i];
                }
                --i;
                auto variable = _symbolTable->get(token);
                if(variable == nullptr){
                    throw std::runtime_error("Missing Variable");
                }
                if(variable->getType() == "integer"){
                    ObjectTypeInt* variableInt = _symbolTable->parseToInt(variable);
                    if(typeOfOperation == 0){
                        evaluatedValue += variableInt->getValue();
                    }else if(typeOfOperation == 1){
                        evaluatedValue -= variableInt->getValue();
                    }else if(typeOfOperation == 2){
                        evaluatedValue *= variableInt->getValue();
                    }else if(typeOfOperation == 3){
                        evaluatedValue /= variableInt->getValue();
                    }
                }else if(variable->getType() == "double"){
                    ObjectTypeDouble* variableDouble = _symbolTable->parseToDouble(variable);
                    if(typeOfOperation == 0){
                        evaluatedValue += variableDouble->getValue();
                    }else if(typeOfOperation == 1){
                        evaluatedValue -= variableDouble->getValue();
                    }else if(typeOfOperation == 2){
                        evaluatedValue *= variableDouble->getValue();
                    }else if(typeOfOperation == 3){
                        evaluatedValue /= variableDouble->getValue();
                    }
                }
            }else if(token == "+"){
                typeOfOperation = 0;
            }else if(token == "-"){
                typeOfOperation = 1;
            }else if(token == "*"){
                typeOfOperation = 2;
            }else if(token == "/"){
                typeOfOperation = 3;
            }
            token ="";
        }
        tree->_value = std::to_string(evaluatedValue);
        return evaluatedValue;
    }
private:
    bool isDigit(std::string value){
        auto digits = _languageDictionary->getNumberAlphabet();
        return digits.find(value[0]) != digits.end();
    }
    bool isDigit(char value){
        auto digits = _languageDictionary->getNumberAlphabet();
        return digits.find(value) != digits.end();
    }
    bool isIdentifier(std::string value){
        auto alphabet = _languageDictionary->getAlphabet();
        return alphabet.find(value[0]) !=  alphabet.end();
    }
    bool isIdentifier(char value){
        auto alphabet = _languageDictionary->getAlphabet();
        return alphabet.find(value) != alphabet.end();
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

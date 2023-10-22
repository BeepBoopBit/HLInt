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
    SymbolTable*            _symbolTable            = &SymbolTable::getInstance();          // The symbol table
    LanguageDictionary*     _languageDictionary     = &LanguageDictionary::getInstance();   // The language dictionary

    // Temporary Support for One Way If
    // Will be removed after the implementation of scopes
    bool _inIfScope = false;

public:
    void interpret(AuxillaryTree* &tree, bool isInterpretAll = false){

        // If the tree is nullptr, then return
        if(tree == nullptr){
            return;
        }

        // If we want to interpret all subtrees, then we will interpret the left and right subtree
        if(isInterpretAll){
            interpret(tree->_left, true);
            interpret(tree->_right, true);
        }

        // Otherwise, we will interpret the current subtree
        bool isCorrect = false;                                                             // Signifies if the tree is correct
        switch(tree->_token){
            case LanguageToken::CharacterToken:
            case LanguageToken::IdentifierToken:
            case LanguageToken::NumberToken:
            case LanguageToken::StringToken:
            case LanguageToken::NumberIntegerToken:
            case LanguageToken::NumberDoubleToken:
            case LanguageToken::LiteralToken:
            case LanguageToken::OutputToken:
            case LanguageToken::InputToken:
                // This means that left and right is nullptr 
                return;
                break;
            case LanguageToken::AdditionToken:
            case LanguageToken::SubtractionToken:
            case LanguageToken::MultiplicationToken:
            case LanguageToken::DivisionToken:
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
                handleOutput(tree);
                break;
            case LanguageToken::RightShiftToken:
                handleInput(tree);
                break;
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
            case LanguageToken::TypeStringToken:
                handleDeclaration(tree);
                break;
            default:
                throw std::runtime_error("Token: " + tree->_value + ". Either the token is not implemented or there's a problem with the AST");
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

            return evaluateComparison<std::string>(tree->_token, lhs->_value, rhs->_value);
        }

        // Then it's an error
        if(rhs->_token == LanguageToken::StringToken){
            throw std::runtime_error("Cannot compare string with non-string");
        }
        
        // Interpret LHS and RHS to get the smallest unit of value
        interpret(lhs, true);
        interpret(rhs, true);
        return evaluateComparison(tree->_token, lhs->_value, rhs->_value);
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
    void handleInput(AuxillaryTree* &tree){
        // Tree token will always be >> operator
        // LHS will always be the input keyword
        // RHS will always be an identifier
        AuxillaryTree* rhs = tree->_right;
        std::string value;
        std::getline(std::cin, value);
        std::string identifier = rhs->_value;
        auto variable = _symbolTable->get(identifier);
        if(variable->getType() == "integer"){
            ObjectTypeInt* variableInt = _symbolTable->parseToInt(variable);
            try{
                variableInt->setValue(std::stoi(value));
            }catch(std::invalid_argument& e){
                throw std::runtime_error("Cannot convert input to integer");
            }
        }else if(variable->getType() == "double"){
            ObjectTypeDouble* variableDouble = _symbolTable->parseToDouble(variable);
            try{
                variableDouble->setValue(std::stod(value));
            }catch(std::invalid_argument& e){
                throw std::runtime_error("Cannot convert input to double");
            }
        }else if(variable->getType() == "string"){
            ObjectTypeString* variableString = _symbolTable->parseToString(variable);
            variableString->setValue(value);
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
        }else if(tree->_token == LanguageToken::TypeStringToken){
            ObjectTypeString* variable = new ObjectTypeString(lhsLhs->_value, "");
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
        if(variable->getType() == "integer"){
            ObjectTypeInt* variableInt = _symbolTable->parseToInt(variable);
            variableInt->setValue(realValue);
            this->_symbolTable->set(lhs->_value, variableInt);
        }else if(variable->getType() == "double"){
            ObjectTypeDouble* variableDouble = _symbolTable->parseToDouble(variable);
            variableDouble->setValue(realValue);
            this->_symbolTable->set(lhs->_value, variableDouble);
        }else if(variable->getType() == "string"){
            ObjectTypeString* variableString = _symbolTable->parseToString(variable);
            variableString->setValue(std::to_string(realValue));
            this->_symbolTable->set(lhs->_value, variableString);
        }
    }
    
    double evaluateMathematicalExpression(AuxillaryTree* &tree){
        // Tree token will always be a mathetical expression
        interpret(tree, true);
        std::string     expression      = tree->_value;         // The expression to be evaluated
        char            c               = ' ';                  // The current character being evaluated
        std::string     token           = "";                   // The current token being evaluated
        double          evaluatedValue  = 0.0;                  // The value of the expression
        bool            nextIsASign     = true;                 // Whether the next token is a sign
        int             signModifier    = 1;                    // The sign modifier
        bool            isString        = false;                // Whether the expression is a string

        // 0 = Addition
        // 1 = Subtraction
        // 2 = Multiplication
        // 3 = Division
        int typeOfOperation = 0;
        for(int i = 0; i < expression.length(); ++i){
            c = expression[i];
            token += c;
            if(token == "\""){
                isString = true;
            }else if(this->isDigit(token)){
                token = getValueUntilNot(expression, i, &Interpreter::isDigit, true);
                evaluateValue(evaluatedValue, std::stod(token)*signModifier, typeOfOperation);
            }
            else if(this->isIdentifier(token)){
                token = getValueUntilNot(expression, i, &Interpreter::isIdentifier);
                auto variable = _symbolTable->get(token);
                if(variable->getType() == "integer"){
                    ObjectTypeInt* variableInt = _symbolTable->parseToInt(variable);
                    evaluateValue(evaluatedValue, (double)variableInt->getValue()*signModifier, typeOfOperation);
                    nextIsASign = false;
                }else if(variable->getType() == "double"){
                    ObjectTypeDouble* variableDouble = _symbolTable->parseToDouble(variable);
                    evaluateValue(evaluatedValue, variableDouble->getValue()*signModifier, typeOfOperation);
                    nextIsASign = false;
                }
            }else if(token == "+"){
                if(nextIsASign) { signModifier = 1; }
                else            { typeOfOperation = 0; }
                nextIsASign = true;
            }else if(token == "-"){
                if(nextIsASign) { signModifier = -1; }
                else            { typeOfOperation = 1; }
                nextIsASign     = true;
            }else if(token == "*"){
                typeOfOperation = 2;
                signModifier    = 1;
                nextIsASign = true;
            }else if(token == "/"){
                typeOfOperation = 3;
                signModifier    = 1;
                nextIsASign     = true;
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
        if(tree->_left != nullptr){
            delete tree->_left;
        }
        if(tree->_right != nullptr){
            delete tree->_right;
        }
        tree->_left = nullptr;
        tree->_right = nullptr;
    }
    std::string getTreeValues(AuxillaryTree* &tree){
        if(tree == nullptr){
            return "";
        }
        return getTreeValues(tree->_left) + tree->_value + getTreeValues(tree->_right);
    }
    std::string getValueUntilNot(std::string &expression, int &index, bool (Interpreter::*func)(char), bool allowDecimal = false){
        std::string token = "";
        char c = expression[index];
        while((this->*func)(c) || (allowDecimal && c == '.')){
            token += c;
            c = expression[++index];
        }
        --index;
        return token;
    }
    template<typename T>
    bool evaluateComparison(LanguageToken &token, T lhsValue, T rhsValue){
        switch(token){
            case LanguageToken::LessThanToken:
                return lhsValue < rhsValue;
            case LanguageToken::GreaterThanToken:
                return lhsValue > rhsValue;
            case LanguageToken::EqualityToken:
                return lhsValue == rhsValue;
            case LanguageToken::NotEqualToken:
                return lhsValue != rhsValue;
            default:
                break;
        }
        throw std::runtime_error("Invalid Comparison");
        return false;
    }
    
    template<typename T>
    void evaluateValue(T &total, T rhsValue, int typeOfOperation){
        switch(typeOfOperation){
            case 0:
                total += rhsValue;
                break;
            case 1:
                total -= rhsValue;
                break;
            case 2:
                total *= rhsValue;
                break;
            case 3:
                total /= rhsValue;
                break;
            default:
                break;
        }
    }

    
};

#endif // INTERPRETER_H

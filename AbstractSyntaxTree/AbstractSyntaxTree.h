#ifndef ABSTRACTSYNTAXTREE_H
#define ABSTRACTSYNTAXTREE_H

#include "../LanguageDictionary/LanguageDictionary.h"
#include "../SymbolTable/symbolTable.h"
#include "../ErrorHandler/ErrorHandler.h"
#include <vector>
#include <string>
#include <iostream>

/** Entended Backus-Naur Form for HLint
 *
 * Letter := a | b | c | d | e | f | g | h | i | j | k | l | m | n | o | p | q | r | s | t | u | w | x | y | z
 * Digit := 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
 * WhiteSpace := ' '
 * End-Of-Statement := ';'
 *
 * Character := Letter | Digit 
 * Number := Digit {Digit}
 * Literal := '"' {Character} '"' | Number
 *
 * Identifier := letter, {letter | digit | '_'}, End-Of-Statement
 * Mathematical-Operator := '+' | '-'
 * Mathematical-Expression := Identifier, Mathematical-Operator, Identifier, (Mathematical-Operator, Identifier)*
 *
 * Assignment := Identifier, ':=', (Identifier | Literal | Mathematical-Expression), End-Of-Statement
 * Type := 'integer' | 'float'
 * Declaration := Identifier, ':', Type, End-Of-Statement
 * Output := 'Output', '<<', (Identifier | Literal), End-Of-Statement
 * Condition := (Identifier | Literal), ('<' | '>' | '==' | '!=') , (Identifier | Literal)
 * Statement := Assignment | Declaration | Output
 * One-Way-If-Condition := 'if', '(', Condition, ')', Statement, End-Of-Statement
**/

class AuxillaryTree{
private:
    using LanguageToken = LanguageDictionary::LanguageToken;

public:
    LanguageToken _token = LanguageToken::InvalidToken;
    AuxillaryTree* _left = nullptr;
    AuxillaryTree* _right = nullptr;
    std::string _value = "";

    int _line = 0;
    int _column = 0;

public:
    AuxillaryTree(LanguageToken token, std::string value, int line, int column): _token(token), _value(value), _line(line), _column(column){
    }
    ~AuxillaryTree(){}
};


// ################################################################################################

class AST{

private:
    using LanguageToken = LanguageDictionary::LanguageToken;
    ErrorHandler* _errorHandler = &ErrorHandler::getInstance();

// Constructors and Deconstructor
private:
    AST(){}
    ~AST(){}


// Other Important Methods
public:
    // SIngleton
    static AST& getInstance(){

        // Create the Singleton Instance
        static AST instance;
        
        // Return the Instance
        return instance;
    }

    // Remove copy constructor and assignment operator
    AST(AST const&) = delete;
    void operator=(AST const&) = delete;

// Owned
private:
    LanguageDictionary* _languageDictionary = &LanguageDictionary::getInstance();;
    SymbolTable* _symbolTable = &SymbolTable::getInstance();
    int _parenthesisCount = 0;
private:
    // Main Tree
    AuxillaryTree *_root = new AuxillaryTree(LanguageToken::RootNode, "RootNode",0,0);

    // Tree Totality
    std::vector<AuxillaryTree*> _totalityTree;

    // Small Trees
    std::vector<AuxillaryTree*> _smallTrees;
    int _currentSmallTreeIndex = 0;

    // latest Small Tree Index
    AuxillaryTree* _latestSmallTree = nullptr;

    bool _isConditional = false;

    int _currentLine = 0;
    int _currentColumn = 0;

public:
    // Value can be empty
    void insert(LanguageToken token, std::string value = "", int line = 0, int column = 0){
        
        // Set the current line and column
        _currentLine = line;
        _currentColumn = column;

        bool isConditional = false;
        if((token == LanguageToken::OpenParenthesisToken) || (isConditional = isConditionalOperator(value))){
            if(!isConditional){
                ++_parenthesisCount;
            }

            if(_latestSmallTree == nullptr && isConditional){
                _smallTrees.push_back(new AuxillaryTree(token,value, line, column));
                return;
            }else if(_latestSmallTree == nullptr){
                return; 
            }
            
            // Push the latest small tree to it
            _smallTrees.push_back(_latestSmallTree);

            // Set the latest small tree to null
            _latestSmallTree = nullptr;

            if((_isConditional = isConditional)){
                _latestSmallTree = new AuxillaryTree(token, value, line, column);
                _smallTrees.push_back(_latestSmallTree);
                _latestSmallTree = nullptr;
            }

            return;

        }else if(token == LanguageToken::CloseParenthesisToken){
            if(--_parenthesisCount < 0){
                _errorHandler->addError("Parenthesis count is less than 0");
                //throw std::runtime_error("Parenthesis count is less than 0");
            }

            if(_smallTrees.empty()){
                return;
            }

            AuxillaryTree* lastSmallTree = _smallTrees.back();
            bool isConditional = this->isConditionalOperator(lastSmallTree->_value);
            bool isDoubleOperator = this->isDoubleOperator(lastSmallTree->_value);
            if(isDoubleOperator && !isConditional){
                _smallTrees.push_back(_latestSmallTree);
                _latestSmallTree = nullptr;
                return;
            }
            if(_parenthesisCount > 0){
                return;
            }
            _smallTrees.pop_back();
            if(lastSmallTree->_left != nullptr && lastSmallTree->_right != nullptr){
                if(_latestSmallTree->_left == nullptr){
                    _latestSmallTree->_left = lastSmallTree;
                }else if(_latestSmallTree ->_right ==nullptr){
                    _latestSmallTree->_right = lastSmallTree;
                }
                //_latestSmallTree = lastSmallTree;
                _smallTrees.push_back(_latestSmallTree);
                _latestSmallTree = nullptr;
                return;
            }
            else{
                if(lastSmallTree->_right ==nullptr){
                    lastSmallTree->_right = _latestSmallTree;
                }
                else{
                    lastSmallTree->_left = _latestSmallTree;
                }
            }
            if(_isConditional){
                AuxillaryTree* lhs_value =  _smallTrees.back();
                _smallTrees.pop_back();
                lastSmallTree->_left = lhs_value;
                _isConditional = false;

                AuxillaryTree* newLastSmallTree = _smallTrees.back();
                if(newLastSmallTree->_left == nullptr){
                    newLastSmallTree->_left = lastSmallTree;
                }else{
                    AuxillaryTree *tempTree = newLastSmallTree;
                    while(tempTree->_right != nullptr){
                        tempTree = tempTree->_right;
                    }
                    tempTree->_right = lastSmallTree;
                }
                lastSmallTree = newLastSmallTree;
                _smallTrees.pop_back();

            }
            _latestSmallTree = lastSmallTree;
            _smallTrees.push_back(_latestSmallTree);
            _latestSmallTree = nullptr;
            return;
        }
       
        // If the token is end of statement, then we need to merge the small trees and append it to the latest node of the main tree
        if(token == LanguageToken::EndOfStatementToken){
            if(_parenthesisCount != 0){
                _errorHandler->addError("Parenthesis count is not 0");
                //throw std::runtime_error("Parenthesis count is not 0");
            }
            while(!_smallTrees.empty()){
                AuxillaryTree* tempTree = _smallTrees.back();
                if(_latestSmallTree == nullptr){
                    _latestSmallTree = tempTree;
                    _smallTrees.pop_back();
                    continue;
                }
                if(tempTree->_left == nullptr){
                    tempTree->_left = _latestSmallTree;
                }else{
                    AuxillaryTree *newTempTree = tempTree;
                    bool newTempTreeChanged = false;
                    while(newTempTree->_right != nullptr){
                        newTempTree = newTempTree->_right;
                    }

                    if(this->isLiteralOrIdentifier(newTempTree)){
                        if((newTempTree = findValidTree(_latestSmallTree)) == nullptr){
                            _errorHandler->addError("Invalid tree found");
                            //throw std::runtime_error("Invalid tree found");
                        }
                        newTempTreeChanged = true;
                    }
                    if(newTempTreeChanged){
                        if(newTempTree->_right == nullptr){
                            newTempTree->_right = tempTree;
                        }
                        else{
                            newTempTree->_left = tempTree;
                        }
                        tempTree = newTempTree;
                    }
                    else{
                        if(tempTree->_right == nullptr){
                            tempTree->_right = _latestSmallTree;
                        }else{
                            tempTree->_left = _latestSmallTree;
                        }

                    }
               }
                _latestSmallTree = tempTree;
                _smallTrees.pop_back();
            }
            _root = _latestSmallTree;
            _totalityTree.push_back(_latestSmallTree);
            _latestSmallTree = nullptr;
            if(evaluateTree(_root)){
                //std::cout << "[/] Statement is good" << std::endl;
            }
            else{
                //std::cout << "[!] Statement is bad" << std::endl;
            }
            return;
        }

        if(_latestSmallTree == nullptr){
            _latestSmallTree = new AuxillaryTree(token, value, line, column);
            return;
        }
 

        if(isOperator(value) || isKeyword(value)){
            _latestSmallTree = processOperator(token, value, _latestSmallTree);
            if(this->isDoubleOperator(_latestSmallTree->_value)){
                _smallTrees.push_back(_latestSmallTree);
                _latestSmallTree = nullptr;
            }
        }
        else{
            if(this->isDoubleOperator(_latestSmallTree->_value) || this->isKeyword(_latestSmallTree->_value)){
                _smallTrees.push_back(_latestSmallTree);
                _latestSmallTree = new AuxillaryTree(token, value, line, column);
            }
            else if(_latestSmallTree->_left == nullptr){
                _latestSmallTree->_left = new AuxillaryTree(token, value, line, column);
            }else{
                AuxillaryTree *tempTree = _latestSmallTree;
                while(tempTree->_right != nullptr){
                    tempTree = tempTree->_right;
                }
                tempTree->_right = new AuxillaryTree(token, value, line, column);
            }
        }
    }

    void print(){
        for(int i = 0; i < _totalityTree.size(); ++i){
            std::cout << "\n#################### Totality [" << i << "] ####################\n";
            printBT(_totalityTree[i]);
            std::cout << "#################### Summary [" << i << "] ####################\n";
            std::cout << summarizeTree(_totalityTree[i]) << '\n';
            std::cout << "#################### Summarized [" << i << "] ####################\n";
        }
        std::cout << "TESTING" << std::endl;
    }

    std::vector<AuxillaryTree*> getTrees(){
        return _totalityTree;
    }

    AuxillaryTree* findValidTree(AuxillaryTree* &tree){
        if (tree == nullptr){
            return nullptr;
        }
        // Traverse until we find a valid tree
        AuxillaryTree* rhs = nullptr;
        if(!isLiteralOrIdentifier(tree->_right)){
            rhs = findValidTree(tree->_right);
            if(rhs != nullptr){
                return rhs;
            }
        }

        AuxillaryTree* lhs = nullptr;
        if(!isLiteralOrIdentifier(tree->_left)){
            lhs = findValidTree(tree->_left);
            if(lhs != nullptr){
                return lhs;
            }
        }

        if(tree->_left == nullptr || tree->_right == nullptr){
                return tree;
        }
        return nullptr;
    }


    AuxillaryTree* processOperator(LanguageToken token, std::string value, AuxillaryTree* tree){

        bool tokenIsOperatorSize = tree->_value.length() == 1;
        bool tokenIsOperator = isOperator(tree->_value);
        bool notAKeyword = !isKeyword(tree->_value);
        bool notAnOperator = !tokenIsOperatorSize && !tokenIsOperator;
        bool notAConditional = !isConditionalOperator(tree->_value);
        bool isDoubleOperator = this->isDoubleOperator(tree->_value);

        // If the token on the latest small node is not an operator or a keyword
        // notAnOperator && notAKeyword && notAConditional
        if((notAnOperator && notAConditional) || isDoubleOperator){

            AuxillaryTree* newTree = tree;
            tree = new  AuxillaryTree(token, value, tree->_line, tree->_column);
            tree->_left = newTree;
            // Create a new tree on the latest small tree base on the latest small tree
            //tree->_left = new AuxillaryTree(tree->_token, tree->_value);
            
            // Update the latest small tree token and value to reflect the new value
            //tree->_token = token;
            //tree->_value = value;
        }else{
            if(token == LanguageToken::MultiplicationToken){
                AuxillaryTree* tempTree = tree->_right;
                AuxillaryTree* newTree = new AuxillaryTree(token, value, tree->_line, tree->_column);
                newTree->_left = tempTree;
                tree->_right = newTree;

            }else{
                AuxillaryTree* newTree = new AuxillaryTree(token, value, tree->_line, tree->_column);
                newTree->_left = tree;
                tree = newTree;
                return tree;
            }

        }
        return tree;
    }
private:
    std::string summarizeTree(AuxillaryTree* &tree){
        std::string total_string = "";
        if(tree == nullptr){
            return total_string;
        }
        total_string += summarizeTree(tree->_left);
        total_string += tree->_value;
        total_string += summarizeTree(tree->_right);
        return total_string;
    }

    void summarizeTreeValue(AuxillaryTree* &tree){
        if(tree == nullptr){
            return;
        }
        summarizeTreeValue(tree->_left);
        summarizeTreeValue(tree->_right);
    }
 
private:
    bool evaluateTree(AuxillaryTree*& tree){
        if(tree == nullptr){
            return true;
        }
        //std::cout << "[PROCESSING] " << tree->_value << '\n';
        bool process = processEvaluation(tree);
        bool lhs = evaluateTree(tree->_left);
        bool rhs = evaluateTree(tree->_right);

        if(!process || !lhs || !rhs){
            return false;
        }
        return true;
    }

    bool processEvaluation(AuxillaryTree* &tree){
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
                if(this->expect(tree, &AST::isNull)){
                    isCorrect = true;
                }
                break;
            case LanguageToken::AdditionToken:
            case LanguageToken::SubtractionToken:
            case LanguageToken::MultiplicationToken:
            case LanguageToken::AssignmentToken:
            case LanguageToken::LessThanToken:
            case LanguageToken::GreaterThanToken:
            case LanguageToken::EqualityToken:
            case LanguageToken::NotEqualToken:
                if(this->expect(tree, &AST::isMathematical)){
                    isCorrect = true;
                }
                break;
            case LanguageToken::ColonToken:
                // Consider only the left side of the tree
                //this->expect(tree, &AST::isIdentifier, 1);
                if(this->expect(tree, &AST::isIdentifier, &AST::isNull)){
                    isCorrect = true;
                }
                break;
            case LanguageToken::LeftShiftToken:
                //this->expect(tree, &AST::isPrintable);
                if(this->expect(tree, &AST::isKeyword ,&AST::isPrintable)){
                    isCorrect = true;
                }

                break;
            case LanguageToken::IfToken:
                //this->expect(tree, &AST::isConditional, &AST::isAStatement);
                if(this->expect(tree, &AST::isConditional, &AST::isAStatement)){
                    isCorrect = true;
                }
                break;
            case LanguageToken::TypeIntegerToken:
            case LanguageToken::TypeDoubleToken:
                // Consider only the left side of the tree
                //this->expect(tree, &AST::declarable, 1);
                if(this->expect(tree, &AST::declarable,1)){
                    isCorrect = true;
                }
                break;
            // Non-Existent or Non-Essential Tokens
            case LanguageToken::EqualToken:
                throw std::runtime_error("Equal Token is not a valid token. Please Check the Lexer");
                break;
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
            _errorHandler->addError(tree->_value, tree->_line, tree->_column);
            return false;
        }
        return true;
    }

    void evaluateTree(){
        for(int i = 0; i < _totalityTree.size(); ++i){
            std::cout << "\n#################### Evaluate [" << i << "] ####################\n";
            evaluateTree(_totalityTree[i]);
            std::cout << "#################### Evaluate [" << i << "] ####################\n";
        }
    }

    bool expect(AuxillaryTree* tree, bool (AST::*function)(AuxillaryTree*), int pathway = 2){
        if(pathway == 3){
            if((this->*function)(tree->_left) || (this->*function)(tree->_right)){
                return true;
            }
        }
        else if(pathway == 2){
            if((this->*function)(tree->_left) && (this->*function)(tree->_right)){
                return true;
            }
        }else if(pathway == 1){
            if((this->*function)(tree->_left)){
                return true;
            }
        }else if(pathway == 0){
            if((this->*function)(tree->_right)){
                return true;
            }
        }
        
        return false;
    }

    bool expect(AuxillaryTree* tree, bool (AST::*lhs_func)(AuxillaryTree*), bool (AST::*rhs_func)(AuxillaryTree*)){
        if((this->*lhs_func)(tree->_left) && (this->*rhs_func)(tree->_right)){
            return true;
        }
        return false;
    }

private:
    bool isLiteralOrIdentifier(AuxillaryTree* tree){
        if(tree == nullptr){
            return false;
        }
        bool firstRule = tree->_token == LanguageToken::LiteralToken;
        bool secondRule = tree->_token == LanguageToken::IdentifierToken;
        bool thirdRule = tree->_token == LanguageToken::NumberIntegerToken;
        bool fourthRule = tree->_token == LanguageToken::NumberDoubleToken;
        if(firstRule || secondRule || thirdRule || fourthRule){
            return true;
        }
        return false;
    }
    bool isIdentifier(AuxillaryTree* tree){
        if(tree == nullptr){
            return false;
        }
        bool firstRule = tree->_token == LanguageToken::IdentifierToken;
        if(firstRule){
            return true;
        }
        return false;
    }
    bool isConditional(AuxillaryTree* tree){
        if(tree == nullptr){
            return false;
        }
        bool firstRule = tree->_token == LanguageToken::LessThanToken;
        bool secondRule = tree->_token == LanguageToken::GreaterThanToken;
        bool thirdRule = tree->_token == LanguageToken::EqualityToken;
        bool fourthRule = tree->_token == LanguageToken::NotEqualToken;
        if(firstRule || secondRule || thirdRule || fourthRule){
            return true;
        }
        return false;
    }
    bool isAStatement(AuxillaryTree* tree){
        if(tree == nullptr){
            return false;
        }
        bool firstRule = tree->_token == LanguageToken::AssignmentToken;
        bool secondRule = tree->_token == LanguageToken::ColonToken;
        bool thirdRule = tree->_token == LanguageToken::LeftShiftToken;
        bool fourthRule = tree->_token == LanguageToken::IfToken;
        if(firstRule || secondRule || thirdRule || fourthRule){
            return true;
        }
        return false;
    }
    bool isNull(AuxillaryTree* tree){
        if(tree == nullptr){
            return true;
        }
        return false;
    }

    bool isMathematical(AuxillaryTree *tree){
        if(tree == nullptr){
            return false;
        }
        bool firstRule = tree->_token == LanguageToken::LiteralToken;
        bool secondRule = tree->_token == LanguageToken::IdentifierToken;
        bool thirdRule = tree->_token == LanguageToken::NumberIntegerToken;
        bool fourthRule = tree->_token == LanguageToken::NumberDoubleToken;
        bool fifthRule = tree->_token == LanguageToken::AdditionToken;
        bool sixthRule = tree->_token == LanguageToken::SubtractionToken;
        bool seventhRule = tree->_token == LanguageToken::MultiplicationToken;
        // We can add Strings
        bool eightRule= tree->_token == LanguageToken::StringToken;
        if(firstRule || secondRule || thirdRule || fourthRule || fifthRule || sixthRule || seventhRule || eightRule){
            return true;
        }
        return false;
    }

    bool isPrintable(AuxillaryTree *tree){
        if(tree == nullptr){
            return false;
        }
        bool firstRule = isMathematical(tree);
        bool secondRule = tree->_token == LanguageToken::StringToken;
        if(firstRule || secondRule){
            return true;
        }
        return false;
    }

    bool declarable(AuxillaryTree *tree){
        if(tree == nullptr){
            return false;
        }

        bool firstRule = tree->_token == LanguageToken::ColonToken;
        bool secondRule = tree->_left->_token == LanguageToken::IdentifierToken;
        if (firstRule && secondRule){
            return true;
        }
        return false;
    }
    
    bool isKeyword(AuxillaryTree* tree){
        if(tree == nullptr){
            return false;
        }
        bool firstRule = tree->_token == LanguageToken::IfToken;
        bool secondRule = tree->_token == LanguageToken::OutputToken;
        if(firstRule || secondRule){
            return true;
        }
        return false;
    }
    bool isComparable(AuxillaryTree* tree){
        if(tree == nullptr){
            return false;
        }
        bool firstRule = isMathematical(tree);
        bool secondRule = tree->_token == LanguageToken::StringToken;
        if(firstRule || secondRule){
            return true;
        }
        return false;
    }

private:

    // Reference: https://stackoverflow.com/questions/36802354/print-binary-tree-in-a-pretty-way-using-c
    void printBT(const std::string& prefix, const AuxillaryTree* node, bool isLeft)
    {
        if( node != nullptr )
        {
            std::cout << prefix;

            std::cout << (isLeft ? "├──" : "└──" );

            // print the value of the node
            std::cout << node->_value << std::endl;

            // enter the next tree level - left and right branch
            printBT( prefix + (isLeft ? "│   " : "    "), node->_left, true);
            printBT( prefix + (isLeft ? "│   " : "    "), node->_right, false);
        }
    }

    void printBT(const AuxillaryTree* node)
    {
        printBT("", node, false);    
    }

    bool isDoubleOperator(std::string value){
        auto alphabet = _languageDictionary->getDoubleOperator();
        return alphabet.find(value) != alphabet.end();

    }
    bool isOperator(std::string value){
        auto alphabet = _languageDictionary->getOperatorAlphabet();
        return alphabet.find(value) != alphabet.end();
    }

    bool isKeyword(std::string value){
        auto alphabet = _languageDictionary->getLanguageKeywords();
        return alphabet.find(value) != alphabet.end();
    }

    bool isConditionalOperator(std::string value){
        auto alphabet = _languageDictionary->getConditionalOperator();
        return alphabet.find(value) != alphabet.end();
    }
};

#endif // ABSTRACTSYNTAXTREE_h


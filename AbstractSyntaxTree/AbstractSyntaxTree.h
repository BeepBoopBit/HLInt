#ifndef ABSTRACTSYNTAXTREE_H
#define ABSTRACTSYNTAXTREE_H

#include "../LanguageDictionary/LanguageDictionary.h"
#include "../SymbolTable/symbolTable.h"
#include "../ErrorHandler/ErrorHandler.h"
#include "AuxillaryTree.h"

// Standard Library
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


class AST{

// Constructors and Deconstructor
private:
    AST(){}
    ~AST(){}

// Implementing Singleton
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
    using               LanguageToken           = LanguageDictionary::LanguageToken;        // Used to Simplify the Code
    ErrorHandler*       _errorHandler           = &ErrorHandler::getInstance();             // Used to log errors
    LanguageDictionary* _languageDictionary     = &LanguageDictionary::getInstance();       // Used to get the tokens
    SymbolTable*        _symbolTable            = &SymbolTable::getInstance();             // Used to store the variables
    int                 _parenthesisCount       = 0;                                       // Used to check if the parenthesis are balanced


private:
    AuxillaryTree*                  _root                   = nullptr;                          // The root of the tree
    std::vector<AuxillaryTree*>     _totalityTree;                                              // The totality tree. Contains all the statement trees from all lines
    std::vector<AuxillaryTree*>     _smallTrees;                                                // The small trees. Contains the statement trees from a single line
    AuxillaryTree*                  _latestSmallTree        = nullptr;                          // The latest small tree. Used to push the small tree to the totality tree
    int                             _currentSmallTreeIndex  = 0;                                // The current small tree index. Used to push the small tree to the totality tree
    int                             _line                   = 0;                                // The current line. Used for better error handling
    int                             _column                 = 0;                                // The current column. Used for better error handling
    bool                            _isConditional          = false;                            // Used to check if the current small tree is a conditional statement



public:
    // Value can be empty
    void insert(LanguageToken token, std::string value = "", int line = 0, int column = 0){
        
        // Set the current line and column
        _line = line;
        _column = column;

        bool isConditional = false;
        if((token == LanguageToken::OpenParenthesisToken) || (isConditional = isConditionalOperator(value))){
            processParenthesisOrOperator(token, value, isConditional);
            return;
        }else if(token == LanguageToken::CloseParenthesisToken){
            processCloseParenthesis(token, value);
            return;
        }
        // If the token is end of statement, then we need to merge the small trees and append it to the latest node of the main tree
        if(token == LanguageToken::EndOfStatementToken){
            processEndOfStatement();
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
            }else if(_smallTrees.size() > 1){
                if(_latestSmallTree->_right== nullptr){
                    _latestSmallTree->_right= new AuxillaryTree(token, value, line, column);
                }else if(_latestSmallTree->_left == nullptr){
                    _latestSmallTree->_left = new AuxillaryTree(token, value, line, column);
                }
                else{
                    AuxillaryTree *tempTree = _latestSmallTree;
                    while(tempTree->_right != nullptr){
                        tempTree = tempTree->_right;
                    }
                    tempTree->_right = new AuxillaryTree(token, value, line, column);
                }
            }else{
                if(_latestSmallTree->_left == nullptr){
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
    }

    void evaluateTree(){
        for(int i = 0; i < _totalityTree.size(); ++i){
#ifdef DEBUG
            std::cout << "\nEvaluate [" << i << "]\n";
#endif
            if(evaluateTree(_totalityTree[i])){
#ifdef DEBUG
                std::cout << "[/] Statement is good" << std::endl;
#endif
            }
        }
    }

// Others
public:

    
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

// Auxillary methods
private:
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


// Processes
private:
    AuxillaryTree* processOperator(LanguageToken token, std::string value, AuxillaryTree* tree){
        
        // Rules for opeartors and keywords
        bool tokenIsOperatorSize = tree->_value.length() == 1;
        bool tokenIsOperator = isOperator(tree->_value);
        bool notAKeyword = !isKeyword(tree->_value);
        bool notAnOperator = !tokenIsOperatorSize && !tokenIsOperator;
        bool notAConditional = !isConditionalOperator(tree->_value);
        bool isDoubleOperator = this->isDoubleOperator(tree->_value);

        // If the token on the latest small node is not an operator or a keyword
        // notAnOperator && notAKeyword && notAConditional
        if((notAnOperator && notAConditional) || isDoubleOperator){

            // Then we need to replace the latest small tree with the new tree as its parent 
            AuxillaryTree* newTree = tree;
            tree = new  AuxillaryTree(token, value, tree->_line, tree->_column);
            tree->_left = newTree;
        }else{
            // Otherwise, check what kind of operation we are dealing with
            if(this->isMultiplicationOrDivision(token) && (!this->isMultiplicationOrDivision(_latestSmallTree->_token))){
            //if(token == LanguageToken::MultiplicationToken){
                
                // Go the left most side of the tree
                AuxillaryTree* tempTreeParent = tree;
                AuxillaryTree* tempTree = tree->_right;
                if(tempTree == nullptr){
                    // Otherwise, we need make the new created tree the parent of the latest small tree
                    AuxillaryTree* newTree = new AuxillaryTree(token, value, tree->_line, tree->_column);
                    newTree->_left = tree;
                    tree = newTree;
                    return tree;
                }
                while(tempTree->_right != nullptr){
                    if(tempTree->_value == "*" || tempTree->_value == "/"){
                        break;
                    }
                    tempTree = tempTree->_left;
                }

                AuxillaryTree* newTree = new AuxillaryTree(token, value, tree->_line, tree->_column);
                newTree->_left = tempTree;
                tempTreeParent->_right= newTree;
                return tempTreeParent;
            }else{
                // Otherwise, we need make the new created tree the parent of the latest small tree
                AuxillaryTree* newTree = new AuxillaryTree(token, value, tree->_line, tree->_column);
                newTree->_left = tree;
                tree = newTree;
                return tree;
            }

        }
        return tree;
    }

    void processEndOfStatement(){

        // Check if the parenthesis count is 0
        if(_parenthesisCount != 0){
            _errorHandler->addError("Parenthesis count is not 0");
            //throw std::runtime_error("Parenthesis count is not 0");
        }

        // While there's still something to merge from the small trees
        while(!_smallTrees.empty()){

            // Create a temporary storage from the latest small tree
            AuxillaryTree* tempTree = _smallTrees.back();

            // If the latest smallest tress is null.
            // More common that you think base on the current implementation
            if(_latestSmallTree == nullptr){
                // Set the latest small tree to the temp tree
                _latestSmallTree = tempTree;
                // Pop the small tree
                _smallTrees.pop_back();
                // And Continue
                continue;
            }
            
            if(this->isMultiplicationOrDivision(tempTree->_token)){
            //if(tempTree->_token == LanguageToken::MultiplicationToken){
                // This tree will be  the child of the latest tree

                // If the parent node is again a multiplication, then it's safe to assume that the latest small tree should be a root node
                if(this->isMultiplicationOrDivision(_latestSmallTree->_token)){
                    AuxillaryTree* rhsRoot = _latestSmallTree->_right;
                    _latestSmallTree->_right = tempTree;
                    // Then it might have parenthesis
                    if(_latestSmallTree->_left == nullptr){
                        _latestSmallTree->_left = tempTree;
                    }else if(_latestSmallTree->_right == nullptr){
                        _latestSmallTree->_right = tempTree;
                    }

                    // Normal Processing
                    else if(tempTree->_left == nullptr){
                        tempTree->_left = rhsRoot;
                    }else if(tempTree->_right == nullptr){
                        tempTree->_right = rhsRoot;
                    }else{
                        if(rhsRoot != nullptr){
                            throw std::runtime_error("This Case in not yet Handled by the Language");
                        }
                    }

 
                }else{
                    AuxillaryTree* lhsRoot = _latestSmallTree->_left;
                    _latestSmallTree->_left = tempTree;
                    // Then it might have parenthesis
                    if(_latestSmallTree->_left == nullptr){
                        _latestSmallTree->_left = tempTree;
                    }else if(_latestSmallTree->_right == nullptr){
                        _latestSmallTree->_right = tempTree;
                    }
                    
                    // Normal Processing
                    else if(tempTree->_left == nullptr){
                        tempTree->_left = lhsRoot;
                    }else if(tempTree->_right == nullptr){
                        tempTree->_right = lhsRoot;
                    }else{
                        if(lhsRoot != nullptr){
                            throw std::runtime_error("This Case in not yet Handled by the Language");
                        }
                    }
                }
               _smallTrees.pop_back();
                continue;
            }

            // Check if lhs or rhs is null to determine where to put the temp tree
            if(tempTree->_right == nullptr){
                tempTree->_right= _latestSmallTree;
            }else if(tempTree->_left == nullptr){
                tempTree->_left = _latestSmallTree;
            }
            else{

                // If lhs is not null, then we need to do some processing to maintain the order of operations
                // This bool is used to check if the temp tree has been changed.
                // If it's not, then the algorithm found a valid rhs for the temp tree
                bool newTempTreeChanged = false;

                // Create a new temp tree that will be used to traverse the tree
                AuxillaryTree *newTempTree = tempTree;

                // Traverse the tree until the rhs is null
                while(newTempTree->_right != nullptr){
                    newTempTree = newTempTree->_right;
                }
                
                // If rhs after traversal is a literal or identifier, then we need to do additional processing.
                // We need to check for other valid tree from the latest small tree and use it as the rhs
                // This still maintain the order of operation as this case will only happen if
                // there are parenthesis, where the first statement to be processed is on the far right
                // This type of situation make it so that that most of the small trees to be complete, which is why
                // we need to traverse the main tree to find a valid tree.
                // This can be improved by better implementation of order of operation in the future
                if(this->isLiteralOrIdentifier(newTempTree)){

                    // Find a valid tree from the latest small tree
                    if((newTempTree = findValidTree(_latestSmallTree)) == nullptr){
                        _errorHandler->addError("Invalid tree found");
                    }

                    // Set the new temp tree changed to true
                    newTempTreeChanged = true;
                }

                // If it's changed, then we need to set the rhs of the new temp tree to the temp tree
                if(newTempTreeChanged){
                    if(newTempTree->_right == nullptr){
                        newTempTree->_right = tempTree;
                    }
                    else{
                        newTempTree->_left = tempTree;
                    }
                    tempTree = _latestSmallTree;
                }

                // Otherwise we just need to set the rhs of the temp tree to the latest small tree
                else{
                    if(tempTree->_right == nullptr){
                        tempTree->_right = _latestSmallTree;
                    }else{
                        tempTree->_left = _latestSmallTree;
                    }

                }
           }

            // Update the latest small tree and pop the small tree
            _latestSmallTree = tempTree;
            _smallTrees.pop_back();
        }

        // Update the currenet root and push the latest small tree to the totality tree
        _root = _latestSmallTree;
        _totalityTree.push_back(_latestSmallTree);
        _latestSmallTree = nullptr;
        return;
    }

    void processCloseParenthesis(LanguageToken &token, std::string &value){

        // If the parenthesis count is less than 0, then there is an error
        if(--_parenthesisCount < 0){
            _errorHandler->addError("Parenthesis count is less than 0");
        }
        
        // If the small tree is empty, then there's no point in continuing
        if(_smallTrees.empty()){
            return;
        }
        
        // Get the latest smallest tree from the small trees
        AuxillaryTree* lastSmallTree = _smallTrees.back();

        // Check if it's a conditional operator or a double operator
        bool isConditional = this->isConditionalOperator(lastSmallTree->_value);
        bool isDoubleOperator = this->isDoubleOperator(lastSmallTree->_value);

        // If it's a double operator, then push the small tree to the totality tree to maintain precedence
        if(isDoubleOperator && !isConditional){
            _smallTrees.push_back(_latestSmallTree);
            _latestSmallTree = nullptr;
            return;
        }

        // if there are still parenthesis left, there's a possibility that a statement has a higher precedence that the current small tree. So return, and let the next statement handle it
        if(_parenthesisCount > 0){
            return;
        }

        // Otherwise, pop the latest small tree from the small tree and process it
        _smallTrees.pop_back();

        // If both lhs and rhs are not null
        if(lastSmallTree->_left != nullptr && lastSmallTree->_right != nullptr){
            // Then there's probably a nullptr value in the _latestSmallTree
            // Find it, and replace it with the last small tree
            
            // This happens because of the way the tree is built.
            // Sometimes, left and right are populated if it's enclosed by parenthesis such as the mathematical expression: (a + b) + c
            // (a + b) will produce a complete small tree, which wouldn't have any nullptr values
            // In this case, we are sure that the _latestSmallTree has a nullptr value
            if(_latestSmallTree->_left == nullptr){
                _latestSmallTree->_left = lastSmallTree;
            }else if(_latestSmallTree ->_right ==nullptr){
                _latestSmallTree->_right = lastSmallTree;
            }

            // Push the updated latest small tree in the small trees
            _smallTrees.push_back(_latestSmallTree);

            // Set the latest small tree to null to accept new statements
            _latestSmallTree = nullptr;
            
            return;
        }

        // If there's only one child that is not null
        else{
            _smallTrees.push_back(lastSmallTree);
            lastSmallTree = _latestSmallTree;
            // Find what it's and set it to the latest small tree
            //if(lastSmallTree->_right ==nullptr){
                //lastSmallTree->_right = _latestSmallTree;
            //}
            //else{
                //lastSmallTree->_left = _latestSmallTree;
            //}
        }

        // If it's a conditional statement, Additional processing is required
        if(_isConditional){
            bool foundAConditional = false;
            int defaultSize = _smallTrees.size();
            for(int i = 0; i < defaultSize; ++i){
                auto treeValue = _smallTrees.back();
                if(this->isConditional(treeValue)){
                    foundAConditional = true;
                    treeValue->_right = _latestSmallTree;
                    _latestSmallTree = treeValue;
                    _smallTrees.pop_back();
                    continue;
                }
                // Expects that the parenthesis was already merge at this point so othere's only one tree left before the if-token
                if(foundAConditional){
                    _latestSmallTree->_left = treeValue;
                    _smallTrees.pop_back();
                    break;
                }
                AuxillaryTree* backTree = _smallTrees.back();
                if(backTree->_left == nullptr){
                    backTree->_left = _latestSmallTree;
                }else if(backTree->_right == nullptr){
                    backTree->_right = _latestSmallTree;
                }else{
                    std::cout << "UNDEFINED FOR NOW";
                }
                _latestSmallTree = backTree;
                _smallTrees.pop_back();
            }
            // Mark the small tree as not conditional. This is because we've already processed it
            _isConditional = false;
            
            // Now to process the if-token, we need to pop the small tree again
            AuxillaryTree* newLastSmallTree = _smallTrees.back();

            // Check what is available, and append the if-token to it.
            if(newLastSmallTree->_left == nullptr){
                newLastSmallTree->_left = _latestSmallTree;
            }else{

                // There's a possibility that the lhs is not null, but the rhs is null on the outermost right, that's why we need to traverse the tree to find the outermost right
                AuxillaryTree *tempTree = newLastSmallTree;
                while(tempTree->_right != nullptr){
                    tempTree = tempTree->_right;
                }
                tempTree->_right = _latestSmallTree;
            }

            // Update then pop the small tree
            lastSmallTree = newLastSmallTree;
            _smallTrees.pop_back();

        }

        // Update the latest small tree base on the processes tree and push it back to the small tree to maintain precedence
        _latestSmallTree = lastSmallTree;
        _smallTrees.push_back(_latestSmallTree);
        _latestSmallTree = nullptr;
        return;

    }


    void processParenthesisOrOperator(LanguageToken &token, std::string &value, bool &isConditional){
        // If it's not a conditional statement, it's a parenthesis
        if(!isConditional){
            ++_parenthesisCount;
        }
        
        // If the latest small tree is null and it's a conditional conditional, create a new small tree
        if(_latestSmallTree == nullptr && isConditional){
            // This is done so that the conditional statement wouldn't merge without the next part of the condition is read
            _smallTrees.push_back(new AuxillaryTree(token,value, _line, _column));
            return;
        }
            
        // If the latest small tree is null and it's not a conditional statement, return
        else if(_latestSmallTree == nullptr){
            // This is done because it wouldn't really do anything in the following code. Done for optimization
            return; 
        }
        
        // Create a new small tree from the latest small tree
        // This is done so that the current latest small tree wouldn't be affected by the next read. Thereby maitaining its precedence
        _smallTrees.push_back(_latestSmallTree);

        // Reset the latest small tree to store the next small tree
        _latestSmallTree = nullptr;

        // If it's a conditional operator, create a new small tree
        if(!_isConditional){
            _isConditional = isConditional;
            if(_isConditional){
                // Create a new small tree base on the current token and value.
                // This is done to ensure the presedence of the conditional statement whilst making sure that the next part of the condition is with correct precedence
                _latestSmallTree = new AuxillaryTree(token, value, _line, _column);
                _smallTrees.push_back(_latestSmallTree);
                _latestSmallTree = nullptr;
            }
        }
        
        return;
    }

// Others
private:

    // Summarize and get all the  value of the tree
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
    
    // Auxillaray function to summarize the value of the tree
    void summarizeTreeValue(AuxillaryTree* &tree){
        if(tree == nullptr){
            return;
        }
        summarizeTreeValue(tree->_left);
        summarizeTreeValue(tree->_right);
    }
 
    // Auxillary function to evaluate the tree
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
    
    // Auxillary Function to process evaluation
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
            case LanguageToken::InputToken:
                if(this->expect(tree, &AST::isNull)){
                    isCorrect = true;
                }
                break;
            case LanguageToken::AdditionToken:
            case LanguageToken::SubtractionToken:
            case LanguageToken::MultiplicationToken:
            case LanguageToken::DivisionToken:
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
            case LanguageToken::RightShiftToken:
                //this->expect(tree, &AST::isInputable);
                if(this->expect(tree, &AST::isKeyword ,&AST::isInputable)){
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
            case LanguageToken::TypeStringToken:
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

    // Ease up the function call for evaluation
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
    
    // Cases where the function needs to evaluate lhs and rhs seperately
    bool expect(AuxillaryTree* tree, bool (AST::*lhs_func)(AuxillaryTree*), bool (AST::*rhs_func)(AuxillaryTree*)){
        if((this->*lhs_func)(tree->_left) && (this->*rhs_func)(tree->_right)){
            return true;
        }
        return false;
    }

// Auxillary Evaluators
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
        bool seventhRule = this->isMultiplicationOrDivision(tree->_token);
        //bool seventhRule = tree->_token == LanguageToken::MultiplicationToken;
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

    bool isInputable(AuxillaryTree *tree){
        if(tree == nullptr){
            return false;
        }
        bool firstRule = tree->_token == LanguageToken::IdentifierToken;
        if(firstRule){
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
        bool thirdRule = tree->_token == LanguageToken::InputToken;
        if(firstRule || secondRule || thirdRule){
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

    bool isMultiplicationOrDivision(LanguageToken token){
        return token == LanguageToken::MultiplicationToken || token == LanguageToken::DivisionToken;
    }
};

#endif // ABSTRACTSYNTAXTREE_h


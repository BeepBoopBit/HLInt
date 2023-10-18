#ifndef ABSTRACTSYNTAXTREE_H
#define ABSTRACTSYNTAXTREE_H

#include "../LanguageDictionary/LanguageDictionary.h"
#include <vector>
#include <string>
#include <iostream>

class AuxillaryTree{
private:
    using LanguageToken = LanguageDictionary::LanguageToken;

public:
    LanguageToken _token = LanguageToken::InvalidToken;
    AuxillaryTree* _left = nullptr;
    AuxillaryTree* _right = nullptr;
    std::string _value = "";

public:
    AuxillaryTree(LanguageToken token, std::string value): _token(token), _value(value){
    }
    ~AuxillaryTree(){}
};

class AST{

private:
    using LanguageToken = LanguageDictionary::LanguageToken;

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
private:
    // Main Tree
    AuxillaryTree *_root = new AuxillaryTree(LanguageToken::RootNode, "RootNode");

    // Tree Totality
    std::vector<AuxillaryTree*> _totalityTree;

    // Small Trees
    std::vector<AuxillaryTree*> _smallTrees;
    int _currentSmallTreeIndex = 0;

    // latest Small Tree Index
    AuxillaryTree* _latestSmallTree = nullptr;

    bool _isConditional = false;

public:
    // Value can be empty
    void insert(LanguageToken token, std::string value = ""){
        bool isConditional = false;
        if((token == LanguageToken::OpenParenthesisToken) || (isConditional = isConditionalOperator(value))){

            if(_latestSmallTree == nullptr){
                return; 
            }
            
            // Push the latest small tree to it
            _smallTrees.push_back(_latestSmallTree);

            // Set the latest small tree to null
            _latestSmallTree = nullptr;

            if((_isConditional = isConditional)){
                _latestSmallTree = new AuxillaryTree(token, value);
                _smallTrees.push_back(_latestSmallTree);
                _latestSmallTree = nullptr;
            }

            return;

        }else if(token == LanguageToken::CloseParenthesisToken){

            if(_smallTrees.empty()){
                return;
            }

            AuxillaryTree* lastSmallTree = _smallTrees.back();
            bool isConditional = this->isConditionalOperator(lastSmallTree->_value);
            bool isDoubleOperator = this->isDoubleOperator(lastSmallTree->_value);
            if(isDoubleOperator && !isConditional){
                return;
            }
            _smallTrees.pop_back();
            if(lastSmallTree->_left != nullptr && lastSmallTree->_right != nullptr){
                if(_latestSmallTree->_left == nullptr){
                    _latestSmallTree->_left = lastSmallTree;
                }else if(_latestSmallTree ->_right ==nullptr){
                    _latestSmallTree->_right = lastSmallTree;
                }
                _latestSmallTree = lastSmallTree;
                _smallTrees.push_back(_latestSmallTree);
                _latestSmallTree = nullptr;
                return;
            }
            else{
                lastSmallTree->_right = _latestSmallTree;
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
            while(!_smallTrees.empty()){
                AuxillaryTree* tempTree = _smallTrees.back();
                if(tempTree->_left == nullptr){
                    tempTree->_left = _latestSmallTree;
                }else{
                    AuxillaryTree *newTempTree = tempTree;
                    while(newTempTree->_right != nullptr){
                        newTempTree = newTempTree->_right;
                    }
                    newTempTree->_right = _latestSmallTree;
                }
                _latestSmallTree = tempTree;
                _smallTrees.pop_back();
            }
            _root = _latestSmallTree;
            _totalityTree.push_back(_latestSmallTree);
            _latestSmallTree = nullptr;
            return;
        }

        if(_latestSmallTree == nullptr){
            _latestSmallTree = new AuxillaryTree(token, value);
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
                _latestSmallTree = new AuxillaryTree(token, value);
            }
            else if(_latestSmallTree->_left == nullptr){
                _latestSmallTree->_left = new AuxillaryTree(token, value);
            }else{
                AuxillaryTree *tempTree = _latestSmallTree;
                while(tempTree->_right != nullptr){
                    tempTree = tempTree->_right;
                }
                tempTree->_right = new AuxillaryTree(token, value);
            }
        }
    }

    void print(){
        for(int i = 0; i < _totalityTree.size(); ++i){
            std::cout << "\n#################### Totality [" << i << "] ####################\n";
            printBT(_totalityTree[i]);
            std::cout << "#################### Totality [" << i << "] ####################\n";
        }
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
            tree = new  AuxillaryTree(token, value);
            tree->_left = newTree;
            // Create a new tree on the latest small tree base on the latest small tree
            //tree->_left = new AuxillaryTree(tree->_token, tree->_value);
            
            // Update the latest small tree token and value to reflect the new value
            //tree->_token = token;
            //tree->_value = value;
        }else{
            if(token == LanguageToken::MultiplicationToken){
                AuxillaryTree* tempTree = tree->_right;
                AuxillaryTree* newTree = new AuxillaryTree(token, value);
                newTree->_left = tempTree;
                tree->_right = newTree;

            }else{
                AuxillaryTree* newTree = new AuxillaryTree(token, value);
                newTree->_left = tree;
                tree = newTree;
                return tree;
            }

        }
        return tree;
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


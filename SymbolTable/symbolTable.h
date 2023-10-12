#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "objectType.h"

class SymbolTable{

// variables
private:
    std::map<std::string, ObjectType*> variableTable;
    std::map<std::string, std::string> keywords = {
        {"int", "int"},
        {"float", "float"},
        {"string", "string"},
        {"bool", "bool"},
        {"true", "bool"},
        {"false", "bool"},
        {"if", "if"},
        {"else", "else"},
        {"while", "while"},
        {"for", "for"},
        {"return", "return"},
        {"print", "print"},
        {"println", "println"},
        {"read", "read"},
        {"readln", "readln"},
        {"and", "and"},
        {"or", "or"},
        {"not", "not"}
    };
    std::map<char, char> operators = {
        {'+', '+'},
        {'-', '-'},
        {'*', '*'},
        {'/', '/'},
        {'%', '%'},
        {'^', '^'},
        {'>', '>'},
        {'<', '<'},
        {'=', '='},
        {'!', '!'},
        {':', ':'} // Assignment
    };
    std::map<char, char> alphabeth ={
        {'a','a'},
        {'b','b'},
        {'c','c'},
        {'d','d'},
        {'e','e'},
        {'f','f'},
        {'g','g'},
        {'h','h'},
        {'i','i'},
        {'j','j'},
        {'k','k'},
        {'l','l'},
        {'m','m'},
        {'n','n'},
        {'o','o'},
        {'p','p'},
        {'q','q'},
        {'r','r'},
        {'s','s'},
        {'t','t'},
        {'u','u'},
        {'v','v'},
        {'w','w'},
        {'x','x'},
        {'y','y'},
        {'z','z'},
        {'A','A'},
        {'B','B'},
        {'C','C'},
        {'D','D'},
        {'E','E'},
        {'F','F'},
        {'G','G'},
        {'H','H'},
        {'I','I'},
        {'J','J'},
        {'K','K'},
        {'L','L'},
        {'M','M'},
        {'N','N'},
        {'O','O'},
        {'P','P'},
        {'Q','Q'},
        {'R','R'},
        {'S','S'},
        {'T','T'},
        {'U','U'},
        {'V','V'},
        {'W','W'},
        {'X','X'},
        {'Y','Y'},
        {'Z','Z'},
        {'_','_'},
        {'0','0'},
        {'1','1'},
        {'2','2'},
        {'3','3'},
        {'4','4'},
        {'5','5'},
        {'6','6'},
        {'7','7'},
        {'8','8'},
        {'9','9'}
    };
public:
    SymbolTable(){
        // Initialize the symbol table
        this->variableTable = std::map<std::string, ObjectType*>();
    }

// Methods
public:
    void addVariable(std::string name, ObjectType* variable){
        if(!this->isVariable(name)){
            this->variableTable[name] = variable;
            return;
        }
        // ERROR: Variable already exists
        std::cout << "ERROR: Variable already exists\n";
    }
    ObjectType* getVariable(std::string name){
        return this->variableTable[name];
    }
    std::vector<std::string> getVariableNames(){
        std::vector<std::string> names;
        for(std::map<std::string, ObjectType*>::iterator it = this->variableTable.begin(); it != this->variableTable.end(); it++){
            names.push_back(it->first);
        }
        return names;
    }
    std::vector<ObjectType*> getVariables(){
        std::vector<ObjectType*> variables;
        for(std::map<std::string, ObjectType*>::iterator it = this->variableTable.begin(); it != this->variableTable.end(); it++){
            variables.push_back(it->second);
        }
        return variables;
    }
    void printVariableTable(){
        std::cout << "Variable Table" << std::endl;
        for(std::map<std::string, ObjectType*>::iterator it = this->variableTable.begin(); it != this->variableTable.end(); it++){
            if (it->second->getType() == "integer"){
                ObjectTypeInt* variable = (ObjectTypeInt*)it->second;
                std::cout << it->first << " = " << variable->getValue() << std::endl;
            }else if(it->second->getType() == "double"){
                ObjectTypeDouble* variable = (ObjectTypeDouble*)it->second;
                std::cout << it->first << " = " << variable->getValue() << std::endl;
            }
        }
    }

// Checkers
public:
    bool isKeyword(std::string token){
        return this->keywords.find(token) != this->keywords.end();
    }
    bool isOperator(char token){
        return this->operators.find(token) != this->operators.end();
    }
    bool isVariable(std::string token){
        return this->variableTable.find(token) != this->variableTable.end();
    }
    bool isValidVariableCharacter(char token){
        return this->alphabeth.find(token) != this->alphabeth.end();
    }
};
#endif // SYMBOLTABLE_H

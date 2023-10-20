#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <fstream>

#include "objectType.h"


/**
 * Symbol Table (Singleton)
 * - Store the list of variables used in the program.
**/
class SymbolTable{

// variables
private:
    // Store the list of variables
    std::map<std::string, ObjectType*> variableTable;

    std::string _filename = "RES_SYM.txt";
    std::ofstream _file;

private:
    SymbolTable(){
        // Initialize the symbol table
        this->variableTable = std::map<std::string, ObjectType*>();
    }

    ~SymbolTable(){}

public:

    // Delete the copy constructor and assignment operator
    SymbolTable(SymbolTable const&) = delete;
    void operator=(SymbolTable const&) = delete;

public:
    static SymbolTable& getInstance(){
        static SymbolTable instance;
        return instance;
    }


// non-destructive methods
public:
    void declare(std::string name, ObjectType* variable){
        if(!this->isVariable(name)){
            this->variableTable[name] = variable;
            return;
        }
        // ERROR: Variable already exists
        throw std::runtime_error("ERROR: Variable already exists\n");
    }

    ObjectType* get(std::string name){
        auto value = this->variableTable.find(name) != this->variableTable.end() ? this->variableTable[name] : nullptr;
        if(value == nullptr){
            throw std::runtime_error("Variable is not Declared");
        }
        return value;
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


// Destructive Methods
public:
    
    
    // Assign the value of the variable
    void set(std::string name, ObjectType* variable){
        
        // Happens when you assign a variable that doesn't eixsts
        if(!this->isVariable(name)){
            // ERROR: Variable does not exist
            throw std::runtime_error("ERROR: Variable does not exist\n");
        }
        this->variableTable[name] = variable;
    }

    // Remove the variable from the symbol table
    void remove(std::string name){
        if(this->isVariable(name)){
            this->variableTable.erase(name);
            return;
        }
        // ERROR: Variable does not exist
        throw std::runtime_error("ERROR: Variable does not exist\n");
    }

   
// Parsing Auxillary Methods
public:
    ObjectTypeInt* parseToInt(ObjectType* variable){
        if(variable->getType() == "integer"){
            return (ObjectTypeInt*)variable;
        }
        // ERROR: Cannot convert to int
        std::cout << "ERROR: Cannot convert to int\n";
        return nullptr;
    }

    ObjectTypeDouble* parseToDouble(ObjectType *variable){
        if(variable->getType() == "double"){
            return (ObjectTypeDouble*)variable;
        }
        // ERROR: Cannot convert to double
        std::cout << "ERROR: Cannot convert to double\n";
        return nullptr;
    }

    ObjectTypeString* parseToString(ObjectType *variable){
        if(variable->getType() == "string"){
            return (ObjectTypeString*)variable;
        }
        // ERROR: Cannot convert to string
        std::cout << "ERROR: Cannot convert to string\n";
        return nullptr;
    }

private:
    void writeToFile(){
        this->_file.open(this->_filename);
        std::string tempData = "";
        for(std::map<std::string, ObjectType*>::iterator it = this->variableTable.begin(); it != this->variableTable.end(); it++){
            if (it->second->getType() == "integer"){
                ObjectTypeInt* variable = (ObjectTypeInt*)it->second;
                tempData += variable->getType() + ": " + it->first + " = " + std::to_string(variable->getValue()) +  '\n';
            }else if(it->second->getType() == "double"){
                ObjectTypeDouble* variable = (ObjectTypeDouble*)it->second;
                tempData += variable->getType() + ": "+ it->first + " = " + std::to_string(variable->getValue()) + '\n';
            }
        }
        this->_file << tempData;
        this->_file.close();
    }

// Others
public:
void saveToFile(){
    // Write the symbol table to a file
    writeToFile();
#ifdef DEBUG
    std::cout << "[/] Successfuly Closed the file [" << this->_filename << "]" << std::endl;
#endif
}

// Debug
public:
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
    bool isVariable(std::string token){
        return this->variableTable.find(token) != this->variableTable.end();
    }
};
#endif // SYMBOLTABLE_H


/**
 *
 * Notes:
 * 1. We don't need to support scope base on the instruction. Finish the instruction first before adding other shits.
 *
**/

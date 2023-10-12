#ifndef OBJECTTYPE_H
#define OBJECTTYPE_H

#include <string>

class ObjectType{

protected:
    std::string name;
    std::string type;

public:

// Methods
public:
    std::string getName(){
        return this->name;
    }
    std::string getType(){
        return this->type;
    }
};

class ObjectTypeInt : public ObjectType{

private:
    int value;

// Constructor
public:
    ObjectTypeInt(std::string name){
        this->name = name;
        this->type = "integer";
    }

// Methods
public:
    int getValue(){
        return this->value;
    }

// Operators
public:

    // Assignment
    ObjectTypeInt operator=(int value){
        this->value = value;
        return *this;
    }
    ObjectTypeInt operator=(ObjectTypeInt value){
        this->value = value.getValue();
        return *this;
    }
    
    // Addition
    int operator+(int value){
        return this->value + value;
    }
    int operator+(ObjectTypeInt value){
        return this->value + value.getValue();
    }
    
    // Subtraction
    int operator-(int value){
        return this->value - value;
    }
    int operator-(ObjectTypeInt value){
        return this->value - value.getValue();
    }

    // Multiplication
    int operator*(int value){
        return this->value * value;
    }
    int operator*(ObjectTypeInt value){
        return this->value * value.getValue();
    }

    // Division
    int operator/(int value){
        return this->value / value;
    }
    int operator/(ObjectTypeInt value){
        return this->value / value.getValue();
    }
};

class ObjectTypeDouble : public ObjectType{


private:
    double _value;
public:
    ObjectTypeDouble(std::string name){
        this->name = name;
        this->type = "double";
    }
public:
    double getValue(){
        return this->_value;
    }

public:
    // Assignment
    ObjectTypeDouble operator=(double value){
        this->_value = value;
        return *this;
    }
    ObjectTypeDouble operator=(ObjectTypeDouble value){
        this->_value = value.getValue();
        return *this;
    }
    
    // Addition
    double operator+(double value){
        return this->_value + value;
    }
    double operator+(ObjectTypeInt value){
        return this->_value + value.getValue();
    }
    
    // Subtraction
    double operator-(double value){
        return this->_value - value;
    }
    double operator-(ObjectTypeInt value){
        return this->_value - value.getValue();
    }

    // Multiplication
    double operator*(double value){
        return this->_value * value;
    }
    double operator*(ObjectTypeInt value){
        return this->_value * value.getValue();
    }

    // Division
    double operator/(double value){
        return this->_value / value;
    }
    double operator/(ObjectTypeInt value){
        return this->_value / value.getValue();
    }
};

#endif // OBJECTTYPE_H

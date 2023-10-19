#ifndef OBJECTTYPE_H
#define OBJECTTYPE_H

#include <string>
#include <iostream>

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


// ####################################################################

class ObjectTypeInt : public ObjectType{

private:
    int value;

// Constructor
public:
    ObjectTypeInt(std::string name){
        this->name = name;
        this->type = "integer";
    }
    ObjectTypeInt(std::string name, int value){
        this->name = name;
        this->value = value;
        this->type = "integer";
    }
    ObjectTypeInt(int value){
        this->value = value;
        this->type = "integer";
    }
    ObjectTypeInt(ObjectTypeInt &value){
        this->value = value.getValue();
        this->type = "integer";
    }
    ObjectTypeInt(double value){
        this->value = (int)value;
        this->type = "integer";
    }
// Implicit Conversion
public:
    operator int(){
        return this->value;
    }
    operator double(){
        return (double)this->value;
    }

// Methods
public:
    int getValue(){
        return this->value;
    }
    void setValue(int value){
        this->value = value;
    }
    void setValue(ObjectTypeInt value){
        this->value = value.getValue();
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
    ObjectTypeInt operator+(int value){
        this->value = value + this->value;
        return *this;
    }
    ObjectTypeInt operator+(ObjectTypeInt value){
        this->value = value.getValue() + this->value;
        return *this;
    }
    
    // Subtraction
    ObjectTypeInt operator-(int value){
        this->value = this->value - value;
        return *this;
    }
    ObjectTypeInt operator-(ObjectTypeInt value){
        this->value = this->value - value.getValue();
        return *this;
    }

    // Multiplication
    ObjectTypeInt operator*(int value){
        this->value = value * this->value;
        return *this;
    }
    ObjectTypeInt operator*(ObjectTypeInt value){
        this->value = value.getValue() * this->value;
        return *this;
    }

    // Division
    ObjectTypeInt operator/(int value){
        if(value == 0){
            // ERROR: Division by zero
            throw std::runtime_error("Division by zero");
        }
        this->value = this->value / value;
        return *this;
    }
    ObjectTypeInt operator/(ObjectTypeInt value){
        if(value.getValue() == 0){
            // ERROR: Division by zero
            throw std::runtime_error("Division by zero");
        }
        this->value = this->value / value.getValue();
        return *this;
    }
// Comparisons
public:
    bool operator==(int value){
        return this->value == value;
    }
    bool operator==(ObjectTypeInt value){
        return this->value == value.getValue();
    }
    bool operator!=(int value){
        return this->value != value;
    }
    bool operator!=(ObjectTypeInt value){
        return this->value != value.getValue();
    }
    bool operator>(int value){
        return this->value > value;
    }
    bool operator>(ObjectTypeInt value){
        return this->value > value.getValue();
    }
    bool operator<(int value){
        return this->value < value;
    }
    bool operator<(ObjectTypeInt value){
        return this->value < value.getValue();
    }
    bool operator>=(int value){
        return this->value >= value;
    }
    bool operator>=(ObjectTypeInt value){
        return this->value >= value.getValue();
    }
    bool operator<=(int value){
        return this->value <= value;
    }
    bool operator<=(ObjectTypeInt value){
        return this->value <= value.getValue();
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
    ObjectTypeDouble(std::string name, double value){
        this->name = name;
        this->_value = value;
        this->type = "double";
    }
    ObjectTypeDouble(double value){
        this->_value = value;
        this->type = "double";
    }
    ObjectTypeDouble(ObjectTypeDouble &value){
        this->_value = value.getValue();
        this->type = "double";
    }

// Implicit Conversion
public:
    operator int(){
        return (int)this->_value;
    }
    operator double(){
        return this->_value;
    }

public:
    double getValue(){
        return this->_value;
    }
    void setValue(double value){
        this->_value = value;
    }
    void setValue(ObjectTypeDouble value){
        this->_value = value.getValue();
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
    ObjectTypeDouble* operator+(double value){
        this->_value = value + this->_value;
        return this;
    }
    ObjectTypeDouble* operator+(ObjectTypeDouble value){
        this->_value = value.getValue() + this->_value;
        return this;
    }
    
    // Subtraction
    ObjectTypeDouble* operator-(double value){
        this->_value = this->_value - value;
        return this;
    }
    ObjectTypeDouble* operator-(ObjectTypeDouble value){
        this->_value = this->_value - value.getValue();
        return this;
    }

    // Multiplication
    ObjectTypeDouble* operator*(double value){
        this->_value = value * this->_value;
        return this;
    }
    ObjectTypeDouble* operator*(ObjectTypeDouble value){
        this->_value = value.getValue() * this->_value;
        return this;
    }

    // Division
    ObjectTypeDouble* operator/(double value){
        if(value == 0){
            // ERROR: Division by zero
            throw std::runtime_error("Division by zero");
        }
        this->_value = this->_value / value;
        return this;
    }
    ObjectTypeDouble* operator/(ObjectTypeDouble value){
        this->_value = this->_value / value.getValue();
        return this;
    }
// Comparisons
public:
    bool operator==(double value){
        return this->_value == value;
    }
    bool operator==(ObjectTypeDouble value){
        return this->_value == value.getValue();
    }
    bool operator!=(double value){
        return this->_value != value;
    }
    bool operator!=(ObjectTypeDouble value){
        return this->_value != value.getValue();
    }
    bool operator>(double value){
        return this->_value > value;
    }
    bool operator>(ObjectTypeDouble value){
        return this->_value > value.getValue();
    }
    bool operator<(double value){
        return this->_value < value;
    }
    bool operator<(ObjectTypeDouble value){
        return this->_value < value.getValue();
    }
    bool operator>=(double value){
        return this->_value >= value;
    }
    bool operator>=(ObjectTypeDouble value){
        return this->_value >= value.getValue();
    }
    bool operator<=(double value){
        return this->_value <= value;
    }
    bool operator<=(ObjectTypeDouble value){
        return this->_value <= value.getValue();
    }
};

class ObjectTypeString : public ObjectType{

private:
    std::string value;

public:
    ObjectTypeString(std::string name){
        this->name = name;
        this->type = "string";
    }
    ObjectTypeString(std::string name, std::string value){
        this->name = name;
        this->value = value;
        this->type = "string";
    }
    ObjectTypeString(ObjectTypeString &value){
        this->value = value.getValue();
        this->type = "string";
    }

// Implicit Conversion
public:
    operator std::string(){
        return this->value;
    }

// Methods
public:
    std::string getValue(){
        return this->value;
    }
    void setValue(std::string value){
        this->value = value;
    }
    void setValue(ObjectTypeString value){
        this->value = value.getValue();
    }

// Operators
public:
    // Assignment
    ObjectTypeString operator=(std::string value){
        this->value = value;
        return *this;
    }
    ObjectTypeString operator=(ObjectTypeString value){
        this->value = value.getValue();
        return *this;
    }
    
    // Addition
    ObjectTypeString operator+(std::string value){
        this->value = value + this->value;
        return *this;
    }
    ObjectTypeString operator+(ObjectTypeString value){
        this->value = value.getValue() + this->value;
        return *this;
    }

// Comparison
public:
    bool operator==(std::string value){
        return this->value == value;
    }
    bool operator==(ObjectTypeString value){
        return this->value == value.getValue();
    }
    bool operator!=(std::string value){
        return this->value != value;
    }
    bool operator!=(ObjectTypeString value){
        return this->value != value.getValue();
    }
    bool operator>(std::string value){
        return this->value > value;
    }
    bool operator>(ObjectTypeString value){
        return this->value > value.getValue();
    }
    bool operator<(std::string value){
        return this->value < value;
    }
    bool operator<(ObjectTypeString value){
        return this->value < value.getValue();
    }
    bool operator>=(std::string value){
        return this->value >= value;
    }
    bool operator>=(ObjectTypeString value){
        return this->value >= value.getValue();
    }
    bool operator<=(std::string value){
        return this->value <= value;
    }
    bool operator<=(ObjectTypeString value){
        return this->value <= value.getValue();
    }
    
};

#endif // OBJECTTYPE_H

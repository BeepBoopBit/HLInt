# Main Code Documentation


## Classes

This section list and define the classes of the program including their methods and functions.

### HLint

Interface for the whole interpreter.

#### Constructors

* `HLint(std::string filenam)`

#### Methods

* start()
    * Start the Interpreter.


### SymbolTable

Store and maintain information amongst the classes. This include creating, deleting, modifying, and reading information created by the program.

#### Constructor

* SymbolTable()
    * Private Constructor to deploy a singleton class
* SymbolTable(SymbolTable const&)
    * Private Copy Constructor to avoid multiple copy of the class

#### Methods

* void operator=(SymbolTable const&) = delete
    * Deleted assignment operator since the SymbolTable is a sigleton

### LexicalAnalyzer



#### Constructor



#### Methods



#### Checkers



### ErrorHandler

This will handle the logging of errors within the program, put it in a file and the console.

### LanguageDictionary



### SyntaxAnalyzer



## Variables

* std::map<std::string, ObjectType*> variableTable;
    * Contains all information about variables that is being used in the program.

#### Auxillary Class: ObjectType

This handles the mathematical operators within the program. This includes the declaration of data types, obtaining values, and different operators.

##### Constructor

* ObjectTypeInt(std::string name)
    * This sets the type of the object to a certain data type such as "integer."
    * The same thing is applied with the double data type under the ObjectTypeDouble class.

##### Methods

* std::string getName()
    * This gets access from the name attribute and returns its value.

* std::string getType()
    * This gets access from the type attribute and returns its value.

* int getValue()
    * This gets access from the private attribute value and return its value.

##### Operators

* It defines the operators used and how they behave within the program. This includes the assignment, addition, subtraction, multiplication, and division.
    * The process has two classes: ObjectTypeInt (deals with integer values) and ObjectTypeDouble (deals with double values).
    * Under the following arithmetic operators such as +, -, *, /, the proccess done in (int values) is applied to (double values)  

###### Assignment

* ObjectTypeInt operator=(int value)
    * This overloads the '=' to assign an integer value.

* ObjectTypeDouble operator=(double value)
    * This overloads the '=' to assign a double value.

* ObjectTypeInt operator=(ObjectTypeInt value)
    * This overloads the '=' to copy an integer value from one object to another.

* ObjectTypeDouble operator=(ObjectTypeDouble value)
    * This overloads the '=' to copy a double value from one object to another.

###### Addition

* int operator+(int value)
    * This overloads the '+' to add an integer to an object and returning the value.

* int operator+(ObjectTypeInt value)
    * This overloads the '+' to enable the addition between objects and returns the sum of the value.

###### Subtraction

* int operator-(int value)
    * This overloads the '-' to subtract an integer to an object and returning the value.

* int operator-(ObjectTypeInt value)
    * This overloads the '-' to enable the subtraction between objects and returns the sum of the value.

###### Multiplication

* int operator*(int value)
    * This overloads the '*' to multiply an integer to an object and returning the value.

* int operator*(ObjectTypeInt value)
    * This overloads the '*' to enable the multiplication between objects and returns the sum of the value.

###### Division
* int operator/(int value)
    * This overloads the '/' to divide an integer to an object and returning the value.

* int operator/(ObjectTypeInt value)
    * This overloads the '/' to enable the division between objects and returns the sum of the value.
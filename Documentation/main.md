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

This handles the reading of characters, identifies and categorizes tokens and then displays the identified tokens or reports errors for each token.

#### Constructor

* LexicalAnalyzer(std::string filename)
    * This initializes the object and setting various member variables. This includes the preparation for tokenizing and analyzing the content of a file. 

#### Methods

* void analyze()
    * This reads characters from a file, tokenizes the input by identifying language tokens, categorizes the tokens, and prints the identified tokens.

#### Checkers

 It determines if characters or strings correspond to specific tokens, This includes identifying alphabetic characters, digits, operators, or language keywords.

##### Alphabetic Characters

 * std::map<char, LanguageToken> alphabet
    * This contains the alphabet fof the language.

* LanguageToken isAlphabet(char c)
    * This will verify if the character in in the Alphabet.

##### Digits

* std::map<char, LanguageToken> numberAlphabet
    * This contains the digits of the language. 

*  LanguageToken isDigit(char c)
    * This will verify the validity of the character as a digit.

##### Operators

* std::map<char, LanguageToken> operatorAlphabet
    * This contains the used operators of the language.

* LanguageToken isOperator(char c)
    * This verifies the validity of the character as an operator. 

##### Keywords

* std::map<std::string, LanguageToken> LanguageKeywords
    * This contains the keywords of the langugae such as (if, integer, and float).

* LanguageToken isKeyword(std::string str)
    * This verifies the character as a valid keyword.


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
# Main Code Documentation


## Classes

This section list and define the classes of the program including their methods and functions.

### HLint

Interface for the whole interpreter. This initiates the lexical analysis process in the interpreter. 

#### Constructors

* `HLint(std::string filenam)`
    * Initializes an instance of the HLint class and associates it with a LexicalAnalyzer.

#### Destructor

* `~HLint()`
    * Deletes the dynamically allocated LexicalAnalyzer object. Freeing the memory associated with it.

#### Methods

* `start()`
    * Starts the Interpreter. This triggers the lexical analysis process.





### SymbolTable

Store and maintain information amongst the classes. This include creating, deleting, modifying, and reading information created by the program.

#### Constructor

* `SymbolTable()`
    * Private Constructor to deploy a singleton class.

* `SymbolTable(SymbolTable const&)`
    * Private Copy Constructor to avoid multiple copy of the class.

#### Methods

* `void operator=(SymbolTable const&) = delete`
    * Deleted assignment operator since the SymbolTable is a sigleton.






### LexicalAnalyzer

This handles the reading of characters, identifies and categorizes tokens and then displays the identified tokens or reports errors for each token.

#### Constructor

* `LexicalAnalyzer(std::string filename)`
    * This initializes the object and setting various member variables. This includes the preparation for tokenizing and analyzing the content of a file. 

#### Methods

* `void analyze()`
    * This reads characters from a file, tokenizes the input by identifying language tokens, categorizes the tokens, and prints the identified tokens.

#### Checkers

 It determines if characters or strings correspond to specific tokens, This includes identifying alphabetic characters, digits, operators, or language keywords.

##### Alphabetic Characters

* `LanguageToken isAlphabet(char c)`
    * This will verify if the character in in the Alphabet.

##### Digits

*  `LanguageToken isDigit(char c)`
    * This will verify the validity of the character as a digit.

##### Operators

* `LanguageToken isOperator(char c)`
    * This verifies the validity of the character as an operator. 

##### Keywords

* `LanguageToken isKeyword(std::string str)`
    * This verifies the character as a valid keyword.

### Auxiliary Functions

* This processes and categorizes language tokens during code analysis. This includes determining token types and handling specific cases and conditions.

* `void processDigit(std::string& token_value, char &c, bool &isDigit)`
    * Determines whether the incoming character should be considered part of an identifier or the beginning of a literal number

* `void processOperator(std::string& token_value, char &c, bool &isDigit, std::vector<LanguageToken> &line_token, LanguageToken &tokenType)`
    * Handles the processing of operators, categorizes characters as part of identifiers, keywords, or literals, and checks conditional operators and quoted strings.

* `void processIdentifier(std::string& token_value, char& c, bool& isDigit)`
    * Appends the characters to the token_value while checking if the identifier is valid. Outputs an error if the identifier starts with a number.

* `void processCheckConditionalOperator(char &c, char &next, LanguageToken &tokenType)`
    * Checks both the current and following characters if it forms a conditional operator and updates the tokenType.






### ErrorHandler

This will handle the logging of errors within the program, put it in a file and the console. This provides various error handling functions.

#### Constructors

*  `ErrorHandler()`
    * It initializes an instance and opens an error log file. 

* `ErrorHandler(ErrorHandler const&)`
    * This is a private copy constructor that prevents the instantiation of multiple copies of the class.

### Destructor

* `~ErrorHandler()`
    * Performs cleanup when the object is no longer needed. This ensures that any errors are saved and that the error log file is closed.

#### Methods

* `static ErrorHandler& getInstance()`
    * Makes sure that only one instance of the clas exists and provides access to that instance.

* `void errorBreakdown()`
    * Formats and Displays an error breakdown message to the console.

* `void saveError()`
    * Saves the error message to a log file

* `bool displayError()`
    * Checks if there are any errors. This includes displaying the error breakdown and saves the error to a log file using the errorBreakdown() and saveError(). 

* `void displaySuccess(std::string at, std::string message)`
    * Display a success message with location information and the message itself if debugging is enabled.

* `void addError(std::string error)`
    * Adds an error message, increments error count, sets the error flag to true, and appends error message to the internal error string.

* `void addError(std::string error, int line, int column)`
    * Allows specifying line and column information for the error. This appends error message with line and column details to the internal error string.

* `int getErrorCount()`
    * Returns the count of errors. It indicates the number of errors that occured.

#### Debug

* `void _debug(std::string message)`
    * Adds a debug message to the error log when debugging is active.





### LanguageDictionary

Serves as a data structure. It stores language-related tokens, keywords, and character mappings used for lexical analysis.

#### Constructors

* `LanguageDictionary()`
    * Private Constructor to deploy a singleton class.

* `LanguageDictionary(LanguageDictionary const&)` 
    * Prevents the creation of new instances by deleting both the copy constructor and the assignment operator.

#### Methods

* static LanguageDictionary& getInstance()
    * Ensures that there is only one instance of the LanguageDictionary class.

* std::string getTokenName(LanguageToken token)
    * Returns the name of a language token as a string, to easily identify tokens.

* std::map<std::string, LanguageToken>& getLanguageKeywords()
    * Provieds access and returns a map of language keywords.

* std::map<std::string, LanguageToken>& getOperatorAlphabet()
    *  Provides access to and returns a map that associates operator strings.

* std::map<std::string , LanguageToken>& getConditionalOperator()
    * Provides access and returns a map with conditional operators.

* std::map<char, LanguageToken>& getNumberAlphabet()
    * Provides access to and returns a map. This includes individual digits (0-9).

* std::map<char, LanguageToken>& getAlphabet()
    * Provides access to and returns a map including the alphanumeric characters (letters, digits, and underscore).

* std::map<std::string, LanguageToken>& getDoubleOperator()
    * Provides access to and returns a map with Double Operators.

#### Alphabetic Characters

* std::map<char, LanguageToken> _alphabet
    * This contains the alphabet fof the language. This includes the uppercase and lowercase forms, underscore, and numbers.

#### Digits

* std::map<char, LanguageToken> _numberAlphabet
    * This contains the digits of the language from 0 to 9. 

#### Operator Alphabet

* std::map<char, LanguageToken> _operatorAlphabet
    * This contains the used operators of the language such as ('+', '-', '*', ';', ':', ':=', '=', '\', '==', '!=', '<<', '<', '>', '(', ')', '!').

#### Conditional Operators

* std::map<std::string, LanguageToken> _conditionalOperator
    * This contains the operators used for conditional statements. This includes the ('<', '>', '==', '!=').

#### Double Operators

* std::map<std::string, LanguageToken> _doubleOperator
    * This contains the double-character operators. This includes the ('<<', ':=', '==', '!=').

#### Keywords

* std::map<std::string, LanguageToken> _languageKeywords
    * This contains the keywords of the langugae such as (if, integer, double, and output).







### SyntaxAnalyzer

It performs syntax analysis on lines of code. This includes detecting, ensuring it complies to this language's syntax rules, and reports errors.

#### Constructor

* SyntaxAnalyzer()
    * This initialized the SyntaxAnalyzer by setting some initial state variables and obtaining an instance of the ErrorHandler class.

#### Methods

* bool analyze(std::vector<LanguageToken> line_token)
    * Examines the first token in a sequence, delegates analysis and error handling to other methods based on the token's type, and returns the results.

* bool identifier(std::vector<LanguageToken> &line_token, int position)
    * Analyzes a sequence of language tokens, checks the type of the token, and either continues the analysis by calling related methods or reports errors.

* bool declaration(std::vector<LanguageToken> &line_token, int position)
    * Examines language tokens, checks the validuty of token as a varibale type declaration, and reports errors.

* bool assignment(std::vector<LanguageToken> &line_token, int position)
    * Analyzes the assignment operators, checks the validty, and reports errors.

*  bool oneWayIfCondition(std::vector<LanguageToken> &line_token, int position)
    * Analyzes one-way if conditions, validates it by checking the formatting of conditions, and reports errors.

*  bool mathematicalExpression(std::vector<LanguageToken> &line_token, int position, bool conditional = false)
    * Examines and validates mathematical expressions by ensuring correct syntaxes. This handles the identifiers, literals, operators, and reports errors.

* bool output(std::vector<LanguageToken> &line_token, int position)
    * Analyzes and checks the validity of output statements. This includes the LeftShiftToken, valid mathematical expression, and reports errors.  

* bool parenthesisBalancer(std::vector<LanguageToken> &line_token, int& position, bool open)
    * This verifies that open and close parentheses are balanced. 

* bool isAuxParenthesis(LanguageToken& token, bool open)
    * Used within parenthesisBalancer(), verifies if a LanguageToken is an open or close parenthesis, balances and reports error.

* bool isParenthesis(LanguageToken& token)
    * Checks and counts all parentheses, reporting an error for unbalanced parentheses.

#### Auxiliary Checker

* bool isIdentifierOrLiteral(LanguageToken token)
    * Checks if the LanguageToken is an identifier, literal, number, or string and returns true if it matches.

* bool isConditionalOperator(LanguageToken token)
    * Checks if the LanguageToken is a conditional operator. This includes ('>', '<', '==', '!='), and returns true if the token matches.

* bool isOperator(LanguageToken token)
    * Checks if the LanguageToken is an operator. This includes ('+' and '-') and returns true if the token matches.





### AbstractSyntaxTree


### Interpreter





## Variables

* `std::map<std::string, ObjectType*> variableTable;`
    * Contains all information about variables that is being used in the program.

* ``
    * 

* ``
    * 

* ``
    * 






#### Auxillary Class: ObjectType

Provides a basic framework for handling mathematical operators, data type declarations, and obtaining values.

##### Constructor

* `ObjectTypeInt(std::string name)`
    * This sets the type of the object to a certain data type such as "integer."

* `ObjectTypeInt(std::string name, int value)`
    * Initializes an object with a given name and integer value, specifying the type as "integer."

* `ObjectTypeInt(int value)`
    * Sets the value of an ObjectTypeInt object to an integer and specifies its data type as "integer.

* `ObjectTypeInt(ObjectTypeInt &value)`
    * Creates an ObjectTypeInt object by copying the integer value from another ObjectTypeInt object and setting its type to "integer."

* `ObjectTypeInt(double value)`
    * Creates an object of the ObjectTypeInt class with a double value and converts it to an integer, setting the object's type to "integer."

##### Implicit Conversion

* `operator int()`
    * Defines a conversion operator that converts an object into an integer. This returns an integer value.

* `operator double()`
    * Defines a conversion operator that converts an object into a double. This returns a double value.

##### Methods

* `std::string getName()`
    * This gets access from the name attribute and returns its value.

* `std::string getType()`
    * This gets access from the type attribute and returns its value.

* `int getValue()`
    * This gets access from the private attribute value and return its value.

* `void setValue(int value)`
    * Allows you to change the value stored in the object to a new integer value.

* `void setValue(ObjectTypeInt value)`
    * Copies the value from another object and assigns it to the current object's value attribute.

##### Operators

* It defines the operators used and how they behave within the program. This includes the assignment, addition, subtraction, multiplication, and division.
    * The process has two classes: ObjectTypeInt (deals with integer values) and ObjectTypeDouble (deals with double values).
    * Under the following arithmetic operators such as +, -, *, /, the proccess done in (int values) is applied to (double values)  

###### Assignment

* `ObjectTypeInt operator=(int value)`
    * This overloads the '=' to assign an integer value.

* `ObjectTypeDouble operator=(double value)`
    * This overloads the '=' to assign a double value.

* `ObjectTypeInt operator=(ObjectTypeInt value)`
    * This overloads the '=' to copy an integer value from one ObjectTypeInt object to another.

* `ObjectTypeDouble operator=(ObjectTypeDouble value)`
    * This overloads the '=' to copy a double value from one object to another.

###### Addition

* `ObjectTypeInt operator+(int value)`
    * This overloads the '+' to add an integer to an object and returning the value.

* `ObjectTypeInt operator+(ObjectTypeInt value)`
    * This overloads the '+' to enable the addition between objects and returns the sum of the value.

###### Subtraction

* `int operator-(int value)`
    * This overloads the '-' operator to subtract an integer from an object and returns the value.

* `int operator-(ObjectTypeInt value)`
    * This overloads the '-' operator to enable subtraction between objects and returns the difference of the value.

###### Multiplication

* `int operator*(int value)`
    * This overloads the '*' operator to multiply an object by an integer and returns the value.

* `int operator*(ObjectTypeInt value)`
    * This overloads the '*' operator to enable multiplication between objects and returns the product value.

###### Division
* `int operator/(int value)`
    * This overloads the '/' operator to divide an object by an integer. It includes error handling for division by zero.

* `int operator/(ObjectTypeInt value)`
    * This overloads the '/' to enable the division between objects and returns the quotient of the value.

##### Comparisons

Defines a set of comparison operators for a class, It compares an object with either an integer or another object. Same process is applied to ObjectTypeDouble and ObjectTypeString.

* `bool operator==(int value)`
    * Checks if an object is equal to an integer.

* `bool operator==(ObjectTypeInt value)`
    * Checks if two objects have the same integer value.

* `bool operator!=(int value)`
    * Checks if the object's value is different from a given integer.

* `bool operator!=(ObjectTypeInt value)`
    * Checks if the object's value is different from the value of another.

* `bool operator>(int value)`
    * Checks if the value of an object is greater than a given integer.

* `bool operator>(ObjectTypeInt value)`
    * Checks if the value of an object is greater than the value of another object.

* `bool operator<(int value)`
    * Checks if the value of an object is less than a given integer

* `bool operator<(ObjectTypeInt value)`
    * Checks if the value of an object is less than the value of another object.

* ` bool operator>=(int value)`
    * Checks if the value of an object is greater than or equal to a given integer

* `bool operator>=(ObjectTypeInt value)`
    * Checks if the value of an object is greater than or equal to the value of another object.

* `bool operator<=(int value)`
    * Checks if the value of an object is less than or equal to a given integer

* `bool operator<=(ObjectTypeInt value)`
    * Checks if the value of an object is less than or equal to the value of another object.
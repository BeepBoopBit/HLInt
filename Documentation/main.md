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
    * Private Constructor to deploy a singleton class. Initializes the symbol table.

* `SymbolTable(SymbolTable const&)`
    * Private Copy Constructor to avoid multiple copy of the class.

#### Methods

* `void operator=(SymbolTable const&) = delete`
    * Deleted assignment operator since the SymbolTable is a singleton.

* `static SymbolTable& getInstance(){`
    * Ensures that there's only one instance of the class and returns it.

* `void declare(std::string name, ObjectType* variable)`
    * Adds a variable with a specified name and its data type, throws an error if it already exists.

* `ObjectType* get(std::string name)`
    * Retrieves the variable associated with a given name from the symbol table and returns it. Throws an error if it doesn't exist.

* `void set(std::string name, ObjectType* variable)`
    * sets the variable associated with a given name in the symbol table. Throws an error if it doesn't exist.

* `void remove(std::string name)`
    * Removes a variable from the symbol table if it exists. Throws an error if it doesn't exist.

* `ObjectTypeInt* parseToInt(ObjectType* variable)`
    * Converts a variable to an integer type and returns it. If conversion is not possible, it prints an error message and returns 'nullptr'.

* `ObjectTypeDouble* parseToDouble(ObjectType *variable)`
    * Converts a variable to a double type and returns it. If conversion is not possible, it prints an error message and returns 'nullptr'.

* `ObjectTypeString* parseToString(ObjectType *variable)`
    * Converts a variable to a string type and returns it. If conversion is not possible, it prints an error message and returns 'nullptr'.

##### Debug

* `void printVariableTable()`
    * Prints the contents of the variable table to the console. Displays the variable names and their values.

##### Checkers

* `bool isVariable(std::string token)`
    * Checks if a given string exists in the variable table. It either returns true or fales depending if it exists or not.


### LexicalAnalyzer

This handles the reading of characters, identifies and categorizes tokens and then displays the identified tokens or reports errors for each token.

#### Constructor

* `LexicalAnalyzer(std::string filename)`
    * This initializes the object and setting various variables. This includes the preparation for tokenizing and analyzing the content of a file. 

#### Destructor

* `~LexicalAnalyzer()`
    * Ensures that open files are properly closed to prevent resource leaks and saves symbol information before the LexicalAnalyzer instance is destroyed.

#### Methods

* `void analyze()`
    * This reads characters from a file, tokenizes the input by identifying language tokens, categorizes these tokens, and manages their processing.  

* `bool isEndOfStatement(char c)`
    * Checks if a given character is a semicolon.

* `void processDigit(char c)`
    * Reads characters from a file that starts with a digit. It checks if the sequence represents either integer or double and adds the corresponding token to a data structure.

* `void processIdentifier(char c)`
    * Reads characters from a file to form an identifier. Checks if it's a keyword, and assigns the corresponding token or labels it as a general "IdentifierToken" if it's the latter.

* `void processOperator(char c)`
    * Processes operators, which can be either single or double operators, and inserts them into the Abstract Syntax Tree.

* `void processStringLiteral()`
    * Extracts and tokenizes a string literal enclosed in double quotation marks from the input code, adding it to the Abstract Syntax Tree.

#### Checkers

 It determines if characters or strings correspond to specific tokens, This includes identifying alphabetic characters, digits, operators, or language keywords.

##### Method

* `bool detectIfASign(char c)`
    * Determines if a character is a valid operator based on context and the previous token in the language.

* `LanguageToken isIdentifier(char c)`
    * Checks whether a given character is a valid identifier character.

*  `LanguageToken isDigit(char c)`
    * Checks whether a given character is a valid digit character.

* `LanguageToken isOperator(char c)`
    * Checks whether a given character is a valid operator.

* `LanguageToken isOperator(std::string str)`
    * Checks whether a given string is a valid operator.

* `LanguageToken isKeyword(std::string str)`
    * Checks if a given string is a language keyword by basing it in the language's keyword dictionary.

* `bool isInFileGood()`
    * Checks if a file is successfully opened and prints a message indicating success or failure.

* `bool isOutFileGood()`
    * Checks if the output file is open. Verifes if the output file is accessible and open for writing.

* `void closeFiles(int fileIndex, std::string filename)`
    * It can close either the input or output file based on the provided fileIndex. This also includes an optional debug messages.


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

* `static LanguageDictionary& getInstance()`
    * Ensures that there is only one instance of the LanguageDictionary class.

* `std::string getTokenName(LanguageToken token)`
    * Returns the name of a language token as a string, to easily identify tokens.

* `std::map<std::string, LanguageToken>& getLanguageKeywords()`
    * Provieds access and returns a map of language keywords.

* `std::map<std::string, LanguageToken>& getOperatorAlphabet()`
    *  Provides access to and returns a map that associates operator strings.

* `std::map<std::string , LanguageToken>& getConditionalOperator()`
    * Provides access and returns a map with conditional operators.

* `std::map<char, LanguageToken>& getNumberAlphabet()`
    * Provides access to and returns a map. This includes individual digits (0-9).

* `std::map<char, LanguageToken>& getAlphabet()`
    * Provides access to and returns a map including the alphanumeric characters (letters, digits, and underscore).

* `std::map<std::string, LanguageToken>& getDoubleOperator()`
    * Provides access to and returns a map with Double Operators.

#### Alphabetic Characters

* `std::map<char, LanguageToken> _alphabet`
    * This contains the alphabet fof the language. This includes the uppercase and lowercase forms, underscore, and numbers.

#### Digits

* `std::map<char, LanguageToken> _numberAlphabet`
    * This contains the digits of the language from 0 to 9. 

#### Operator Alphabet

* `std::map<char, LanguageToken> _operatorAlphabet`
    * This contains the used operators of the language such as ('+', '-', '*', ';', ':', ':=', '=', '\', '==', '!=', '<<', '<', '>', '(', ')', '!').

#### Conditional Operators

* `std::map<std::string, LanguageToken> _conditionalOperator`
    * This contains the operators used for conditional statements. This includes the ('<', '>', '==', '!=').

#### Double Operators

* `std::map<std::string, LanguageToken> _doubleOperator`
    * This contains the double-character operators. This includes the ('<<', ':=', '==', '!=').

#### Keywords

* `std::map<std::string, LanguageToken> _languageKeywords`
    * This contains the keywords of the langugae such as (if, integer, double, and output).


### SyntaxAnalyzer

It performs syntax analysis on lines of code. This includes detecting, ensuring it complies to this language's syntax rules, and reports errors.

#### Constructor

* `SyntaxAnalyzer()`
    * This initialized the SyntaxAnalyzer by setting some initial state variables and obtaining an instance of the ErrorHandler class.

#### Methods

* `bool analyze(std::vector<LanguageToken> line_token)`
    * Examines the first token in a sequence, delegates analysis and error handling to other methods based on the token's type, and returns the results.

* `bool identifier(std::vector<LanguageToken> &line_token, int position)`
    * Analyzes a sequence of language tokens, checks the type of the token, and either continues the analysis by calling related methods or reports errors.

* `bool declaration(std::vector<LanguageToken> &line_token, int position)`
    * Examines language tokens, checks the validuty of token as a varibale type declaration, and reports errors.

* `bool assignment(std::vector<LanguageToken> &line_token, int position)`
    * Analyzes the assignment operators, checks the validty, and reports errors.

*  `bool oneWayIfCondition(std::vector<LanguageToken> &line_token, int position)`
    * Analyzes one-way if conditions, validates it by checking the formatting of conditions, and reports errors.

*  `bool mathematicalExpression(std::vector<LanguageToken> &line_token, int position, bool conditional = false)`
    * Examines and validates mathematical expressions by ensuring correct syntaxes. This handles the identifiers, literals, operators, and reports errors.

* `bool output(std::vector<LanguageToken> &line_token, int position)`
    * Analyzes and checks the validity of output statements. This includes the LeftShiftToken, valid mathematical expression, and reports errors.  

* `bool parenthesisBalancer(std::vector<LanguageToken> &line_token, int& position, bool open)`
    * This verifies that open and close parentheses are balanced. 

* `bool isAuxParenthesis(LanguageToken& token, bool open)`
    * Used within parenthesisBalancer(), verifies if a LanguageToken is an open or close parenthesis, balances and reports error.

* `bool isParenthesis(LanguageToken& token)`
    * Checks and counts all parentheses, reporting an error for unbalanced parentheses.

#### Auxiliary Checker

* `bool isIdentifierOrLiteral(LanguageToken token)`
    * Checks if the LanguageToken is an identifier, literal, number, or string and returns true if it matches.

* `bool isConditionalOperator(LanguageToken token)`
    * Checks if the LanguageToken is a conditional operator. This includes ('>', '<', '==', '!='), and returns true if the token matches.

* `bool isOperator(LanguageToken token)`
    * Checks if the LanguageToken is an operator. This includes ('+' and '-') and returns true if the token matches.


### AbstractSyntaxTree

Processes input code, organizes it into a hierarchical tree structure, and checks its validity based on the syntax and semantics of a specific language, allowing for operations like parsing, evaluation, and debugging.

#### Constructors

* `AST()`
    * Empty constructor defined for the AST class.

* `AST(AST const&) = delete`
    * Deletes the copy constructor to prevent the creation of multiple copies of the class.

#### Destructor

* `~AST()`
    * Empty destructor defined for the AST class.

#### Methods

* `void operator=(AST const&) = delete`
    * Removes assignment operator since the AST is a singleton.

* `static AST& getInstance()`
    * Ensures that there's only one instance of the class and returns it.

* `void insert(LanguageToken token, std::string value = "", int line = 0, int column = 0)`
    * Manages the insertion of language tokens and their values into the abstract syntax tree. To ensure proper parsing and ordering of operations.

* `void evaluateTree()`
    * Iterates through a collection of statements or expressions in AST and evaluates each of them.

* `void print()`
    * Displays the structure of the entire AST. This includes individual tree within it.

* `std::vector<AuxillaryTree*> getTrees()`
    * Returns a vector containing all the individual trees within the AST.

* `AuxillaryTree* findValidTree(AuxillaryTree* &tree)`
    * Used to find the rightmost or outermost valid subtree by searching through an AuxillaryTree.

* `void processToken(LanguageToken& token, std::string& value)`
    * Manages the construction of the AST by appropriately updating _latestSmallTree based on the provided tokens and values.

* `AuxillaryTree* processOperator(LanguageToken token, std::string value, AuxillaryTree* tree)`
    * Processes operators and keywords while constructing the AST.

* `void processEndOfStatement()`
    * Merging of small trees into the main AST. Ensures the correct order of operations and overall tree structure.

* `void processCloseParenthesis(LanguageToken &token, std::string &value)`
    * Handles the completion and merging of small trees in the AST when encountering close parenthesis. This includes operator precedence and conditional statements.

* `void processParenthesisOrOperator(LanguageToken &token, std::string &value, bool &isConditional)`
    * Handles the processing of parentheses and conditional statements in the AST. Maintains their precedence and the correct order of operations.

* `bool evaluateTree(AuxillaryTree*& tree)`
    * Evaluates a binary tree structure. Checks that each node in the tree meets certain evaluation criteria. Returns true if it passes, and false if it's the latter.

* `bool processEvaluation(AuxillaryTree* &tree)`
    * Verifies whether the node meets the expected criteria for its token type. Flags any errors encountered in the process.

* `bool expect(AuxillaryTree* tree, bool (AST::*function)(AuxillaryTree*), int pathway = 2)`
    * Simplifies the syntax for evaluating specific conditions on subtrees of the AST. 

* `bool expect(AuxillaryTree* tree, bool (AST::*lhs_func)(AuxillaryTree*), bool (AST::*rhs_func)(AuxillaryTree*))`
    * Allows for separate evaluations of the left and right subtrees of an AuxillaryTree.

* `bool isLiteralOrIdentifier(AuxillaryTree* tree)`
    * Checks whether the tree represents a literal, an identifier, or certain number types.

* `bool isIdentifier(AuxillaryTree* tree)`
    * Checks whether the tree represents an identifier.

* `bool isConditional(AuxillaryTree* tree)`
    * Checks whether the tree represents a conditional operator.

* `bool isAStatement(AuxillaryTree* tree)`
    * Checks whether the tree represents a statement.

* `bool isNull(AuxillaryTree* tree)`
    * Checks whether the tree is null.

* `bool isMathematical(AuxillaryTree *tree)`
    *  Checks whether the tree is a valid mathematical expression.

* `bool isPrintable(AuxillaryTree *tree)`
    * Checks whether the tree is a valid expression that can be printed.

* `bool isInputable(AuxillaryTree *tree)`
    * Checks whether the tree is a valid expression that can be used as input.

* `bool declarable(AuxillaryTree *tree)`
    * Checks whether the tree is a valid expression for declaration.

* `bool isKeyword(AuxillaryTree* tree)`
    * Checks whether the tree is a valid keyword. Considers if the tree's token matches one of the predefined keyword tokens.

* `bool isComparable(AuxillaryTree* tree)`
    * Checks whether the tree is a valid expression that can be used for comparison.

* `void printBT(const std::string& prefix, const AuxillaryTree* node, bool isLeft)`
    * Used to print a binary tree, displaying each node's value and its position within the tree.

* `void printBT(const AuxillaryTree* node)`
    * Simplifies the process of printing a binary tree structure by calling the printBT() with default parameters.

* `bool isDoubleOperator(std::string value)`
    * Checks if a given string is a double operator based on a predefined set of double operators in the language dictionary.

* `bool isOperator(std::string value)`
    * Checks if a given string is an operator. Compares value with operator strings obtained from _languageDictionary. 

* `bool isKeyword(std::string value)`
    * Checks if a given string is a keyword. Compares it with a set of language keywords from _languageDictionary.

* `bool isConditionalOperator(std::string value)`
    * Checks if a given string is a conditional operator. Compares the value with conditional operator strings from _languageDictionary.

* `bool isMultiplicationOrDivision(LanguageToken token)`
    * Checks whether it represents a multiplication or division operation.

* `AuxillaryTree* createTree(LanguageToken& token, std::string& value)`
    * Used to construct and initialize AuxillaryTree nodes with the given token and value. Adds line and column from the new created instance.

* `AuxillaryTree* createTree(LanguageToken& token, std::string& value, int line, int column)`
    * Ultimately does the same process as the previous createTree() but with additional parameters for specifications.


### Interpreter

This interprets and executes the code. Its functionalities includes interpretation of abstract syntax trees, mathematical expressions, variable handling, I/O handling, conditionals, and error handling.   

#### Constructors

* ` Interpreter()`
    * Private Constructor to deploy a singleton class.

* `Interpreter(const Interpreter&) = delete`
    * Deletes the copy constructor to prevent the creation of multiple copies of the class.

#### Methods

* `Interpreter& operator=(const Interpreter&) = delete`
    * Deleted assignment operator since the Interpreter is a singleton.

* `static Interpreter& getInstance()`
    * Ensures that there's only one instance of the class and returns it.

* `void interpret(AuxillaryTree* &tree, bool isInterpretAll = false)`
    * Processes an abstract syntax tree. It evaluates various language tokens and executes math operations, assignments, input/output, conditions, and variable declarations.

* `void handleIfCondition(AuxillaryTree* tree)`
    * Handles "if" conditions. Checks condition and executing the enclosed statements if the condition evaluates to true.

* `bool handleCondition(AuxillaryTree* tree)`
    * Evaluates conditions by comparing values or expressions.

* `void handleOutput(AuxillaryTree* tree)`
    * Handles different cases of output operations and prints the appropriate values to the console. This includes printing strings, variable, and numeric values.

* `void handleInput(AuxillaryTree* &tree)`
    * Handles reading input and storing it in the correct variable based on the variable's type.

* `void handleDeclaration(AuxillaryTree* &tree)`
    * Handles variable declarations by identifying their data type, creating instances of corresponding data type classes, and managing their scope.

* `void handleAssignment(AuxillaryTree* &tree)`
    * Handles variable assignments by evaluating mathematical expressions on the right-hand side and updating the assigned variable's value.

* `double evaluateMathematicalExpression(AuxillaryTree* &tree)`
    * Calculates the result of a mathematical expression found in the provided tree, considering various mathematical operations and variable values.

* `bool isDigit(std::string value)`
    * Checks if the first character of a string value is a digit.

* `bool isDigit(char value)`
    * Checks if a given character is a digit by comparing it to a set of valid digits defined in the _languageDictionary.

* `bool isIdentifier(std::string value)`
    * Checks if the first character of a string value is a valid identifier. Compares it to a set of valid identifier characters defined in the _languageDictionary. 

* `bool isIdentifier(char value)`
    * Checks if a given character is valid. Compares it to a set of valid identifier characters defined in the _languageDictionary.

* `LanguageToken getNumberType(AuxillaryTree* &tree)`
    * Checks the _token of the provided tree and returns the corresponding numeric type. if it doesn't match, it throws a runtime error that says "Invalid Number Type."

* `void deleteReplaceTree(AuxillaryTree* &tree, LanguageToken token, std::string value)`
    * It updates the content of an AuxillaryTree node with new values and clears any related existing data.

* `std::string getTreeValues(AuxillaryTree* &tree)`
    * It builds and returns a string by combining the values of an AuxillaryTree and its child nodes through recursive traversal.

### TestCaseHandler

Facilitate the execution and management of test cases. 

#### Constructor

* `TestCaseHandler()`
    * Empty constructor defined for the TestCaseHandler class.

#### Destructor

* `~TestCaseHandler()`
    * Empty destructor defined for the TestCaseHandler class.

#### Method

* `void runTestCases()`
    * Runs test cases and displays a message indicating that it's running.

## Variables

* `std::map<std::string, ObjectType*> variableTable;`
    * Contains all information about variables that is being used in the program.

* `std::string _filename = "RES_SYM.txt";`
    * Initializes a variable named _filename with the string value "RES_SYM.txt"

* `std::ofstream _file`
    * Declares a variable named _file as an output file stream.

* `std::string _filename = "test.txt`
    * Initializes a variable _filename with the value "test.txt". Sets the default filename.

* `std::string _outfile = "NOSPACES.txt"`
    * Initializes a variable _outfile with the value "NOSPACES.txt". Sets the default filename.

* `std::ifstream _file`
    * Declares a variable _file with input file stream. It is for reading data from an input file.

* `std::ofstream _oFile`
    * Declares a variable _oFile with an output file stream. It is used for writing data to an output file.

* `int _line`
    * Records the current line number in the input file for error handling.

* `int _column`
    * Records the current column number in the input file for error handling.

* `int _errorCount`
    * Records current error count for the file.

* `std::string _totalStringNoSpace = ""`
    * Stores a string without spaces. Used for further processing and output file.

* `LanguageToken _prevToken = LanguageToken::InvalidToken`
    * Records the previous language token. Used for sign identification.

* `std::string _prevValue = ""`
    * Initialized with an empty string. Records the previous value and used for sign identification.  

* `bool _hasEndedSuccessfully = false`
    * Initially set to false. Indicates whether the file has ended successfully with a semicolon.

* `std::string _errorLogPath = "ERROR.log"`
    * Private string variable initialized with a string value "ERROR.log" and represents the path to the error log file.

* `std::ofstream _errorLog`
    * Private instance of the std::ofstream class, used for writing error messages to a file.

* `bool _isDebug = true`
    * Private boolean variable that indicates whether the code is in debug mode or not.

* `std::string _errorString = ""`
    * Private string variable used to store error messages.

* `bool _hasError = false`
    * Private boolean variable that indicates whether an error has occurred.

* `bool _hasAlreadyDisplayed = false`
    * Private boolean variable used to keep track of whether an error has already been displayed.

* `int _errorCount = 0`
    * Private integer variable that keeps count of the number of errors.

* `ErrorHandler* _errorHandler = &ErrorHandler::getInstance()`
    * It's used to log errors.

* `LanguageDictionary* _languageDictionary = &LanguageDictionary::getInstance()`
    * It's used to get the tokens

* `SymbolTable* _symbolTable = &SymbolTable::getInstance()`
    * It's used to store the variables.

* `int _parenthesisCount = 0`
    * This is an integer used to check if the parenthesis are balanced.

* `AuxillaryTree* _root = nullptr`
    * This is a pointer to the root of the tree.

* `std::vector<AuxillaryTree*> _totalityTree`
    * It contains all the statement trees from all lines.

* `std::vector<AuxillaryTree*> _smallTrees`
    * It contains the statement trees from a single line.

* `AuxillaryTree* _latestSmallTree = nullptr`
    * This is a pointer to the latest small tree. It's used to push the small tree to the totality tree.

* `int _currentSmallTreeIndex = 0`
    * This is an integer used to push the small tree to the totality tree.

* `int _line = 0`
    * This is an integer used for better error handling. It represents the current line.

* `int _column = 0`
    * This is an integer used for better error handling. It represents the current column.

* `bool _isConditional = false`
    * This is a boolean used to check if the current small tree is a conditional statement.

* `LanguageToken _token = LanguageToken::InvalidToken`
    * Declares a variable _token and initializes it to the value LanguageToken::InvalidToken.

* ` AuxillaryTree* _left = nullptr`
    *  Declares a variable _left and initializes it to nullptr.

* `AuxillaryTree* _right = nullptr`
    * Declares a variable _right and initializes it to nullptr.

* `std::string _value = ""`
    * Declares a variable _value and initializes it to an empty string.

* `int _line = 0`
    * Declares a variable _line and initializes it to 0.

* `int _column = 0`
    * Declares a variable _column and initializes it to 0.

* `std::string _parentDir = "../TestCases/";`
    * Initializes a variable _parentDir with the string value "../TestCases/".


#### Auxillary Class: AuxillaryTree

Provides a structure for individual nodes in the tree and includes information such as the token type, left and right child pointers, the value, and location information.

##### Constructor

* `AuxillaryTree(LanguageToken token, std::string value, int line, int column): _token(token), _value(value), _line(line), _column(column)`
    * Initializes an AuxillaryTree object with the provided token, value, line, and column values.

##### Destructor

* `~AuxillaryTree()`
    * Empty destructor defined for the AuxillaryTree class.


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

* `operator int()`
    * This is under the ObjectTypeDouble class. Turns an object into a regular whole number by removing its decimal part.

* `operator double()`
    * This is under the ObjectTypeDouble class. It essentially does the same, it starts with a double value and returns the same.  

* `operator std::string()`
    * Transform an object of the String class into a regular string.

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
    * The process has three classes: ObjectTypeInt (deals with integer values), ObjectTypeDouble (deals with double values) and ObjectTypeString (deals with string values).
    * Under the following arithmetic operators such as +, -, *, /, the proccess done in (int values) is applied to both (double values) and (string values). 

###### Assignment

* `ObjectTypeInt operator=(int value)`
    * This overloads the '=' to assign an integer value.

* `ObjectTypeDouble operator=(double value)`
    * This overloads the '=' to assign a double value.

* `ObjectTypeInt operator=(ObjectTypeInt value)`
    * This overloads the '=' to copy an integer value from one ObjectTypeInt object to another.

* `ObjectTypeDouble operator=(ObjectTypeDouble value)`
    * This overloads the '=' to copy a double value from one object to another.

* `ObjectTypeString operator=(std::string value)`
    * This overloads the '=' to assign a string value.

* `ObjectTypeString operator=(ObjectTypeString value)`
    * This overloads the '=' to copy a string value from one object to another.

###### Addition

* `ObjectTypeInt operator+(int value)`
    * This overloads the '+' to add an integer to an object and returning the value.

* `ObjectTypeInt operator+(ObjectTypeInt value)`
    * This overloads the '+' to enable the addition between objects and returns the sum of the value.

###### Subtraction

* `ObjectTypeInt operator-(int value)`
    * This overloads the '-' operator to subtract an integer from an object and returns the value.

* `ObjectTypeInt operator-(ObjectTypeInt value)`
    * This overloads the '-' operator to enable subtraction between objects and returns the difference of the value.

###### Multiplication

* `ObjectTypeInt operator*(int value)`
    * This overloads the '*' operator to multiply an object by an integer and returns the value.

* `ObjectTypeInt operator*(ObjectTypeInt value)`
    * This overloads the '*' operator to enable multiplication between objects and returns the product value.

###### Division
* `ObjectTypeInt operator/(int value)`
    * This overloads the '/' operator to divide an object by an integer. It includes error handling for division by zero.

* `ObjectTypeInt operator/(ObjectTypeInt value)`
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
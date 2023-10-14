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



### ErrorHandler



## Variables

* std::map<std::string, ObjectType*> variableTable;
    * Contains all information about variables that is being used in the program.

#### Auxillary Class: ObjectType

This handles the mathematical operators within the program. This includes the declaration of data types, obtaining values, and different operators.

##### Constructor

* ObjectTypeInt(std::string name)
    * This sets the type of the object to a certain data type (Integer or Double)

##### Methods

* std::string getName()
    * This gets access from the name attribute and returns its value.
* std::string getType()
    * This gets access from the type attribute and returns its value.
* int getValue()
    * This gets access from the private attribute value and return its value.

##### Operators





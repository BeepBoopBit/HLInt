#ifndef LANGUAGEDICTIONARY_H
#define LANGUAGEDICTIONARY_H

#include <map>
#include <string>

// Contains all the tokens and language keywords
class LanguageDictionary{
public:
    // Tokens of the Language
    enum LanguageToken{
        CharacterToken,         // Used for Individual Characters (is not use in AST)
        IdentifierToken,        // Used for Identifiers
        NumberToken,            // Used for Numbers (is not use in AST)
        StringToken,            // Used for Strings
        // WhiteSpaceToken,     // 
        AdditionToken,          //
        SubtractionToken,       //
        // EndLineToken,        //
        AssignmentToken,        //
        EndOfStatementToken,    //
        ColonToken,             //
        InvalidToken,           //
        QuoteToken,             //
        LeftShiftToken,         //
        LessThanToken,          // 
        GreaterThanToken,       //
        EqualityToken,          //
        NotEqualToken,          //
        IfToken,                //
        OpenParenthesisToken,   //
        CloseParenthesisToken,  //
        LiteralToken,           //
        TypeIntegerToken,       //
        TypeDoubleToken,        //
        OutputToken,            //
        RootNode,               //
        // BranchNode,          //
        MultiplicationToken,    //
        NumberIntegerToken,     //
        NumberDoubleToken,      //
        EqualToken,
        DivisionToken,
        InputToken,
        RightShiftToken,
        TypeStringToken
    };
private:
    LanguageDictionary(){}
    ~LanguageDictionary(){}
public:
    static LanguageDictionary& getInstance(){
        static LanguageDictionary instance;
        return instance;
    }

    // Delete the copy constructor and assignment operator
    LanguageDictionary(LanguageDictionary const&) = delete;
    void operator=(LanguageDictionary const&) = delete;

public:

    std::map<std::string, LanguageToken>& getLanguageKeywords(){
        return _languageKeywords;
    }

    std::map<std::string, LanguageToken>& getOperatorAlphabet(){
        return _operatorAlphabet;
    }

    std::map<std::string , LanguageToken>& getConditionalOperator(){
        return _conditionalOperator;
    }

    std::map<char, LanguageToken>& getNumberAlphabet(){
        return _numberAlphabet;
    }
    
    std::map<char, LanguageToken>& getAlphabet(){
        return _alphabet;
    }

    std::map<std::string, LanguageToken>& getDoubleOperator(){
        return _doubleOperator;
    }

private:
    std::map<std::string, LanguageToken> _languageKeywords ={
        {"if", LanguageToken::IfToken},
        {"integer", LanguageToken::TypeIntegerToken},
        {"double", LanguageToken::TypeDoubleToken},
        {"output", LanguageToken::OutputToken},
        {"input", LanguageToken::InputToken},
        {"string", LanguageToken::TypeStringToken}
    };

    std::map<std::string, LanguageToken> _operatorAlphabet ={
        {"+", LanguageToken::AdditionToken},
        {"-", LanguageToken::SubtractionToken},
        {"*", LanguageToken::MultiplicationToken},
        {"/", LanguageToken::DivisionToken},
        {";", LanguageToken::EndOfStatementToken},
        {":", LanguageToken::ColonToken},
        {":=", LanguageToken::AssignmentToken},
        {"=", LanguageToken::EqualToken},
        {"\"", LanguageToken::QuoteToken},
        {"(", LanguageToken::OpenParenthesisToken},
        {")", LanguageToken::CloseParenthesisToken},
        {"!", LanguageToken::NotEqualToken},
        {"<", LanguageToken::LessThanToken},
        {">", LanguageToken::GreaterThanToken},
        {"==", LanguageToken::EqualityToken},
        {"!=", LanguageToken::NotEqualToken},
        {"<<", LanguageToken::LeftShiftToken},
        {">>", LanguageToken::RightShiftToken}
    };

    std::map<std::string, LanguageToken> _conditionalOperator ={
        {"<", LanguageToken::LessThanToken},
        {">", LanguageToken::GreaterThanToken},
        {"==", LanguageToken::EqualityToken},
        {"!=", LanguageToken::NotEqualToken}
    };

    std::map<std::string, LanguageToken> _doubleOperator{
        {"<<", LanguageToken::LeftShiftToken},
        {":=", LanguageToken::AssignmentToken},
        {"==", LanguageToken::EqualityToken},
        {"!=", LanguageToken::NotEqualToken},
        {">>", LanguageToken::RightShiftToken}
    };

    std::map<char, LanguageToken> _numberAlphabet ={
        {'0', LanguageToken::NumberToken},
        {'1', LanguageToken::NumberToken},
        {'2', LanguageToken::NumberToken},
        {'3', LanguageToken::NumberToken},
        {'4', LanguageToken::NumberToken},
        {'5', LanguageToken::NumberToken},
        {'6', LanguageToken::NumberToken},
        {'7', LanguageToken::NumberToken},
        {'8', LanguageToken::NumberToken},
        {'9', LanguageToken::NumberToken}
    };

    // Contains the alphabet of the language
    // I used map because it's faster to search than normal looping
    std::map<char, LanguageToken> _alphabet ={
        {'a', LanguageToken::CharacterToken},
        {'b', LanguageToken::CharacterToken},
        {'c', LanguageToken::CharacterToken},
        {'d', LanguageToken::CharacterToken},
        {'e', LanguageToken::CharacterToken},
        {'f', LanguageToken::CharacterToken},
        {'g', LanguageToken::CharacterToken},
        {'h', LanguageToken::CharacterToken},
        {'i', LanguageToken::CharacterToken},
        {'j', LanguageToken::CharacterToken},
        {'k', LanguageToken::CharacterToken},
        {'l', LanguageToken::CharacterToken},
        {'m', LanguageToken::CharacterToken},
        {'n', LanguageToken::CharacterToken},
        {'o', LanguageToken::CharacterToken},
        {'p', LanguageToken::CharacterToken},
        {'q', LanguageToken::CharacterToken},
        {'r', LanguageToken::CharacterToken},
        {'s', LanguageToken::CharacterToken},
        {'t', LanguageToken::CharacterToken},
        {'u', LanguageToken::CharacterToken},
        {'v', LanguageToken::CharacterToken},
        {'w', LanguageToken::CharacterToken},
        {'x', LanguageToken::CharacterToken},
        {'y', LanguageToken::CharacterToken},
        {'z', LanguageToken::CharacterToken},
        {'A', LanguageToken::CharacterToken},
        {'B', LanguageToken::CharacterToken},
        {'C', LanguageToken::CharacterToken},
        {'D', LanguageToken::CharacterToken},
        {'E', LanguageToken::CharacterToken},
        {'F', LanguageToken::CharacterToken},
        {'G', LanguageToken::CharacterToken},
        {'H', LanguageToken::CharacterToken},
        {'I', LanguageToken::CharacterToken},
        {'J', LanguageToken::CharacterToken},
        {'K', LanguageToken::CharacterToken},
        {'L', LanguageToken::CharacterToken},
        {'M', LanguageToken::CharacterToken},
        {'N', LanguageToken::CharacterToken},
        {'O', LanguageToken::CharacterToken},
        {'P', LanguageToken::CharacterToken},
        {'Q', LanguageToken::CharacterToken},
        {'R', LanguageToken::CharacterToken},
        {'S', LanguageToken::CharacterToken},
        {'T', LanguageToken::CharacterToken},
        {'U', LanguageToken::CharacterToken},
        {'V', LanguageToken::CharacterToken},
        {'W', LanguageToken::CharacterToken},
        {'X', LanguageToken::CharacterToken},
        {'Y', LanguageToken::CharacterToken},
        {'Z', LanguageToken::CharacterToken},
        {'_', LanguageToken::CharacterToken},
        {'0', LanguageToken::CharacterToken},
        {'1', LanguageToken::CharacterToken},
        {'2', LanguageToken::CharacterToken},
        {'3', LanguageToken::CharacterToken},
        {'4', LanguageToken::CharacterToken},
        {'5', LanguageToken::CharacterToken},
        {'6', LanguageToken::CharacterToken},
        {'7', LanguageToken::CharacterToken},
        {'8', LanguageToken::CharacterToken},
        {'9', LanguageToken::CharacterToken},
    };


};


#endif // LANGUAGEDICTIONARY_H

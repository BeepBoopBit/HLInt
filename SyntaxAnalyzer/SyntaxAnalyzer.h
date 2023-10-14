#ifndef SYNTAXANALYZER_H
#define SYNTAXANALYZER_H

#include <vector>
#include <iostream>
#include "../LanguageDictionary/LanguageDictionary.h"
#include "../ErrorHandler/ErrorHandler.h"


/** Entended Backus-Naur Form for HLint
 *
 * Letter := a | b | c | d | e | f | g | h | i | j | k | l | m | n | o | p | q | r | s | t | u | w | x | y | z
 * Digit := 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
 * WhiteSpace := ' '
 * End-Of-Statement := ';'
 *
 * Character := Letter | Digit 
 * Number := Digit {Digit}
 * Literal := '"' {Character} '"' | Number
 *
 * Identifier := letter, {letter | digit | '_'}, End-Of-Statement
 * Mathematical-Operator := '+' | '-'
 * Mathematical-Expression := Identifier, Mathematical-Operator, Identifier, (Mathematical-Operator, Identifier)*
 *
 * Assignment := Identifier, ':=', (Identifier | Literal | Mathematical-Expression), End-Of-Statement
 * Type := 'integer' | 'float'
 * Declaration := Identifier, ':', Type, End-Of-Statement
 * Output := 'Output', '<<', (Identifier | Literal), End-Of-Statement
 * Condition := (Identifier | Literal), ('<' | '>' | '==' | '!=') , (Identifier | Literal)
 * Statement := Assignment | Declaration | Output
 * One-Way-If-Condition := 'if', '(', Condition, ')', Statement, End-Of-Statement
**/


class SyntaxAnalyzer{


private:
    using LanguageToken = LanguageDictionary::LanguageToken;
    int parenthesis_count = 0, quote_count = 0;
    ErrorHandler* _errorHandler = &ErrorHandler::getInstance();

public:
    SyntaxAnalyzer(){
    }
    bool analyze(std::vector<LanguageToken> line_token){
        if (line_token[0] == LanguageToken::IdentifierToken){
            return _errorHandler->lastError(line_token[0],identifier(line_token,0));
        }
        else if (line_token[0] == LanguageToken::IfToken){
            return _errorHandler->lastError(line_token[0],oneWayIfCondition(line_token,0));
        }
        else if(line_token[0] == LanguageDictionary::NumberToken || isOperator(line_token[0])){
            return _errorHandler->lastError(line_token[0],mathematicalExpression(line_token,0));
        }
        else if(line_token[0] == LanguageDictionary::OutputToken){
            return _errorHandler->lastError(line_token[0],output(line_token,0));
        }else if(line_token[0] == LanguageDictionary::EndOfStatementToken){
            return true;
        }
        else{
            return _errorHandler->lastError(line_token[0]);
        }
    }

private:
    bool mathematicalExpression(std::vector<LanguageToken> &line_token, int position){
        // Position at Literal
        try{
            if(line_token[position] == LanguageDictionary::EndOfStatementToken){
                return true;
            }

            // Give Position at Identifier or Literal
            bool firstRule = isIdentifierOrLiteral(line_token[position++]);

            if(parenthesisBalancer(line_token[position++])){
                std::vector<LanguageToken> new_line_token(line_token.begin()+(position), line_token.end());
                return _errorHandler->error(line_token[position],analyze(new_line_token));
            }

            // get back to position
            position--;

            // Give Position at Operator
            bool secondRule = isOperator(line_token[position++]);

            // Probably a Conditional Statement
            if(firstRule && !secondRule){
                // Check if it's a condition
                if(isConditionalOperator(line_token[--position])){
                    ++position;
                    return _errorHandler->error(line_token[position],mathematicalExpression(line_token, position));
                }
            }else if(firstRule && secondRule){
                // Give Position at Identifier or Literal
                return _errorHandler->error(line_token[position],mathematicalExpression(line_token, position));
            }else{
                return _errorHandler->error(line_token[position]);
            }
        }catch(...){
            return _errorHandler->error(line_token[position]);
        }
        return _errorHandler->error(line_token[position]);
    }
    bool identifier(std::vector<LanguageToken> &line_token, int position){
        try{
            if(line_token[position+1] == LanguageToken::ColonToken){
                return _errorHandler->error(line_token[position],declaration(line_token, position+1));
            }
            else if(line_token[position+1] == LanguageToken::AssignmentToken){
                return _errorHandler->error(line_token[position],assignment(line_token, position+1));
            }
            else{
                return _errorHandler->error(line_token[position]);
            }
        }catch(...){
            return _errorHandler->error(line_token[position]);
        }
    }
    bool declaration(std::vector<LanguageToken> &line_token, int position){
        // Position at ColonToken
        try{
            if(line_token[position+1] == LanguageToken::TypeIntegerToken || line_token[position+1] == LanguageToken::TypeDoubleToken){
                if(line_token[position+2] == LanguageToken::EndOfStatementToken){
                    return true;
                }
                else{
                    return _errorHandler->error(line_token[position]);
                }
            }
        }catch(...){
            return _errorHandler->error(line_token[position]);
        }
        return false;
    }
    bool assignment(std::vector<LanguageToken> &line_token, int position){
        // Position at AssignmentToken
        try{
            // Check of Parenthesis
            parenthesisBalancer(line_token[position++]);

            if(isIdentifierOrLiteral(line_token[position++])){
                if(line_token[position++] == LanguageToken::EndOfStatementToken){
                    // Check if Parenthesis is balanced
                    if (parenthesis_count != 0){
                        return _errorHandler->error(line_token[position]);
                    }
                    _errorHandler->displaySuccess("AssignmentToken");
                    return true;
                }
                else{
                    if (isOperator(line_token[--position])){

                        // Position given at Operator
                        return _errorHandler->error(line_token[position],assignment(line_token, position));
                    }
                    else{
                        return _errorHandler->error(line_token[position]);
                    }
                }
            }
        }catch(...){
            return _errorHandler->error(line_token[position]);
        }
        return false;
    }
    bool oneWayIfCondition(std::vector<LanguageToken> &line_token, int position){
        // Position at IfToken
        try{
            parenthesisBalancer(line_token[(++position)++]);
            
            if(mathematicalExpression(line_token, position)){
                // Balance parenthesis
                if(parenthesisBalancer(line_token[position++]) && parenthesis_count != 0){
                    return _errorHandler->error(line_token[position]);
                }
                return true;

            }
        }catch(...){
            return _errorHandler->error(line_token[position]);
        }
        return _errorHandler->error(line_token[position]);
    }
    bool output(std::vector<LanguageToken> &line_token, int position){
        // Position at OutputToken
        try{
            bool firstRule = line_token[position++] == LanguageDictionary::OutputToken;
            bool secondRule = line_token[position++] == LanguageDictionary::LeftShiftToken;
            bool thirdRule = isIdentifierOrLiteral(line_token[position++]);
            bool fourtRule = line_token[position++] == LanguageDictionary::EndOfStatementToken;
            if(firstRule && secondRule && thirdRule && fourtRule){
                _errorHandler->displaySuccess("OutputToken");
                return true;
            }
        }catch(...){
            return _errorHandler->error(line_token[position]);
        }
        return _errorHandler->error(line_token[position]);
    }
private:
    bool parenthesisBalancer(LanguageToken token){
        if(token == LanguageToken::OpenParenthesisToken){
            parenthesis_count += 1;
            return true;
        }else if(token == LanguageToken::CloseParenthesisToken){
            parenthesis_count -= 1;
            if(parenthesis_count < 0){
                std::cout << "Error, no matching parenthesis. Still trying to Continue (DEBUG)" << std::endl;
                return true;
            }
            return true;
        }
        return false;
    }

// Auxillary Checker
private:
    bool isIdentifierOrLiteral(LanguageToken token){
        return token == LanguageToken::IdentifierToken || token == LanguageToken::LiteralToken || token == LanguageToken::NumberToken || token == LanguageToken::StringToken;
    }
    bool isConditionalOperator(LanguageToken token){
        return token == LanguageToken::LessThanToken || token == LanguageToken::GreaterThanToken || token == LanguageToken::EqualityToken || token == LanguageToken::NotEqualToken;
    }
    bool isOperator(LanguageToken token){
        return token == LanguageToken::PlusToken || token == LanguageToken::MinusToken;
    }
};

#endif // SYNTAXANALYZER_H

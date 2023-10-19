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
            return _errorHandler->lastError(line_token[0],identifier(line_token,1));
        }
        else if (line_token[0] == LanguageToken::IfToken){
            return _errorHandler->lastError(line_token[0],oneWayIfCondition(line_token,1));
        }
        else if(line_token[0] == LanguageDictionary::NumberToken || isOperator(line_token[0])){
            return _errorHandler->lastError(line_token[0],mathematicalExpression(line_token,1));
        }
        else if(line_token[0] == LanguageDictionary::OutputToken){
            return _errorHandler->lastError(line_token[0],output(line_token,1));
        }else if(line_token[0] == LanguageDictionary::EndOfStatementToken){
            return true;
        }
        else{
            return _errorHandler->lastError(line_token[0]);
        }
    }

private:
    
    bool identifier(std::vector<LanguageToken> &line_token, int position){
        try{
            if(line_token[position] == LanguageToken::ColonToken){
                // Give Position type
                return _errorHandler->error(line_token[position],declaration(line_token, position+1));
            }
            else if(line_token[position] == LanguageToken::AssignmentToken){
                // Give position literal or identifier
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
        // Position at Type
        try{
            if(line_token[position] == LanguageToken::TypeIntegerToken || line_token[position] == LanguageToken::TypeDoubleToken){
                if(line_token[position+1] == LanguageToken::EndOfStatementToken){
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
        // Position at Literal or Identifier or a Parenthesis
        try{
            return _errorHandler->error(line_token[position], mathematicalExpression(line_token, position));
        }catch(...){
            return _errorHandler->error(line_token[position]);
        }
        return false;
    }
    bool oneWayIfCondition(std::vector<LanguageToken> &line_token, int position){
        // Position at Parenthesis
        try{
            
            // Give Position are literal or identifier or Parenthesis
            // Check for expression (x+3)
            // Will balance parenthesis by its own
            if(mathematicalExpression(line_token, position, true)){
                // Balance parenthesis
                if(parenthesis_count != 0){
                    return _errorHandler->error(line_token[position]);
                }
                return true;
            }
        }catch(...){
            return _errorHandler->error(line_token[position]);
        }
        return _errorHandler->error(line_token[position]);
    }
    bool mathematicalExpression(std::vector<LanguageToken> &line_token, int position, bool conditional = false){
        // Position at Literal
        try{
            
            // is Optional, no need to check.
            parenthesisBalancer(line_token, position, true);

            // Will always be identifier or literal position
            bool firstRule = isIdentifierOrLiteral(line_token[position++]);

            if(conditional && isConditionalOperator(line_token[position])){
                return _errorHandler->error(line_token[position],mathematicalExpression(line_token, position+1,  conditional));
            }

            // Can be in a form of (3 + 5) + 8
            // 5 is a literal that can be followed by ONLY closing parenthesis token
            int lastPosition = position;
            if(!parenthesisBalancer(line_token, position, false)){
                _errorHandler->displayError("Bad Parenthesis Rule: Expected Closing Parenthesis (Mathematical Expression)", "SyntaxAnalyzer");
                return false;
            }

            if(conditional && position != lastPosition && parenthesis_count == 0){
                std::vector<LanguageToken> temp_line_token = std::vector<LanguageToken>(line_token.begin()+position, line_token.end());
                return _errorHandler->error(line_token[position], analyze(temp_line_token));
            }

            // Will awlays be operator position
            bool secondRule = isOperator(line_token[position]);

            // A Valid Expression
            if(firstRule && secondRule){
                return _errorHandler->error(line_token[position],mathematicalExpression(line_token, position+1, conditional));
            }
            
            // Probably an Error.
            // Every after an identifier, it 
            else if(!firstRule && secondRule){
                return _errorHandler->error(line_token[position]);
            }

            // EndOfStatement
            else if(line_token[position] == LanguageToken::EndOfStatementToken){
                return true;
            }
        }catch(...){
            return _errorHandler->error(line_token[position]);
        }
        return _errorHandler->error(line_token[position]);
    }
    
    bool output(std::vector<LanguageToken> &line_token, int position){
        // Position at LeftShift 
        try{
            bool firstRule = line_token[position++] == LanguageDictionary::LeftShiftToken;
            if(!firstRule){
                return _errorHandler->error(line_token[position]);
            }
            return _errorHandler->error(line_token[position], mathematicalExpression(line_token, position));
        }catch(...){
            return _errorHandler->error(line_token[position]);
        }
        return _errorHandler->error(line_token[position]);
    }
private:
    bool parenthesisBalancer(std::vector<LanguageToken> &line_token, int& position, bool open){
        
        // will fail if it's not a CloseParenthesisToken
        if (!open){
            while(isAuxParenthesis(line_token[position], false)){
                // Consume all parenthesis
                ++position;
            }
            return true;
        }

        // Assuumes that the input can be as many parenthesis as possible
        else{
            while(isAuxParenthesis(line_token[position], true)){
                // Consume all parenthesis
                ++position;
            }
            return true;
        }

        return false;
    }

    bool isAuxParenthesis(LanguageToken& token, bool open){
        if(open && (token == LanguageToken::OpenParenthesisToken)){
            parenthesis_count += 1;
            return true;
        }else if(!open && (token == LanguageToken::CloseParenthesisToken)){
            parenthesis_count -= 1;
            if(parenthesis_count < 0){
                _errorHandler->displayError("Error, more close parenthesis than open one. (DEBUG) Still Trying to continue", "SyntaxAnalyzer");
                return true;
            }
            return true;
        }
        return false;
    }

    bool isParenthesis(LanguageToken& token){
        if(token == LanguageToken::OpenParenthesisToken){
            parenthesis_count += 1;
            return true;
        }else if(token == LanguageToken::CloseParenthesisToken){
            parenthesis_count -= 1;
            if(parenthesis_count < 0){
                _errorHandler->displayError("Error, more close parenthesis than open one. (DEBUG) Still Trying to continue", "SyntaxAnalyzer");
                return true;
            }
            return true;
        }
        return false;
    }


// Auxillary Checker
public:
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

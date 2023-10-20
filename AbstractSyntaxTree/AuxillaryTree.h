#ifndef AUXILLARYTREE_H
#define AUXILLARYTREE_H

#include "../LanguageDictionary/LanguageDictionary.h"

class AuxillaryTree{
private:
    using LanguageToken = LanguageDictionary::LanguageToken;

public:
    LanguageToken _token = LanguageToken::InvalidToken;
    AuxillaryTree* _left = nullptr;
    AuxillaryTree* _right = nullptr;
    std::string _value = "";

    int _line = 0;
    int _column = 0;

public:
    AuxillaryTree(LanguageToken token, std::string value, int line, int column): _token(token), _value(value), _line(line), _column(column){
    }
    ~AuxillaryTree(){}
};
#endif // AUXILLARYTREE_H

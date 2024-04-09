#pragma once

#ifndef AE_H
#define AE_H

#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <math.h>
#include <tuple>
#include <algorithm>
#include <regex>

using namespace std;
#define NUM 1
#define ADD 2
#define SUB 3
#define WITH 4
#define ID 5
#define FUN 6
#define APP 7


class AST
{

public:
    int type;
    string num = "0";
    string id = "";
    string result = "";
    AST *lhs;
    AST *rhs;

    AST *closureV;
    
    vector<string> dsName;
    vector<string> dsValue;
    AST *dsNext;
    

    void createNum(string);
    void createAdd(AST *, AST *);
    void createSub(AST *, AST *);
    void createWith(AST *, AST *);
    void createId(string);
    void createFun(AST *);
    void createApp(AST *, AST *);

    AST *getLhs();
    AST *getRhs();

    string getASTCode();
};

void AST::createNum(string stringNum)
{
    num = stringNum;
}

void AST::createAdd(AST *astlhs, AST *astrhs)
{
    lhs = astlhs;
    rhs = astrhs;
}

void AST::createSub(AST *astlhs, AST *astrhs)
{
    lhs = astlhs;
    rhs = astrhs;
}

void AST::createWith(AST *namedExpr, AST *body)
{
    lhs = namedExpr;
    rhs = body;
}

void AST::createId(string name)
{
    id = name;
}

void AST::createFun(AST *body)
{
    lhs = body;
}

void AST::createApp(AST *ftn, AST *arg)
{
    lhs = ftn;
    rhs = arg;
}

AST *AST::getLhs()
{
    return lhs;
}

AST *AST::getRhs()
{
    return rhs;
}

string AST::getASTCode()
{
    string astCode = "";
    string numCode = "(num ";
    string addCode = "(add ";
    string subCode = "(sub ";
    string withCode = "(app (fun ";
    string idCode = "(id ";
    string funCode = "(fun ";
    string appCode = "(app ";

    if (type == NUM)
    {
        astCode += numCode;
        astCode += num;
        astCode += ")";
        return astCode;
    }

    if (type == ADD)
    {
        astCode += addCode;
        astCode += lhs->getASTCode();
        astCode += " ";
        astCode += rhs->getASTCode();
        astCode += ")";
        return astCode;
    }

    if (type == SUB)
    {
        astCode += subCode;
        astCode += lhs->getASTCode();
        astCode += " ";
        astCode += rhs->getASTCode();
        astCode += ")";
        return astCode;
    }

    if (type == WITH)
    {
        astCode += withCode;
        astCode += "'";
        astCode += id;
        astCode += " ";
        astCode += rhs->getASTCode();
        astCode += ")";
        astCode += " ";
        astCode += lhs->getASTCode();
        astCode += ")";
        return astCode;
    }

    if (type == ID)
    {
        astCode += idCode;
        astCode += "'";
        astCode += id;
        astCode += ")";
        return astCode;
    }

    if (type == FUN)
    {
        astCode += funCode;
        astCode += "'";
        astCode += id;
        astCode += " ";
        astCode += lhs->getASTCode();
        astCode += ")";
        return astCode;
    }

    if (type == APP){
        astCode += appCode;
        astCode += lhs->getASTCode();
        astCode += " ";
        astCode += rhs->getASTCode();
        astCode += ")";
        return astCode;
    }

    return astCode;
}

#endif
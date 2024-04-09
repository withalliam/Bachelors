#pragma once

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <math.h>
#include "AE.h"
#include "Parser.h"

class Interpreter
{
public:
    AST *interp(AST *, AST *);
    string numAdd(AST *, AST *);
    string numSub(AST *, AST *);
    AST *strict(AST *);
};

AST *Interpreter::interp(AST *sexp, AST *ds)
{
    string numVCode = "(numV ";
    double resultAdd;
    if (sexp->type == NUM)
    {
        sexp->result += numVCode;
        sexp->result += sexp->num;
        sexp->result += ")";
        return sexp;
    }

    if (sexp->type == ADD)
    {
        sexp->result = numAdd((interp(sexp->lhs, sexp->dsNext)), (interp(sexp->rhs, sexp->dsNext)));
        return sexp;
    }

    if (sexp->type == SUB)
    {
        sexp->result = numSub((interp(sexp->lhs, sexp->dsNext)), (interp(sexp->rhs, sexp->dsNext)));
        return sexp;
    }

    if (sexp->type == ID)
    {
        // id (s) (lookup s ds)
        return sexp;
    }

    if (sexp->type == WITH)
    {
    }

    if (sexp->type == FUN)
    {
        sexp->closureV = sexp;
        return sexp;
    }

    if (sexp->type == APP)
    {
        sexp = (interp(sexp->closureV->rhs, interp(sexp->lhs, sexp->dsNext)));

        return sexp;
    }
}

AST *Interpreter::strict(AST *)
{
}

string Interpreter::numAdd(AST *addlhs, AST *addrhs)
{
    string output = "";
    string numVCode = "(numV ";
    stringstream result;

    istringstream ssl(addlhs->result), ssr(addrhs->result);
    string stringBuffer;
    vector<string> lhs, rhs;
    while (getline(ssl, stringBuffer, ' '))
    {
        lhs.push_back(stringBuffer);
    }
    while (getline(ssr, stringBuffer, ' '))
    {
        rhs.push_back(stringBuffer);
    }

    lhs.at(1).pop_back();
    rhs.at(1).pop_back();

    result << stod(lhs.at(1)) + stod(rhs.at(1));
    output += numVCode;
    output += result.str();
    output += ")";

    return output;
}

string Interpreter::numSub(AST *sublhs, AST *subrhs)
{
    string output = "";
    string numVCode = "(numV ";
    stringstream result;

    istringstream ssl(sublhs->result), ssr(subrhs->result);
    string stringBuffer;
    vector<string> lhs, rhs;
    while (getline(ssl, stringBuffer, ' '))
    {
        lhs.push_back(stringBuffer);
    }
    while (getline(ssr, stringBuffer, ' '))
    {
        rhs.push_back(stringBuffer);
    }

    lhs.at(1).pop_back();
    rhs.at(1).pop_back();

    result << stod(lhs.at(1)) - stod(rhs.at(1));
    output += numVCode;
    output += result.str();
    output += ")";

    return output;
}

#endif
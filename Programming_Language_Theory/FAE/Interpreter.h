#pragma once

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include "AE.h"

class Interpreter
{
public:
    tuple<AST *, AST *> interp(AST *, AST *);
    string numAdd(AST *, AST *);
    string numSub(AST *, AST *);
    AST *strict(AST *);
    AST *lookup(AST *, AST *);
};

tuple<AST *, AST *> Interpreter::interp(AST *sexp, AST *ds)
{

    string numVCode = "(numV ";
    double resultAdd;
    if (sexp->type == NUM)
    {
        sexp->result += numVCode;
        sexp->result += sexp->num;
        sexp->result += ")";
        return make_tuple(sexp, ds);
    }

    if (sexp->type == ADD)
    {
        sexp->result = numAdd(get<0>((interp(sexp->lhs, ds))), get<0>((interp(sexp->rhs, ds))));
        return make_tuple(sexp, ds);
    }

    if (sexp->type == SUB)
    {
        sexp->result = numSub(get<0>((interp(sexp->lhs, ds))), get<0>((interp(sexp->rhs, ds))));
        return make_tuple(sexp, ds);
    }

    if (sexp->type == ID)
    {
        sexp = lookup(sexp, ds);
        return make_tuple(sexp, ds);
    }

    if (sexp->type == WITH)
    {
        if (sexp->lhs->type == FUN)
        {
            sexp->funName.push_back(sexp->id);
            sexp->closureV = sexp->lhs;
            sexp->closureV->type = CLOSUREV;
        }
        else
        {
            ds->dsName.push_back(sexp->id);
            ds->dsValue.push_back(get<0>(interp(sexp->lhs, ds))->result);
        }
        if (!sexp->funName.empty())
        {
            sexp->rhs->closureV = sexp->closureV;
            sexp->rhs->funName = sexp->funName;
        }
        sexp->result = get<0>(interp(sexp->rhs, ds))->result;
        return make_tuple(sexp, ds);
    }

    if (sexp->type == FUN)
    {

        return make_tuple(sexp, ds);
    }

    if (sexp->type == APP)
    {
        ds->dsName.push_back(sexp->closureV->id);
        ds->dsValue.push_back(get<0>(interp(sexp->rhs, ds))->result);
        sexp->result = get<0>(interp(sexp->closureV, ds))->result;
        return make_tuple(sexp, ds);
    }

    if (sexp->type == CLOSUREV)
    {
        sexp->result = get<0>(interp(sexp->lhs, ds))->result;
        return make_tuple(sexp, ds);
    }

    cout << "syntax error" << endl;
}

// AST *Interpreter::strict(AST *)
// {
// }

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

AST *Interpreter::lookup(AST *sexp, AST *ds)
{
    string free = "free";

    int index = 0;
    for (index = 0; index < ds->dsName.size(); index++)
    {
        if (ds->dsName[index] == sexp->id)
            break;
    }
    if (index == ds->dsName.size())
    {
        cout << "Free identifier";
        return EXIT_SUCCESS;
    }

    sexp->result = ds->dsValue[index];

    return sexp;

    // static scope로 하려면 쭉 가다가 identifier를 찾으면 return하고, 못찾으면 error
    // dynamic scope로 하려면 제일 마지막꺼를 찾아서 return?
    // 아니면 저장 할 때 그 값을 저장해야함.
}

#endif
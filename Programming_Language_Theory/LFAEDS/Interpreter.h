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
    AST *lookup(AST *, AST *);
    AST *dslookup(AST *, AST *);
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
        sexp->lhs->funName = sexp->funName;
        sexp->rhs->funName = sexp->funName;
        sexp->lhs->dsfunName = sexp->dsfunName;
        sexp->rhs->dsfunName = sexp->dsfunName;
        sexp->result = numAdd(get<0>((interp(sexp->lhs, ds))), get<0>((interp(sexp->rhs, ds))));
        return make_tuple(sexp, ds);
    }

    if (sexp->type == SUB)
    {
        sexp->lhs->funName = sexp->funName;
        sexp->rhs->funName = sexp->funName;
        sexp->lhs->dsfunName = sexp->dsfunName;
        sexp->rhs->dsfunName = sexp->dsfunName;
        sexp->result = numSub(get<0>((interp(sexp->lhs, ds))), get<0>((interp(sexp->rhs, ds))));
        return make_tuple(sexp, ds);
    }

    if (sexp->type == ID)
    {
        if (!sexp->funName.empty())
            sexp = lookup(sexp, ds);
        else if (!sexp->dsfunName.empty())
            sexp = dslookup(sexp, ds);
        else
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
        else if (sexp->lhs->type == DSFUN)
        {
            sexp->dsfunName.push_back(sexp->id);
            sexp->exprV = sexp->lhs;
            sexp->exprV->type = EXPRV;
        }
        else
        {
            ds->dsName.push_back(sexp->id);
            ds->dsValue.push_back(get<0>(interp(sexp->lhs, ds))->result);
        }

        if (!sexp->funName.empty())
        {
            sexp->rhs->closureV = sexp->closureV;
            sexp->lhs->funName = sexp->funName;
            sexp->rhs->funName = sexp->funName;
        }
        else if (!sexp->dsfunName.empty())
        {
            sexp->rhs->exprV = sexp->exprV;
            sexp->lhs->dsfunName = sexp->dsfunName;
            sexp->rhs->dsfunName = sexp->dsfunName;
        }

        sexp->result = get<0>(interp(sexp->rhs, ds))->result;
        return make_tuple(sexp, ds);
    }

    if (sexp->type == FUN)
    {
        sexp->type = CLOSUREV;
        sexp->result = sexp->getASTCode();
        return make_tuple(sexp, ds);
    }

    if (sexp->type == APP)
    {
        if (!sexp->funName.empty())
        {
            ds->dsName.push_back(sexp->closureV->id);
            ds->dsValue.push_back(get<0>(interp(sexp->rhs, ds))->result);
            sexp->result = get<0>(interp(sexp->closureV, ds))->result;
        }
        else if (!sexp->dsfunName.empty())
        {
            ds->dsName.push_back(sexp->exprV->id);
            ds->dsValue.push_back(get<0>(interp(sexp->rhs, ds))->result);
            sexp->result = get<0>(interp(sexp->exprV, ds))->result;
        }

        return make_tuple(sexp, ds);
    }

    if (sexp->type == CLOSUREV)
    {
        sexp->lhs->funName = sexp->funName;
        sexp->result = get<0>(interp(sexp->lhs, ds))->result;
        return make_tuple(sexp, ds);
    }

    if (sexp->type == EXPRV)
    {
        sexp->lhs->dsfunName = sexp->dsfunName;
        sexp->result = get<0>(interp(sexp->lhs, ds))->result;
        return make_tuple(sexp, ds);
    }

    cout << "syntax error" << endl;
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

AST *Interpreter::lookup(AST *sexp, AST *ds)
{
    int index;
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
}

AST *Interpreter::dslookup(AST *sexp, AST *ds)
{
    int index;
    for (index = ds->dsName.size() - 1; index >= 0; index--)
    {
        if (ds->dsName[index] == sexp->id)
            break;
    }
    if (index < 0)
    {
        cout << "Free identifier";
        return EXIT_SUCCESS;
    }

    sexp->result = ds->dsValue[index];

    return sexp;
}

#endif
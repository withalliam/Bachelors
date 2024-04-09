#pragma once

#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <regex>
#include "AE.h"

class Parser
{
public:
    // Parser();
    vector<string> splitExpressionAsSubExpressions(string);
    vector<string> getSubExpressions(string);
    AST *parse(string);
    bool isNumeric(string);
    bool isId(string);
};

vector<string> Parser::splitExpressionAsSubExpressions(string exampleCode)
{
    if ((exampleCode.front() == '{' && exampleCode.back() != '}') || (exampleCode.front() != '{' && exampleCode.back() == '}'))
        exit(0);

    if (exampleCode.front() == '{')
        exampleCode = exampleCode.substr(1, exampleCode.length() - 1);
    return getSubExpressions(exampleCode);
}

vector<string> Parser::getSubExpressions(string exampleCode)
{
    vector<string> sexpressions = vector<string>();
    int openingParenthesisCount = 0;
    string strBuffer = "";
    for (int i = 0; i < exampleCode.length(); i++)
    {
        if (i == 0 || (i == 0 && exampleCode.at(i) == '{'))
        {
            strBuffer = strBuffer + exampleCode.at(i);
            if (exampleCode.at(i) == '{')
            {
                openingParenthesisCount++;
            }
            continue;
        }
        else if (exampleCode.at(i) == ' ' && openingParenthesisCount == 0)
        {
            // buffer is ready to be a subexpression
            if (strBuffer.size() > 0)
            {
                sexpressions.push_back(strBuffer);
                strBuffer = ""; // Ready to start a new buffer
            }
            continue;
        }
        else
        {
            if (exampleCode.at(i) == '{' && openingParenthesisCount == 0)
            {
                openingParenthesisCount++;
                // Ready to start a new buffer
                strBuffer = "";
                strBuffer += exampleCode.at(i);
                continue;
            }
            else if (exampleCode.at(i) == '{')
            {
                openingParenthesisCount++;
                strBuffer = strBuffer + exampleCode.at(i);
                continue;
            }
            else if (exampleCode.at(i) == '}' && openingParenthesisCount > 0)
            {
                openingParenthesisCount--;
                strBuffer = strBuffer + exampleCode.at(i);
                continue;
            }
            else if (exampleCode.at(i) == '}')
            {
                // buffer is ready to be a subexpression
                sexpressions.push_back(strBuffer);
                continue;
            }
        }
        strBuffer = strBuffer + exampleCode.at(i);
    }

    sexpressions.push_back(strBuffer);

    return sexpressions;
}

AST *Parser::parse(string exampleCode)
{
    vector<string> subExpressions = splitExpressionAsSubExpressions(exampleCode);
    // for (auto i = subExpressions.begin(); i != subExpressions.end(); ++i)
    //     cout << *i << endl;
    // cout << '?' << endl;

    if (subExpressions.size() == 1 && isNumeric(subExpressions.front()))
    {
        AST *num = new AST();
        num->type = NUM;
        num->createNum(subExpressions.front());
        return num;
    }

    if (subExpressions.front().front() == '+')
    {
        AST *add = new AST();
        add->type = ADD;
        add->createAdd(parse(subExpressions.at(1)), parse(subExpressions.at(2)));
        return add;
    }

    if (subExpressions.front().front() == '-')
    {
        AST *sub = new AST();
        sub->type = SUB;
        sub->createSub(parse(subExpressions.at(1)), parse(subExpressions.at(2)));
        return sub;
    }

    if (subExpressions.front() == "with")
    {
        AST *with = new AST();
        with->type = WITH;
        with->id = subExpressions.at(1).at(1);
        with->createWith(parse(splitExpressionAsSubExpressions(subExpressions.at(1)).at(1)), parse(subExpressions.at(2)));
        return with;
    }

    if ((isId(subExpressions.front())) && (subExpressions.size() < 2))
    {
        AST *id = new AST();
        id->type = ID;
        id->createId(subExpressions.front());
        return id;
    }

    if (subExpressions.front() == "fun")
    {
        AST *fun = new AST();
        fun->type = FUN;
        fun->id = subExpressions.at(1).at(1);
        fun->createFun(parse(subExpressions.at(2)));
        return fun;
    }

    if (isId(subExpressions.front()))
    {
        AST *app = new AST();
        app->type = APP;
        app->createApp(parse(subExpressions.at(0)), parse(subExpressions.at(1)));
        return app;
    }

    cout << "bad syntax" << endl;
    return EXIT_SUCCESS;

}

bool Parser::isNumeric(string str)
{
    regex pattern("-?\\d+(\\.\\d+)?");
    smatch match;
    return regex_match(str, match, pattern);
}

bool Parser::isId(string str)
{
    regex pattern("\\w+");
    smatch match;
    return regex_match(str, match, pattern);
}

#endif
#include <iostream>
#include "AE.h"
#include "Parser.h"
#include "Interpreter.h"

using namespace std;

int main(int argc, char *argv[])
{
    string pOption, ConcreteCode;
    AST *ast;
    tuple<AST *, AST *> AbstractCode;
    Parser parser;
    Interpreter interpreter;

    if (argc >= 3)
    {
        if (strcmp(argv[1], "-p") == 0)
        {
            pOption = argv[1];
            ConcreteCode = argv[2];
        }
        else
            return 0;
    }
    else if (argc == 2)
    {
        pOption = "Interprete";
        ConcreteCode = argv[1];
    }

    if (pOption == "-p")
    {
        ast = parser.parse(ConcreteCode);
        cout << ast->getASTCode();
        return 0;
    }
    else if (pOption == "Interprete")
    {
        ast = parser.parse(ConcreteCode);
        AbstractCode = interpreter.interp(ast, ast);
        cout << get<0>(AbstractCode)->result;
        return 0;
    }
    else
        cout << "Syntax Error.";

    return 0;
}